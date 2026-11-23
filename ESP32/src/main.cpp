#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <DHT.h>

#include "config.h"

DHT dht(DHT_PIN, DHT11);
WebServer server(HTTP_PORT);

float ultimaTemperatura = NAN;
float ultimaHumedad = NAN;
bool lecturaValida = false;

static void conectarWiFi() {
  WiFi.mode(WIFI_STA);

#if USE_STATIC_IP
  IPAddress ip(STATIC_IP);
  IPAddress gateway(STATIC_GATEWAY);
  IPAddress subnet(STATIC_SUBNET);
  if (!WiFi.config(ip, gateway, subnet)) {
    Serial.println("Fallo al configurar IP estática");
  }
#endif

  Serial.printf("Conectando a WiFi \"%s\"...\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("WiFi OK. IP: ");
  Serial.println(WiFi.localIP());
}

static bool leerDht() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Error leyendo DHT11");
    return false;
  }

  ultimaTemperatura = t;
  ultimaHumedad = h;
  lecturaValida = true;
  Serial.printf("DHT11 -> T=%.1f C  H=%.1f %%\n", t, h);
  return true;
}

static void agregarCors() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

static void handleDatos() {
  agregarCors();
  if (!leerDht()) {
    server.send(503, "application/json",
                "{\"error\":\"lectura_dht_fallida\",\"temperatura\":null,\"humedad\":null}");
    return;
  }

  char body[96];
  snprintf(body, sizeof(body),
           "{\"temperatura\":%.1f,\"humedad\":%.1f}",
           ultimaTemperatura, ultimaHumedad);
  server.send(200, "application/json", body);
}

static bool servirArchivo(const char *path, const char *contentType) {
  if (!LittleFS.exists(path)) {
    return false;
  }
  File f = LittleFS.open(path, "r");
  if (!f) {
    return false;
  }
  server.streamFile(f, contentType);
  f.close();
  return true;
}

static void handleNotFound() {
  if (server.method() == HTTP_OPTIONS) {
    agregarCors();
    server.send(204);
    return;
  }

  String uri = server.uri();
  if (LittleFS.exists(uri)) {
    const char *contentType = "text/plain";
    if (uri.endsWith(".html")) contentType = "text/html";
    else if (uri.endsWith(".js")) contentType = "application/javascript";
    else if (uri.endsWith(".css")) contentType = "text/css";
    File f = LittleFS.open(uri, "r");
    server.streamFile(f, contentType);
    f.close();
    return;
  }
  server.send(404, "text/plain", "Not found");
}

static void configurarRutas() {
  server.on("/datos", HTTP_GET, handleDatos);
  server.on("/datos", HTTP_OPTIONS, []() {
    agregarCors();
    server.send(204);
  });

  server.on("/", HTTP_GET, []() {
    if (!servirArchivo("/esp32-monitor.html", "text/html")) {
      server.send(500, "text/plain", "esp32-monitor.html no encontrado en LittleFS");
    }
  });

  server.onNotFound(handleNotFound);
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("ESP32 + DHT11 — variante A");

  dht.begin();

  if (!LittleFS.begin(true)) {
    Serial.println("Error montando LittleFS");
  } else {
    Serial.println("LittleFS montado");
  }

  conectarWiFi();
  configurarRutas();
  server.begin();
  Serial.printf("HTTP en http://%s/\n", WiFi.localIP().toString().c_str());
  Serial.println("GET /datos — lectura a demanda");
}

void loop() {
  server.handleClient();
}

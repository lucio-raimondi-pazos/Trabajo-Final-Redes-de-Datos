#pragma once

// Credenciales de la LAN compartida (editar según el router del laboratorio)
#ifndef WIFI_SSID
#define WIFI_SSID "Juancito"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "9182736450"
#endif

// Pin de datos del DHT11
#ifndef DHT_PIN
#define DHT_PIN 4
#endif

// IP estática (opcional). Dejar USE_STATIC_IP en 0 para DHCP.
#ifndef USE_STATIC_IP
#define USE_STATIC_IP 1
#endif

#if USE_STATIC_IP
#define STATIC_IP     192, 168, 100, 13
#define STATIC_GATEWAY 192, 168, 100, 1
#define STATIC_SUBNET  255, 255, 255, 0
#endif

#define HTTP_PORT 80

# ESP32 — servidor HTTP + DHT11

Firmware de la variante A: el nodo es el servidor; la PC (o el dashboard) hace `GET /datos`.

## Hardware

| Señal DHT11 | ESP32 |
|-------------|-------|
| VCC         | 3V3   |
| GND         | GND   |
| DATA        | GPIO4 |

Si el módulo no trae resistencia de pull-up, poner ~10 kΩ entre DATA y 3V3.

## Configuración

Editar [`include/config.h`](include/config.h):

- `WIFI_SSID` / `WIFI_PASSWORD` — red del laboratorio
- `DHT_PIN` — por defecto `4`
- `USE_STATIC_IP` — poner en `1` y ajustar `STATIC_IP` / gateway / máscara cuando la cátedra asigne el plan de direcciones

## Build y flasheo

Requiere [PlatformIO](https://platformio.org/).

```bash
cd ESP32
pio run                 # compilar
pio run -t upload       # firmware
pio run -t uploadfs     # WebApp/ → LittleFS
pio device monitor      # Serial 115200 — muestra la IP
```

`platformio.ini` usa `data_dir = ../WebApp`: el contenido de `WebApp/` se sube al sistema de archivos del ESP.

## API

### `GET /datos`

Lee el sensor en ese momento, guarda la última lectura en RAM y responde:

```json
{"temperatura": 24.5, "humedad": 55.0}
```

Si falla la lectura del DHT11 → HTTP 503 con `temperatura`/`humedad` en `null`.

### `GET /`

Sirve el monitor [`WebApp/esp32-monitor.html`](../WebApp/esp32-monitor.html). También está en `/esp32-monitor.html`.

CORS está habilitado en `/datos` por si abrís el HTML fuera del ESP.

## Prueba rápida

```bash
curl http://<IP>/datos
```

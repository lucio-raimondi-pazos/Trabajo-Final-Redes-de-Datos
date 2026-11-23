# Trabajo Final — Redes de Datos (Variante A)

Nodo **ESP32 + DHT11** como servidor HTTP en la LAN compartida. La PC consulta al nodo (pull).

| Ruta | Qué hace |
|------|----------|
| `GET /` | Monitor (`esp32-monitor.html` desde LittleFS) |
| `GET /datos` | Lee el DHT11 **a demanda** y responde JSON |

Ejemplo:

```json
{"temperatura": 24.5, "humedad": 55.0}
```

## Estructura

- [`ESP32/`](ESP32/) — firmware PlatformIO (WiFi, DHT11, servidor HTTP, LittleFS)
- [`WebApp/`](WebApp/) — código del dashboard; se flashea al ESP con `uploadfs`

## Inicio rápido

1. Editar WiFi (y opcionalmente IP) en [`ESP32/include/config.h`](ESP32/include/config.h).
2. Cablear DHT11: VCC→3V3, GND→GND, DATA→GPIO4.
3. Desde `ESP32/`:

```bash
pio run -t upload
pio run -t uploadfs
pio device monitor
```

4. Abrir `http://<IP_DEL_ESP>/` y pulsar **Actualizar** (consulta a demanda).

Detalle: [`ESP32/README.md`](ESP32/README.md) · [`WebApp/README.md`](WebApp/README.md).

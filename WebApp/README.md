# WebApp — dashboard del nodo

Código del front que **sirve el ESP32** vía LittleFS (`data_dir = ../WebApp` en PlatformIO).

## Archivos

| Archivo | Rol |
|---------|-----|
| `esp32-monitor.html` | Monitor (UI + lógica). El ESP lo sirve en `/` |

Tras cambiar el HTML:

```bash
cd ../ESP32
pio run -t uploadfs
```

## Uso

- Abrí `http://<IP_DEL_ESP>/`
- **Mock** — alterna datos de demostración
- **Actualizar** — una consulta espontánea a `GET /datos`
- **Auto** + N s — consulta periódica cada N segundos (Detener para cortar)

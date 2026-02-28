# MQTT Specification – WATERFRONT Kayak Controller

**Purpose**: exact topics and JSON schemas. This is critical because backend (Supabase/webhook) and ESP32 must speak the same language.

Broker: [your Mosquitto / EMQX / HiveMQ address:port]  
TLS: planned for production  
QoS: 1 or 2 for commands, 0 for telemetry

## Topics

Inbound (backend → ESP32)
- `/kayak/{machineId}/unlock`          QoS 1   JSON payload

Outbound (ESP32 → backend)
- `/kayak/{machineId}/status`          QoS 0   periodic + on change
- `/kayak/{machineId}/event`           QoS 1   taken / returned / error

## Payload Schemas (JSON)

Unlock command example:
```json
{
  "bookingId": "bk_abc123",
  "pin": "7482",
  "durationSeconds": 7200,
  "timestamp": "2026-02-28T11:43:00Z"
}
```

Status telemetry (periodic, ~every 5 min):

JSON

```
{
  "machineId": "bremen-harbor-01",
  "kayakPresent": true,
  "locked": true,
  "batteryPercent": 87,
  "connection": "wifi",           // or "lte" or "offline"
  "rssi": -58,
  "lastEvent": "returned",
  "uptimeMinutes": 1456,
  "timestamp": "2026-02-28T11:43:00Z"
}
```

Event (on take/return/error):

JSON

```
{
  "machineId": "bremen-harbor-01",
  "event": "taken" | "returned" | "error",
  "timestamp": "2026-02-28T11:43:00Z",
  "details": { "distanceCm": 12 }   // optional sensor data
}
```






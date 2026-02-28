# MQTT Specification – WATERFRONT Kayak Controller

**Purpose**: exact topics and JSON schemas. This is critical because backend (Supabase/webhook) and ESP32 must speak the same language.

Broker: [your Mosquitto / EMQX / HiveMQ address:port]  
TLS: planned for production  
QoS: 1 or 2 for commands, 0 for telemetry

## Topics

Inbound (backend → ESP32)
- `waterfront/slots/{slotId}/status`          QoS 1   JSON payload (retained)
- `waterfront/slots/{slotId}/command`         QoS 1   JSON payload

Outbound (ESP32 → backend)
- `waterfront/machine/{machineId}/status`     QoS 0   machine telemetry (retained) – provisioning, IP, conn type, battery etc.
- `waterfront/slots/{slotId}/status`          QoS 0   slot telemetry (retained) – booked, gateState, bookingId etc.
- `waterfront/slots/{slotId}/event`           QoS 1   taken / returned / error
- `waterfront/slots/{slotId}/ack`             QoS 1   acknowledgments

## Payload Schemas (JSON)

Machine status (retained) example:
```json
{
  "state": "idle",
  "battery": 92,
  "connType": "wifi"
}
```

Slot status (retained) example:
```json
{
  "slotId": 1,
  "booked": true,
  "bookingId": "BKG-20260228-001",
  "startTime": "2026-03-01T10:00:00Z",
  "endTime": "2026-03-01T12:00:00Z",
  "customerName": "Marco Rossi",
  "gateState": "locked"
}
```

Command example:
```json
"open_gate"
```

Event (on take/return/error):

JSON

```
{
  "slotId": 1,
  "event": "taken" | "returned" | "error",
  "timestamp": "2026-02-28T11:43:00Z",
  "details": { "distanceCm": 12 }   // optional sensor data
}
```

Ack example:
```json
{
  "slotId": 1,
  "action": "gate_opened",
  "timestamp": 1234567890
}
```






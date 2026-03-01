# WATERFRONT ESP32 API Reference

## Introduction
This guide explains how the WATERFRONT system communicates with other devices or servers using MQTT (a messaging protocol for IoT). Think of MQTT like sending emails between devices. The system sends and receives messages on specific "topics" (like email subjects).

## MQTT Topics

### Incoming Commands (What the System Listens For)
These are messages you can send to control the system remotely.

- `waterfront/{location}/{code}/command`: Commands to control the system.
  - `{location}`: The location name, like "bremen".
  - `{code}`: The location code, like "harbor-01".
  - Example: `{"action": "open_gate", "compartment": 1}` - This tells the system to open the gate for compartment 1.

#### Supported Commands
- `{"action": "open_gate", "compartment": 1}`: Opens the gate for a specific compartment.
- `{"action": "close_gate", "compartment": 1}`: Closes the gate for a specific compartment.
- `{"action": "query_state", "compartment": 1}`: Queries the gate state for a compartment.
- `waterfront/{location}/{code}/config/update`: Updates the system's settings with new configuration.
  - Payload: Full JSON config object.
- `waterfront/{location}/{code}/ota/update`: Triggers an OTA update.
  - Payload: URL of the firmware binary.
- `waterfront/{location}/{code}/booking/paid`: Notifies of a paid booking.
  - Payload: `{"bookingId": "bk_123", "compartmentId": 1, "durationSec": 3600}`.
- `waterfront/{location}/{code}/compartments/+/command`: Commands for specific compartments.
  - Examples: `"open_gate"`, `"close_gate"`, `"query_state"`.
- `waterfront/{location}/{code}/compartments/+/status`: Retained status updates for compartments.
  - Payload: `{"booked": true, "gateState": "locked", "crc": 1234567890}`.

### Outgoing Responses (What the System Sends Back)
These are replies from the system after processing commands.

- `waterfront/{location}/{code}/response`: Responses to commands.
  - Example: `{"ack": true, "action": "open_gate", "compartment": 1, "status": "success", "timestamp": 1234567890}`
    - `ack`: Acknowledgment (true if received).
    - `status`: Result, like "success" or "error".
    - `timestamp`: Time when the response was sent.

### Telemetry (Automatic Messages from the System)
The system sends these messages regularly to report its status.

- `waterfront/{location}/{code}/debug`: Health and status updates every 60 seconds.
  - Payload: `{"uptime": 3600, "heapFree": 204800, "fwVersion": "0.9.2-beta", "batteryPercent": 85, "tasks": 5, "reconnects": 2}`
    - `uptime`: Seconds since boot.
    - `heapFree`: Free heap memory in bytes.
    - `fwVersion`: Current firmware version.
    - `batteryPercent`: Battery level as percentage.
    - `tasks`: Number of active FreeRTOS tasks.
    - `reconnects`: MQTT reconnect count.
- `waterfront/{location}/{code}/alert`: Error and alert messages.
  - Payload: `{"alert": "low_power", "batteryPercent": 15, "solarVoltage": 2.5, "timestamp": 1234567890}` or `{"error": "fatal", "code": "ESP_FAIL", "timestamp": 1234567890}`.
- `waterfront/machine/{code}/status`: Overall machine status.
  - Payload: `{"state": "idle", "battery": 92, "connType": "wifi"}`.
- `waterfront/slots/{id}/status`: Status of individual compartments (slots).
  - Payload: `{"slotId": 1, "state": "open", "booked": false}`.
- `waterfront/locations/{code}/depositRelease`: Messages about releasing deposits.
  - Payload: `{"bookingId": "bk_123", "release": true}`.
- `waterfront/locations/{code}/returnConfirm`: Confirmations when items are returned.
  - Payload: `{"bookingId": "bk_123", "action": "autoLock"}`.
- `waterfront/{location}/{code}/ota/status`: OTA update status.
  - Payload: `{"otaResult": "success", "firmwareVersion": "0.9.2-beta"}`.

### Message Examples

#### Command: Open Gate

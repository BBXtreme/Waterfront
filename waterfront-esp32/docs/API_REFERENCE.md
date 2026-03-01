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
- `{"action": "update_config", "config": {...}}`: Updates the system's settings with new configuration.

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
- `waterfront/{location}/{code}/alert`: Error messages when something goes wrong.
- `waterfront/machine/{code}/status`: Overall machine status.
- `waterfront/slots/{id}/status`: Status of individual compartments (slots).
- `waterfront/locations/{code}/depositRelease`: Messages about releasing deposits.
- `waterfront/locations/{code}/returnConfirm`: Confirmations when items are returned.

### Message Examples

#### Command: Open Gate

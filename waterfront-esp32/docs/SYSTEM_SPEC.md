# WATERFRONT ESP32 Kayak Rental Controller - System Specification

## Overview
The WATERFRONT system is a smart controller for kayak rental stations. It uses an ESP32 microcontroller to manage gates, detect kayaks, handle payments, and communicate securely. It's designed for outdoor use with solar power and cellular backup.

## Hardware Requirements
- ESP32 board (like ESP32-DevKitC) - the brain of the system.
- Ultrasonic sensors (e.g., HC-SR04) - to detect if a kayak is present.
- Servo motors - to open and close gates.
- Limit switches - to know when gates are fully open or closed.
- LTE modem (optional) - for internet when WiFi isn't available.
- Solar panel and battery - for power in remote locations.
- ADC inputs - to measure battery and solar voltage.

## Software Architecture
### Main Parts
- **Main Loop**: Runs continuously, handles MQTT, OTA, LTE, and power checks.
- **MQTT Client**: Sends and receives messages securely (with encryption).
- **Config Loader**: Loads settings from a file on the ESP32.
- **Deposit Logic**: Manages rental payments and timers.
- **Return Sensor**: Measures distance to detect kayaks.
- **Gate Control**: Moves servos to open/close gates.
- **LTE Manager**: Handles cellular internet as backup.
- **Error Handler**: Logs problems and sends alerts.
- **OTA Handler**: Updates software remotely.
- **Provisioning**: Sets up WiFi without coding.

### Background Tasks
- Factory Reset Task: Listens for button press to reset.
- Overdue Check Task: Looks for rentals that are late.
- Debug Task: Sends status updates every minute.

## Configuration
Settings are in `/config.json` on the ESP32's storage. It includes sections for MQTT, location, WiFi, LTE, BLE, compartments, and system options.

### Example Config

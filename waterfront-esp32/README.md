# WATERFRONT ESP32 Kayak Rental Controller

## What is WATERFRONT?
WATERFRONT is a smart system for managing kayak rentals at waterfront locations. It uses a small computer called an ESP32 to automatically open and close gates for kayak compartments. It connects to the internet via WiFi or cellular to send updates and receive commands. The system is designed to work outdoors with solar power, making it reliable for remote spots.

## Key Features
- **Automatic Gate Control**: Opens and closes gates based on rental bookings.
- **Secure Internet Connection**: Uses MQTT (a messaging protocol) with encryption for safe communication.
- **Power Management**: Monitors battery and solar levels, enters sleep mode to save power.
- **Remote Updates**: Update the software wirelessly without touching the device.
- **Setup Without Coding**: Easy WiFi setup using Bluetooth or a web page.
- **Health Monitoring**: Sends regular reports on system status, like battery level and errors.
- **Error Alerts**: Notifies you if something goes wrong, like low power or connection issues.

## Who is This For?
- Beginners: If you're new to IoT (Internet of Things), this guide will walk you through everything.
- Developers: If you want to modify or extend the system, the code is open-source.
- Rental Business Owners: To automate your kayak rental process.

## Hardware You Need
- **ESP32 Board**: A microcontroller like ESP32-DevKitC (costs about $10-20).
- **Ultrasonic Sensors**: To detect if a kayak is present (e.g., HC-SR04, $5 each).
- **Servo Motors**: To move the gates (e.g., SG90, $5 each).
- **Limit Switches**: To know when gates are fully open or closed ($2 each).
- **LTE Modem** (optional): For internet when WiFi isn't available (e.g., SIM7600, $50).
- **Solar Panel and Battery**: For power in outdoor locations ($20-50).
- **Voltage Sensors**: To measure battery and solar levels (built into ESP32 ADC).

## Software Overview
The system runs on the ESP32 using Arduino-compatible code. It includes:
- **Main Program**: Handles daily operations like checking power and sending messages.
- **MQTT Client**: Sends and receives messages over the internet.
- **Config System**: Loads settings from a file on the ESP32.
- **Gate Controller**: Manages servo motors and sensors.
- **Power Manager**: Monitors and conserves energy.
- **Error Handler**: Logs problems and sends alerts.

## How It Works (Simple Flow)
1. **Setup**: Connect hardware, upload code, configure WiFi.
2. **Normal Operation**: System waits for rental bookings via MQTT.
3. **Rental Starts**: Receives booking, opens gate, starts timer.
4. **Rental Ends**: Detects kayak return, closes gate, releases deposit.
5. **Monitoring**: Sends health updates every minute, alerts on issues.
6. **Power Saving**: If low power, enters deep sleep until charged.

## Configuration
Settings are stored in a file called `config.json` on the ESP32. It includes details like WiFi passwords, MQTT server, and pin numbers for hardware.

### Example Config File

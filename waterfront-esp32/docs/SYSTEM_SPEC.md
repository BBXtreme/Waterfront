# WATERFRONT ESP32 Kayak Rental Controller - System Specification

## What is This System?
The WATERFRONT system is a smart device for kayak rental stations. It automatically manages gates for kayak storage compartments. Built on an ESP32 microcontroller, it connects to the internet to receive rental bookings and send status updates. It's made for outdoor use with solar power and cellular backup.

## Why Use It?
- **Automation**: No need for manual gate opening/closing.
- **Remote Monitoring**: Check status from anywhere via internet.
- **Reliability**: Handles power outages and network issues.
- **Scalability**: Supports multiple compartments.

## Hardware Components
- **ESP32 Microcontroller**: The brain, handles logic and communication.
- **Ultrasonic Sensors**: Measure distance to detect kayaks (sound waves bounce back).
- **Servo Motors**: Mechanical arms that open/close gates.
- **Limit Switches**: Sensors that detect if gates are fully open or closed.
- **LTE Modem** (optional): Cellular module for internet without WiFi.
- **Solar Panel and Battery**: Powers the system sustainably.
- **ADC Inputs**: Measure voltage levels for power monitoring.

## Software Architecture
The code is organized into modules, each handling a specific job.

### Main Modules
- **Main Loop**: Runs forever, checks power, handles MQTT, manages tasks.
- **MQTT Handler**: Sends/receives messages securely.
- **Config Loader**: Reads settings from a file.
- **Deposit Logic**: Manages rental payments and timers.
- **Return Sensor**: Detects kayak presence.
- **Gate Control**: Moves servos, checks switches.
- **LTE Manager**: Switches to cellular if WiFi fails.
- **Error Handler**: Logs errors, sends alerts.
- **OTA Updater**: Downloads new software wirelessly.
- **Provisioning**: Sets up WiFi easily.

### Background Tasks
- **Factory Reset Task**: Listens for button press to reset device.
- **Overdue Check Task**: Closes gates for late returns.
- **Debug Task**: Sends health updates every minute.

## Configuration Details
All settings are in `/config.json` on the ESP32's storage. This file is in JSON format (easy to read/edit).

### Sections Explained
- **mqtt**: Internet connection settings (server, login, security).
- **location**: Your location name and code.
- **wifiProvisioning**: Default WiFi for setup.
- **lte**: Cellular settings if using modem.
- **ble**: Bluetooth settings for setup.
- **compartments**: List of gates, with pin numbers for hardware.
- **system**: General settings like debug mode and power thresholds.
- **other**: Extra options like offline PIN time.         

### MQTT Section                                

This section configures the MQTT broker connection for secure communication.  

- **broker**: IP address or hostname of the MQTT server (e.g., "192.168.178.50" for local, or "mqtt.example.com" for remote).                  

- **port**: Port number (1883 for plain MQTT, 8883 for TLS).          

- **username**: Username for broker authentication (leave empty if not required).                                    

- **password**: Password for broker authentication (leave empty if not required).                                    

- **clientIdPrefix**: Prefix for the client ID (e.g., "waterfront" results in "waterfront-client").                              

- **useTLS**: Boolean to enable TLS encryption (true for secure connections).  

- **caCertPath**: Path to CA certificate file on LittleFS (e.g., "/ca.pem" for TLS).                                      

- **clientCertPath**: Path to client certificate file (optional, for mutual TLS).                                      

- **clientKeyPath**: Path to client private key file (optional, for mutual TLS).                                      

### Messages the System Sends (Responses and Updates)              

- `waterfront/{location}/{code}/response`: Replies to commands.         

- `waterfront/{location}/{code}/compartments/+/ack`: Acknowledgments, e.g., `{"compartmentId":1,"action":"gate_opened","timestamp":1234567890,"crc":987654321}`.                                       

- `waterfront/{location}/{code}/debug`: Health reports every 60 seconds, e.g., `{"uptime":3600,"heapFree":204800,"fwVersion":"0.9.2-beta","batteryPercent":85,"tasks":5,"reconnects":2}`.                            

- `waterfront/{location}/{code}/alert`: Alerts for issues, e.g., `{"alert":"low_power","batteryPercent":15,"solarVoltage":2.5,"timestamp":1234567890}`.                                      

- `waterfront/machine/{code}/status`: Machine status.              

- `waterfront/slots/{id}/status`: Compartment status.              

- `waterfront/locations/{code}/depositRelease`: Deposit releases.        

- `waterfront/locations/{code}/returnConfirm`: Return confirmations.      

- `waterfront/{location}/{code}/ota/status`: Update status.           

### Location Section                              

Defines the location of the rental station for topic organization.       

- **slug**: Short name for the location (e.g., "bremen" for city name).     

- **code**: Unique code for the station (e.g., "harbor-01" for first harbor in 

Bremen).                             

## Testing the Code                               

We use unit tests to ensure the code works. Run them with `pio test` in     

PlatformIO. Tests cover 90%+ of the code, checking things like config loading  

and MQTT messages.                                

### WiFi Provisioning Section                          

Settings for initial WiFi setup when no credentials are stored.         

- **fallbackSsid**: Default SSID for SoftAP mode (e.g., "WATERFRONT-DEFAULT"). 

- **fallbackPass**: Default password for SoftAP (e.g., "defaultpass123").        

### LTE Section                                 

Configuration for cellular fallback using LTE modem.              

- Follow the style: Use Arduino conventions.                  

- Add tests for new features.                          

- Update docs when changing things.                       

### LTE Section                                 

Configuration for cellular fallback using LTE modem.              

- **apn**: Access Point Name for the cellular network (e.g., "internet.t-mobile.de" for T-Mobile).                      

- **simPin**: PIN for the SIM card (leave empty if not required).        

- **rssiThreshold**: Minimum signal strength to switch to LTE (e.g., -70 dBm). 

- **dataUsageAlertLimitKb**: Alert threshold for data usage in KB (e.g., 100000 for 100MB).                                                         

### BLE Section                                 

+Settings for Bluetooth Low Energy provisioning.                 

- **serviceUuid**: UUID for the BLE service (e.g., "12345678-1234-1234-1234-123456789abc").                     

- **ssidCharUuid**: UUID for SSID characteristic.                

- **passCharUuid**: UUID for password characteristic.              

- **statusCharUuid**: UUID for status notifications.                                             

### Compartments Section                            

Array of objects defining each kayak compartment.                

- **number**: Compartment ID (1-based index).                  

- **servoPin**: GPIO pin for servo motor (e.g., 12 for ESP32 GPIO 12).     

- **limitOpenPin**: GPIO pin for open limit switch (e.g., 13).         

- **limitClosePin**: GPIO pin for close limit switch (e.g., 14).        

- **ultrasonicTriggerPin**: GPIO pin for ultrasonic trigger (e.g., 15).     

- **ultrasonicEchoPin**: GPIO pin for ultrasonic echo (e.g., 16).        

- **weightSensorPin**: GPIO pin for weight sensor (e.g., 17).                                              

### System Section                               

General system settings for operation.                     

- **maxCompartments**: Maximum number of compartments (e.g., 10).        

- **debugMode**: Boolean to enable debug telemetry (true for development).   

- **gracePeriodSec**: Grace period in seconds for rentals (e.g., 3600 for 1   

hour).                                      

- **batteryLowThresholdPercent**: Battery level to trigger alerts (e.g., 20%). 

+- **solarVoltageMin**: Minimum solar voltage to avoid deep sleep (e.g., 3.0V).                                  

### Other Section                                

Miscellaneous settings.                             

- **offlinePinTtlSec**: Time-to-live for offline PINs in seconds (e.g., 86400  

for 24 hours).                                  

- **depositHoldAmountFallback**: Fallback deposit amount (e.g., 50 units).                                       

## MQTT Communication                              

MQTT is like a chat room for devices. The system subscribes to topics to listen

for commands and publishes to topics to send updates.              

### Incoming Topics (What It Listens For)                    

- `waterfront/{location}/{code}/command`: General instructions.         

- `waterfront/{location}/{code}/config/update`: New settings.          

- `waterfront/{location}/{code}/ota/update`: Software update links.       

- `waterfront/{location}/{code}/booking/paid`: Rental starts.          

- `waterfront/{location}/{code}/compartments/+/command`: Gate commands.     

- `waterfront/{location}/{code}/compartments/+/status`: Status from other    

systems.                                                                   

### Outgoing Topics (What It Sends)                       

- `waterfront/{location}/{code}/response`: Command replies.           

- `waterfront/{location}/{code}/compartments/+/ack`: Action confirmations.   

- `waterfront/{location}/{code}/debug`: Health data every 60s.         

- `waterfront/{location}/{code}/alert`: Error notifications.          

- `waterfront/machine/{code}/status`: Device status.              

- `waterfront/slots/{id}/status`: Compartment updates.             

- `waterfront/locations/{code}/depositRelease`: Payment releases.        

- `waterfront/locations/{code}/returnConfirm`: Return handling.         

- `waterfront/{location}/{code}/ota/status`: Update results.                                              

## Remote Alerting and Health Telemetry                     

- **Debug Messages**: Sent every minute with system info (uptime, memory,    

battery, etc.).                                 

- **Alert Messages**: Sent on errors (e.g., low power, connection failures).  

- Helps monitor the system remotely without visiting the site.                                             

## Testing                                   

Unit tests check the code works correctly. Run with `pio test`. Covers most   

functions to ensure reliability.                                                         

## Power Management                               

- Checks battery and solar every minute.                                     

- If low, sends alert and enters deep sleep (low power mode).          

- LTE turns off if WiFi is active and idle.                                                         

## Error Handling                                

- Logs errors to serial and MQTT.                        

- Fatal errors restart the device.                       

- Alerts sent for critical issues.    

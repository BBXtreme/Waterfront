# WATERFRONT ESP32 Deployment Guide

## What is Deployment?
Deployment means setting up the WATERFRONT system at your kayak rental location. This guide is for beginners – follow each step carefully.

## Before You Start (Prerequisites)
1. **Hardware**: Gather ESP32 board, sensors, motors, solar panel, battery.
2. **Software**: Install PlatformIO (free tool for coding ESP32).
3. **MQTT Broker**: Set up a server for messages (e.g., Mosquitto on a computer).
4. **Certificates** (if using secure connection): Get TLS certs from a CA.
5. **Basic Knowledge**: Know how to connect wires and use a computer.

## Step 1: Prepare the Hardware
1. Connect ultrasonic sensors to ESP32 pins (e.g., Trig to GPIO 15, Echo to GPIO 16).
2. Attach servo motors to GPIO 12.
3. Connect limit switches to GPIO 13 and 14.
4. Wire solar panel and battery to ADC pins (GPIO 34 for battery, GPIO 35 for solar).
5. Optional: Connect LTE modem to serial pins.

## Step 2: Install the Software
1. Download the WATERFRONT code from GitHub.
2. Open it in PlatformIO.
3. Install libraries: Run `pio lib install` (it will download PubSubClient, ArduinoJson, etc.).
4. Select your ESP32 board in `platformio.ini`.
5. Build the code: Click "Build" in PlatformIO.

## Step 3: Configure the System
1. Create a file named `config.json` with your settings (copy from README.md example).
2. Change values:
   - `broker`: Your MQTT server IP.
   - `location.slug` and `code`: Your site name.
   - `wifiProvisioning.fallbackSsid` and `pass`: Default WiFi.
   - `compartments`: List your gates with correct pin numbers.
3. Upload config: Run `pio run --target uploadfs` in PlatformIO.
4. Upload certificates if using TLS: Put `ca.pem` in the data folder and upload.

## Step 4: Upload Code to ESP32
1. Connect ESP32 to computer via USB.
2. In PlatformIO, click "Upload".
3. Wait for "Upload successful".
4. Open serial monitor to see startup messages.

## Step 5: Set Up WiFi
If WiFi isn't configured:
- **Option 1: Bluetooth**: Use an app like nRF Connect to send SSID/password to the ESP32.
- **Option 2: Web Page**: Connect to "WATERFRONT-DEFAULT" WiFi, open 192.168.4.1 in browser, enter details.

## Step 6: Test the System
1. Power on the ESP32.
2. Check serial logs for errors.
3. Send a test MQTT command (e.g., open gate).
4. Verify gate moves and MQTT responses.
5. Run tests: `pio test` to check code.

## Step 7: Go Live
1. Mount hardware at your rental site.
2. Connect solar panel and battery.
3. Monitor MQTT topics for health updates.
4. Set up alerts to notify you of issues.

## Step 8: Maintenance
- Check battery levels via debug messages.
- Update firmware via OTA topic when new versions come out.
- Backup config.json regularly.
- Clean sensors and check wires.

## Troubleshooting During Setup
- **ESP32 not recognized**: Try different USB port or cable.
- **Build fails**: Check PlatformIO version and reinstall libraries.
- **WiFi fails**: Double-check SSID/password, move closer to router.
- **MQTT fails**: Verify broker IP/port, check firewall.

## Tips for Success
- Start small: Test one compartment first.
- Label wires: Note which pin connects to what.
- Keep backups: Save config.json copies.
- Learn MQTT: Use a client like MQTT Explorer to monitor messages.

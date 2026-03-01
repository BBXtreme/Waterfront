# WATERFRONT ESP32 Troubleshooting Guide

## Introduction
This guide helps you fix common problems with the WATERFRONT system. If something isn't working, check here first. The system logs messages to help diagnose issues.

## Common Issues and Fixes

### ESP32 Not Starting
**Symptoms**: No lights, no messages in serial monitor.
**Causes**: Power issues, bad firmware upload.
**Fixes**:
- Check the power supply (use 5V USB).
- Verify the firmware was uploaded correctly (run `pio run --target upload` again).
- Open the serial monitor in PlatformIO and look for error messages.

### WiFi Connection Fails
**Symptoms**: System can't connect to the internet.
**Causes**: Wrong WiFi name/password, weak signal.
**Fixes**:
- Double-check the SSID and password in config.json.
- Move the ESP32 closer to the router.
- Use the provisioning mode to re-enter WiFi details (see Deployment Guide).

### MQTT Connection Fails
**Symptoms**: System can't send/receive messages.
**Causes**: Wrong broker address, missing certificates, bad credentials.
**Fixes**:
- Check the broker IP and port in config.json.
- If using TLS, ensure certificates are uploaded and paths are correct.
- Verify username and password match the MQTT broker settings.

### Sensors Not Working
**Symptoms**: Gates don't open, no distance readings.
**Causes**: Wrong pin connections, sensor faults.
**Fixes**:
- Check the pin numbers in config.json match the hardware wiring.
- Ensure sensors are powered and connected properly.
- For ultrasonic sensors, make sure nothing blocks the sound waves.

### OTA (Over-The-Air) Updates Fail
**Symptoms**: Can't update software remotely.
**Causes**: Network issues, wrong password.
**Fixes**:
- Make sure the ESP32 is connected to WiFi.
- Check the OTA password in the code (default is "admin").
- Watch the MQTT progress messages for errors.

### Low Power Issues
**Symptoms**: System shuts down unexpectedly.
**Causes**: Battery/solar low, wrong thresholds.
**Fixes**:
- Check battery and solar levels in MQTT debug messages.
- Adjust thresholds in config.json (e.g., lower batteryLowThresholdPercent).
- Monitor how often it enters deep sleep.

### Factory Reset Doesn't Work
**Symptoms**: Holding the button doesn't reset.
**Causes**: Wrong pin, button not pressed long enough.
**Fixes**:
- Ensure you're holding GPIO 0 (usually the boot button).
- Hold for exactly 5 seconds.
- Check serial logs for confirmation.

## How to Check Logs
- Enable debug mode in config.json (`"debugMode": true`).
- Open the serial monitor in PlatformIO.
- Look for messages starting with tags like `[MAIN]`, `[MQTT]`, etc.
- MQTT debug topic also shows logs remotely.

## Recovery Steps
- **Factory Reset**: Hold GPIO 0 for 5 seconds to erase settings and restart.
- **Re-upload Firmware**: If the system is bricked, use `pio run --target upload` to reinstall.
- **Restore Config**: If config.json is lost, recreate it from the example in the Deployment Guide.

## Getting Help
- Check the serial logs for specific error codes.
- Search for error messages online.
- Open an issue on GitHub with logs and config details.
- Ensure all prerequisites (like MQTT broker) are set up correctly.

## Prevention Tips
- Always test after configuration changes.
- Keep backups of config.json.
- Monitor MQTT messages regularly.
- Update firmware when new versions are available.

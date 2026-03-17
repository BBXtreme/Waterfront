# WATERFRONT ESP32 Troubleshooting Guide

## Introduction
If something goes wrong with WATERFRONT, don't panic! This guide helps fix common problems. The system logs messages to help you diagnose. Enable debug mode in config.json for more info.

## How to Check for Problems
1. Open the serial monitor in PlatformIO (shows ESP32 messages).
2. Subscribe to MQTT topics like `waterfront/{location}/{code}/debug` and `waterfront/{location}/{code}/alert`.
3. Look for error codes or messages starting with `[ERROR]` or `[WARN]`.

## Common Issues and Fixes

### ESP32 Won't Start
**Symptoms**: No lights, no serial messages.
**Causes**: Bad power, faulty upload.
**Fixes**:
1. Check USB cable and power (5V).
2. Re-upload firmware: `pio run --target upload`.
3. Try a different ESP32 board.

### WiFi Won't Connect
**Symptoms**: Can't reach internet.
**Causes**: Wrong password, weak signal.
**Fixes**:
1. Verify SSID/password in config.json.
2. Move ESP32 closer to router.
3. Use provisioning to re-enter WiFi (see Deployment Guide).
4. Check serial for "WiFi connected" message.

### MQTT Won't Connect
**Symptoms**: No messages sent/received.
**Causes**: Wrong server, missing certs.
**Fixes**:
1. Check broker IP/port in config.json.
2. For TLS, ensure `ca.pem` is uploaded.
3. Verify username/password.
4. Look for "MQTT connected" in serial.

### Gates Don't Move
**Symptoms**: Servo motors don't turn.
**Causes**: Wrong pins, no power.
**Fixes**:
1. Check pin numbers in config.json match wires.
2. Ensure servos have power (5V).
3. Test with simple command via MQTT.
4. Check limit switches are connected.

### Sensors Not Detecting
**Symptoms**: No distance readings.
**Causes**: Wiring issues, blocked sensors.
**Fixes**:
1. Verify ultrasonic pins (Trig/Echo).
2. Make sure sensors face open space.
3. Check power to sensors.
4. Serial logs show distance values.

### OTA Updates Fail
**Symptoms**: Software won't update.
**Causes**: Network issues, wrong URL.
**Fixes**:
1. Ensure WiFi connected.
2. Check OTA password (default "admin").
3. Verify firmware URL is correct.
4. Watch MQTT for progress/errors.

### Low Power Shutdowns
**Symptoms**: Device turns off unexpectedly.
**Causes**: Battery/solar low.
**Fixes**:
1. Check debug messages for battery %.
2. Adjust thresholds in config.json.
3. Charge battery or check solar panel.
4. Monitor alerts for low power.

### Factory Reset Not Working
**Symptoms**: Holding button does nothing.
**Causes**: Wrong pin, short press.
**Fixes**:
1. Hold GPIO 0 for 5 full seconds.
2. Check serial for "Factory reset" message.
3. If not, re-upload firmware.

### No Debug/Alerts
**Symptoms**: Missing health messages.
**Causes**: Debug off, MQTT down.
**Fixes**:
1. Set `"debugMode": true` in config.json.
2. Check MQTT connection.
3. Subscribe to topics manually.

### CRC Errors
**Symptoms**: Messages ignored.
**Causes**: Data corruption.
**Fixes**:
1. Check network stability.
2. Ensure MQTT client is updated.
3. Restart device.

## Recovery Steps
1. **Soft Reset**: Power cycle the ESP32.
2. **Factory Reset**: Hold GPIO 0 for 5s.
3. **Re-upload**: If bricked, `pio run --target upload`.
4. **Restore Config**: Recreate config.json from backup.

## Getting More Help
- Check GitHub issues for similar problems.
- Post logs and config in forums.
- Ensure all hardware is compatible.

## Prevention
- Test after changes.
- Monitor regularly.
- Keep firmware updated.
- Use strong passwords.

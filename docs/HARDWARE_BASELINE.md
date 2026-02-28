# WATERFRONT Hardware Baseline – Kayak Vending Machine Controller

**Project phase**: Pre-development / firmware skeleton  
**Date**: 2026-02-28

**Development toolchain:** PlatformIO (VS Code) + ESP-IDF framework

## 1. ESP32 Board
- Model: [please fill: e.g. ESP32-DevKitC V4, ESP32-S3-DevKitC-1-N8R2, custom PCB?]
- PSRAM: yes / no
- Onboard LED: GPIO __
- Photo/link: [paste]

## 2. Lock Mechanism
- Type: [12 V solenoid + relay module / electronic cabinet lock / other]
- Relay module: [active-high / active-low, opto-isolated?]
- Voltage / current: __ V / __ A
- Control signal: 3.3 V logic

## 3. Return Sensor
- Type: [ultrasonic HC-SR04 / HC-SR04+ / magnetic reed / load cell + HX711 / IR beam / other]
- Voltage: 3.3 V / 5 V
- Additional components needed: [voltage divider / HX711 breakout / …]

## 4. LTE / Cellular Modem
- Model: [SIM7600G-H / SIM7000G / Quectel EC21 / other]
- Logic level: 1.8 V / 3.3 V
- UART baud: 115200 (default)
- PWRKEY / RESET control: yes / no → GPIO __
- Power supply: separate __ V / __ A recommended
- SIM card: test SIM with APN "internet.t-mobile.de" (common in DE)

## 5. Power Supply
- Main ESP32: USB / 5 V VIN / LiPo → 3.3 V regulator
- Solar / battery planned: yes / no → capacity __ Ah, voltage __ V
- Voltage monitoring: yes → divider to ADC pin __

## 6. Additional Peripherals (optional / future)
- Status LED / buzzer: yes / no
- OLED / display: yes / no → I2C pins reserved
- Multi-bay support: __ compartments → additional relays/sensors

## 7. Open Questions / To Decide
- Exact board model confirmation
- Sensor type final decision
- Relay module active state (high/low)
- Whether to control modem power via GPIO (deep sleep benefit)
- First test location / WiFi AP name & password for provisioning tests

Update this file whenever hardware changes occur.
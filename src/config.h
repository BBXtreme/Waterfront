#ifndef CONFIG_H
#define CONFIG_H

// WiFi and MQTT settings
#define WIFI_SSID "default_ssid"
#define WIFI_PASS "default_pass"
#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_SUBSCRIBE_TOPIC "/kayak/test/unlock"
#define MQTT_PUBLISH_TOPIC "/kayak/test/status"

// BLE settings (from nimble.h and bleprph.h)
#define BLE_DEVICE_NAME "KayakDevice"
#define MAX_DEVICES 1024
#define PAX_REPORT_INTERVAL_SEC (60*60)  // 1 hour
#define PAX_SCAN_DURATION_SEC 7
#define PAX_SCAN_INTERVAL_US (5*60*1000000ULL)  // 5 minutes

// GPIO pins (example, adjust as needed)
#define PIN_LED 2

// Other defines from original
#define AP_SSID "VMflow"
#define AP_PASS "12345678"

#endif // CONFIG_H

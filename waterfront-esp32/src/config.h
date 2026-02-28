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
#define MAX_DEVICES 100

// GPIO pins (example, adjust as needed)
#define PIN_LED 2

// Provisioning settings
#define PROVISIONING_BUTTON_PIN 4  // GPIO for button trigger
#define STATUS_LED_PIN 2           // LED for status/provisioning indication
#define RELAY_PIN 23               // For lock control
#define MACHINE_ID "bremen-harbor-01"  // Unique ID
#define MQTT_BROKER "your-mqtt-broker.com"  // Replace with actual
#define MQTT_UNLOCK_TOPIC "/kayak/" MACHINE_ID "/unlock"
#define MQTT_STATUS_TOPIC "/kayak/" MACHINE_ID "/status"
#define MQTT_PROVISION_TOPIC "/kayak/" MACHINE_ID "/provision/start"
#define RELAY_PULSE_DURATION_MS 1500
#define SENSOR_POLL_INTERVAL_MS 10000

#endif // CONFIG_H

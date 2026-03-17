// include/config.h – Waterfront ESP32 Configuration

#ifndef CONFIG_H
#define CONFIG_H

// === Machine / Identity ===
#define MACHINE_ID "kayak-bremen-01"            // unique per device – used in MQTT topics
#define MQTT_CLIENT_ID "waterfront-esp32-" MACHINE_ID

// === WiFi (initial – will be overwritten by provisioning) ===
#define FALLBACK_SSID     "YourTestWiFi"
#define FALLBACK_PASSWORD "test1234"

// === MQTT Broker (use your Mosquitto or test.mosquitto.org for dev) ===
#define MQTT_BROKER       "test.mosquitto.org"  // ← change to your broker IP/domain
#define MQTT_PORT         1883
#define MQTT_USERNAME     ""                    // anonymous for testing
#define MQTT_PASSWORD     ""

// === MQTT Topics (from TSD section 3 – exact match) ===
#define TOPIC_UNLOCK          "/station/" MACHINE_ID "/unlock"
#define TOPIC_STATUS          "/station/" MACHINE_ID "/status"
#define TOPIC_EVENT           "/station/" MACHINE_ID "/event"
#define TOPIC_RETURN_CONFIRM  "/station/" MACHINE_ID "/returnConfirm"
#define TOPIC_DEPOSIT_RELEASE "/station/" MACHINE_ID "/depositRelease"

// === Pin definitions (adjust to your hardware) ===
#define RELAY_LOCK_PIN    5                     // GPIO for solenoid/lock relay
#define STATUS_LED_PIN    2                     // onboard LED or external
#define BUTTON_PROV_PIN   0                     // GPIO0 – long press to start provisioning

// === Timing / Intervals ===
#define MQTT_RECONNECT_MS     5000
#define STATUS_PUBLISH_MS     30000             // send status every 30s

#endif // CONFIG_H
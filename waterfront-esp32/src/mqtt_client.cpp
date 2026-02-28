#include "mqtt_client.h"
#include "config_loader.h"
#include <ArduinoJson.h>

// Adapted from original mqtt_event_handler in mdb-slave-esp32s3.c
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Extern global config
extern GlobalConfig g_config;

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    ESP_LOGI("MQTT", "Message received: %s", message.c_str());

    // Parse JSON
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        ESP_LOGI("MQTT", "JSON parse failed");
        return;
    }
}

void mqtt_init() {
    // Use runtime config with fallback
    String broker = g_config.mqtt.broker.length() > 0 ? g_config.mqtt.broker : "192.168.178.50";
    int port = g_config.mqtt.port > 0 ? g_config.mqtt.port : 1883;
    mqttClient.setServer(broker.c_str(), port);
    mqttClient.setCallback(mqtt_callback);
    String clientId = (g_config.mqtt.clientIdPrefix.length() > 0 ? g_config.mqtt.clientIdPrefix : "waterfront") + "-client";
    if (mqttClient.connect(clientId.c_str())) {
        ESP_LOGI("MQTT", "Connected and subscribed");
    }
}

void mqtt_publish_status() {
    DynamicJsonDocument doc(256);
    doc["state"] = "idle";
    doc["battery"] = 92;
    doc["connType"] = "wifi";
    String payload;
    serializeJson(doc, payload);
    char topic[64];
    String locationCode = g_config.location.code.length() > 0 ? g_config.location.code : "harbor-01";
    snprintf(topic, sizeof(topic), "waterfront/machine/%s/status", locationCode.c_str());
    mqttClient.publish(topic, payload.c_str(), true);  // Retained publish for machine status
}

// New function for publishing slot-specific status (e.g., booked, gateState)
void mqtt_publish_slot_status(int slotId, const char* jsonPayload) {
    char topic[64];
    String locationCode = g_config.location.code.length() > 0 ? g_config.location.code : "harbor-01";
    snprintf(topic, sizeof(topic), "waterfront/slots/%d/status", slotId);
    mqttClient.publish(topic, jsonPayload, true);  // Retained publish for slot status
}

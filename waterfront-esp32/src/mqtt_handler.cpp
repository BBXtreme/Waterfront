/**
 * @file mqtt_handler.cpp
 * @brief Handles all MQTT subscriptions and callbacks for multi-compartment Waterfront booking system.
 * @author BBXtreme + Grok
 * @date 2026-02-28
 * @note Uses retained topics for real-time compartment status sync with CRC32 checksums.
 */

// mqtt_handler.cpp - MQTT client handling for WATERFRONT ESP32
// This file manages MQTT connections, subscriptions, publishing, and callbacks.
// It integrates with the main loop for reliable IoT communication.
// Uses esp_mqtt_client for ESP-IDF compatibility.
// Updated for retained MQTT topics: subscribes to compartment status/command, publishes acks.
// Handles real-time compartment booking sync and gate control.
// Added CRC32 checksums for payload integrity.
// Added OTA update support.

#include "mqtt_handler.h"
#include "mqtt_topics.h"
#include "gate_control.h"
#include "config_loader.h"
#include "deposit_logic.h"
#include <mqtt_client.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPUpdate.h>
#include <Preferences.h>

// CRC32 implementation (simple polynomial)
uint32_t computeCRC32(const char* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint8_t)data[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    return ~crc;
}

// External references
extern esp_mqtt_client_handle_t mqttClient;
extern bool mqttConnected;

// Last-will topic and message for disconnect handling
#define MQTT_LAST_WILL_TOPIC "waterfront/esp32/disconnect"
#define MQTT_LAST_WILL_MESSAGE "{\"status\":\"disconnected\"}"

// Initialize MQTT handler
esp_err_t mqtt_init() {
    vPortEnterCritical(&g_configMutex);
    char broker[64];
    strcpy(broker, g_config.mqtt.broker);
    int port = g_config.mqtt.port;
    vPortExitCritical(&g_configMutex);
    // Build URI
    char uri[128];
    snprintf(uri, sizeof(uri), "mqtt://%s:%d", broker, port);
    esp_mqtt_client_config_t mqtt_config = {};
    mqtt_config.uri = uri;
    mqtt_config.lwt.topic = MQTT_LAST_WILL_TOPIC;
    mqtt_config.lwt.msg = MQTT_LAST_WILL_MESSAGE;
    mqtt_config.lwt.qos = 1;
    mqtt_config.lwt.retain = true;
    vPortEnterCritical(&g_configMutex);
    char clientIdPrefix[32];
    strcpy(clientIdPrefix, g_config.mqtt.clientIdPrefix);
    vPortExitCritical(&g_configMutex);
    char clientId[64];
    if (strlen(clientIdPrefix) > 0) {
        snprintf(clientId, sizeof(clientId), "%s-client", clientIdPrefix);
    } else {
        strcpy(clientId, "waterfront-client");
    }
    mqtt_config.client_id = clientId;
    mqttClient = esp_mqtt_client_init(&mqtt_config);
    if (!mqttClient) return ESP_FAIL;
    esp_mqtt_client_register_event(mqttClient, ESP_EVENT_ANY_ID, event_handler, NULL);
    esp_err_t err = esp_mqtt_client_start(mqttClient);
    if (err != ESP_OK) return err;
    gate_init();  // Initialize gate control
    ESP_LOGI("MQTT", "Initialized with broker %s:%d", broker, port);
    return ESP_OK;
}

// Attempt to connect to MQTT broker with last-will
bool mqtt_connect() {
    if (mqttConnected) return true;
    ESP_LOGI("MQTT", "Connecting...");
    // Since esp_mqtt_client handles connection, just check if started
    return mqttClient != nullptr;
}

// Subscribe to compartment-specific MQTT topics using wildcards
void mqtt_subscribe() {
    char topic[96];
    // Subscribe to status wildcard for this location
    vPortEnterCritical(&g_configMutex);
    char locationSlug[32];
    strcpy(locationSlug, g_config.location.slug);
    char locationCode[32];
    strcpy(locationCode, g_config.location.code);
    vPortExitCritical(&g_configMutex);
    snprintf(topic, sizeof(topic), "waterfront/%s/%s/compartments/+/status", locationSlug, locationCode);
    int msg_id = esp_mqtt_client_subscribe(mqttClient, topic, 1);
    if (msg_id < 0) {
        ESP_LOGE("MQTT", "Failed to subscribe to %s", topic);
    } else {
        ESP_LOGI("MQTT", "Subscribed to %s, msg_id=%d", topic, msg_id);
    }
    // Subscribe to command wildcard for this location
    snprintf(topic, sizeof(topic), "waterfront/%s/%s/compartments/+/command", locationSlug, locationCode);
    msg_id = esp_mqtt_client_subscribe(mqttClient, topic, 1);
    if (msg_id < 0) {
        ESP_LOGE("MQTT", "Failed to subscribe to %s", topic);
    } else {
        ESP_LOGI("MQTT", "Subscribed to %s, msg_id=%d", topic, msg_id);
    }
    // Subscribe to config update
    char configTopic[96];
    snprintf(configTopic, sizeof(configTopic), "waterfront/%s/%s/config/update", locationSlug, locationCode);
    msg_id = esp_mqtt_client_subscribe(mqttClient, configTopic, 1);
    if (msg_id < 0) {
        ESP_LOGE("MQTT", "Failed to subscribe to %s", configTopic);
    } else {
        ESP_LOGI("MQTT", "Subscribed to %s, msg_id=%d", configTopic, msg_id);
    }
    // Subscribe to OTA update
    char otaTopic[96];
    snprintf(otaTopic, sizeof(otaTopic), "waterfront/%s/%s/ota/update", locationSlug, locationCode);
    msg_id = esp_mqtt_client_subscribe(mqttClient, otaTopic, 1);
    if (msg_id < 0) {
        ESP_LOGE("MQTT", "Failed to subscribe to %s", otaTopic);
    } else {
        ESP_LOGI("MQTT", "Subscribed to %s, msg_id=%d", otaTopic, msg_id);
    }
    // Subscribe to booking paid
    char bookingPaidTopic[96];
    snprintf(bookingPaidTopic, sizeof(bookingPaidTopic), "waterfront/%s/%s/booking/paid", locationSlug, locationCode);
    msg_id = esp_mqtt_client_subscribe(mqttClient, bookingPaidTopic, 1);
    if (msg_id < 0) {
        ESP_LOGE("MQTT", "Failed to subscribe to %s", bookingPaidTopic);
    } else {
        ESP_LOGI("MQTT", "Subscribed to %s, msg_id=%d", bookingPaidTopic, msg_id);
    }
}

// Publish retained status for a compartment with CRC32
void mqtt_publish_retained_status(int compartmentId, const char* jsonPayload) {
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, jsonPayload);
    if (error) {
        ESP_LOGE("MQTT", "Failed to parse JSON for status publish: %s", error.c_str());
        return;  // Discard bad message
    }
    uint32_t crc = computeCRC32(jsonPayload, strlen(jsonPayload));
    doc["crc"] = crc;
    doc["firmwareVersion"] = FW_VERSION;  // Include firmware version
    String updatedPayload;
    serializeJson(doc, updatedPayload);
    char topic[96];
    vPortEnterCritical(&g_configMutex);
    char locationSlug[32];
    strcpy(locationSlug, g_config.location.slug);
    char locationCode[32];
    strcpy(locationCode, g_config.location.code);
    vPortExitCritical(&g_configMutex);
    snprintf(topic, sizeof(topic), "waterfront/%s/%s/compartments/%d/status", locationSlug, locationCode, compartmentId);
    int msg_id = esp_mqtt_client_publish(mqttClient, topic, updatedPayload.c_str(), 0, 1, 1);  // QoS 1, retain
    if (msg_id < 0) {
        ESP_LOGE("MQTT", "Failed to publish retained status");
    } else {
        ESP_LOGI("MQTT", "Published retained status to %s: %s, msg_id=%d", topic, updatedPayload.c_str(), msg_id);
    }
}

// Publish acknowledgment for a compartment action with CRC32
void mqtt_publish_ack(int compartmentId, const char* action) {
    char payload[128];
    snprintf(payload, sizeof(payload), "{\"compartmentId\":%d,\"action\":\"%s\",\"timestamp\":%lu}", compartmentId, action, millis());
    uint32_t crc = computeCRC32(payload, strlen(payload));
    char fullPayload[256];
    snprintf(fullPayload, sizeof(fullPayload), "%s,\"crc\":%lu}", payload, crc);
    fullPayload[strlen(fullPayload) - 1] = '}';  // Fix JSON
    char topic[96];
    vPortEnterCritical(&g_configMutex);
    char locationSlug[32];
    strcpy(locationSlug, g_config.location.slug);
    char locationCode[32];
    strcpy(locationCode, g_config.location.code);
    vPortExitCritical(&g_configMutex);
    snprintf(topic, sizeof(topic), "waterfront/%s/%s/compartments/%d/ack", locationSlug, locationCode, compartmentId);
    int msg_id = esp_mqtt_client_publish(mqttClient, topic, fullPayload, 0, 1, 1);  // QoS 1, retain
    if (msg_id < 0) {
        ESP_LOGE("MQTT", "Failed to publish ack");
    } else {
        ESP_LOGI("MQTT", "Published ack to %s: %s, msg_id=%d", topic, fullPayload, msg_id);
    }
}

// MQTT event handler for processing incoming messages with CRC validation
void event_handler(void* args, esp_event_base_t base, int32_t event_id, void* data) {
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) data;
    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            mqttConnected = true;
            ESP_LOGI("MQTT", "Connected");
            mqtt_subscribe();  // Subscribe on connect
            break;
        case MQTT_EVENT_DISCONNECTED:
            mqttConnected = false;
            ESP_LOGW("MQTT", "Disconnected");
            break;
        case MQTT_EVENT_DATA: {
            String msg;
            for (int i = 0; i < event->data_len; i++) msg += (char)event->data[i];
            String topic = String(event->topic, event->topic_len);
            ESP_LOGI("MQTT", "Received on %s: %s at %lu", topic.c_str(), msg.c_str(), millis());

            // Check if config update
            vPortEnterCritical(&g_configMutex);
            char locationSlug[32];
            strcpy(locationSlug, g_config.location.slug);
            char locationCode[32];
            strcpy(locationCode, g_config.location.code);
            vPortExitCritical(&g_configMutex);
            char configTopic[96];
            snprintf(configTopic, sizeof(configTopic), "waterfront/%s/%s/config/update", locationSlug, locationCode);
            if (topic == configTopic) {
                ESP_LOGI("MQTT", "Config update received");
                if (updateConfigFromJson(msg.c_str())) {
                    ESP_LOGI("MQTT", "Config updated, restarting ESP");
                    delay(5000);
                    esp_restart();
                } else {
                    ESP_LOGE("MQTT", "Failed to update config");
                }
                return;
            }

            // Check if OTA update
            char otaTopic[96];
            snprintf(otaTopic, sizeof(otaTopic), "waterfront/%s/%s/ota/update", locationSlug, locationCode);
            if (topic == otaTopic) {
                ESP_LOGI("MQTT", "OTA update received: %s", msg.c_str());
                // Save current version to NVS before update
                Preferences prefs;
                prefs.begin("ota", false);
                prefs.putString("prev_version", FW_VERSION);
                prefs.putBool("attempted", true);
                prefs.end();
                // Assume msg is the URL
                t_httpUpdate_return ret = httpUpdate.update(msg);
                DynamicJsonDocument otaDoc(256);
                otaDoc["firmwareVersion"] = FW_VERSION;
                switch (ret) {
                    case HTTP_UPDATE_FAILED:
                        ESP_LOGE("OTA", "Update failed: %s", httpUpdate.getLastErrorString().c_str());
                        otaDoc["otaResult"] = "failed";
                        otaDoc["error"] = httpUpdate.getLastErrorString().c_str();
                        break;
                    case HTTP_UPDATE_NO_UPDATES:
                        ESP_LOGI("OTA", "No updates available");
                        otaDoc["otaResult"] = "no_updates";
                        break;
                    case HTTP_UPDATE_OK:
                        ESP_LOGI("OTA", "Update successful, restarting");
                        otaDoc["otaResult"] = "success";
                        break;
                    default:
                        ESP_LOGE("OTA", "Unknown update result: %d", ret);
                        otaDoc["otaResult"] = "unknown";
                        break;
                }
                String otaPayload;
                serializeJson(otaDoc, otaPayload);
                char otaStatusTopic[96];
                snprintf(otaStatusTopic, sizeof(otaStatusTopic), "waterfront/%s/%s/ota/status", locationSlug, locationCode);
                int msg_id = esp_mqtt_client_publish(mqttClient, otaStatusTopic, otaPayload.c_str(), 0, 1, 1);
                if (msg_id >= 0) {
                    ESP_LOGI("OTA", "Published OTA status, msg_id=%d", msg_id);
                }
                if (ret == HTTP_UPDATE_OK) {
                    ESP.restart();
                }
                return;
            }

            // Check if booking paid
            char bookingPaidTopic[96];
            snprintf(bookingPaidTopic, sizeof(bookingPaidTopic), "waterfront/%s/%s/booking/paid", locationSlug, locationCode);
            if (topic == bookingPaidTopic) {
                ESP_LOGI("MQTT", "Booking paid received: %s", msg.c_str());
                DynamicJsonDocument doc(256);
                DeserializationError error = deserializeJson(doc, msg);
                if (error) {
                    ESP_LOGE("MQTT", "Failed to parse booking paid payload");
                    return;
                }
                String bookingId = doc["bookingId"];
                int compartmentId = doc["compartmentId"];
                unsigned long durationSec = doc["durationSec"];
                // Start rental timer
                vPortEnterCritical(&g_configMutex);
                unsigned long gracePeriod = g_config.system.gracePeriodSec;
                vPortExitCritical(&g_configMutex);
                startRental(compartmentId, durationSec + gracePeriod);
                ESP_LOGI("MQTT", "Started rental for booking %s, compartment %d, duration %lu sec", bookingId.c_str(), compartmentId, durationSec);
                return;
            }

            // Validate CRC for other messages
            DynamicJsonDocument doc(512);
            DeserializationError error = deserializeJson(doc, msg);
            if (error) {
                ESP_LOGE("MQTT", "JSON parse failed for status: %s", error.c_str());
                return;  // Discard bad message
            }
            uint32_t receivedCrc = doc["crc"];
            doc.remove("crc");  // Remove for recompute
            String cleanMsg;
            serializeJson(doc, cleanMsg);
            uint32_t computedCrc = computeCRC32(cleanMsg.c_str(), cleanMsg.length());
            if (receivedCrc != computedCrc) {
                ESP_LOGE("MQTT", "CRC mismatch: received %lu, computed %lu", receivedCrc, computedCrc);
                return;  // Discard bad message
            }

            // Log full JSON for debugging
            ESP_LOGV("MQTT", "Full JSON after CRC validation: %s", cleanMsg.c_str());

            // Parse topic to extract compartmentId using sscanf for efficiency
            int compartmentId = -1;
            char locationSlugBuf[32], locationCodeBuf[32];
            if (sscanf(topic.c_str(), "waterfront/%31[^/]/%31[^/]/compartments/%d/status", locationSlugBuf, locationCodeBuf, &compartmentId) == 3) {
                // Handle status message (retained sync)
                bool booked = doc["booked"];
                const char* gateState = doc["gateState"];
                ESP_LOGI("MQTT", "Compartment %d: booked=%d, gateState=%s", compartmentId, booked, gateState);
                if (booked && strcmp(gateState, "locked") == 0) {
                    openCompartmentGate(compartmentId);
                    mqtt_publish_ack(compartmentId, "gate_opened");
                }
            } else if (sscanf(topic.c_str(), "waterfront/%31[^/]/%31[^/]/compartments/%d/command", locationSlugBuf, locationCodeBuf, &compartmentId) == 3) {
                // Handle command message
                ESP_LOGI("MQTT", "Compartment %d command: %s", compartmentId, msg.c_str());
                if (msg == "open_gate") {
                    openCompartmentGate(compartmentId);
                    mqtt_publish_ack(compartmentId, "gate_opened");
                } else if (msg == "close_gate") {
                    closeCompartmentGate(compartmentId);
                    mqtt_publish_ack(compartmentId, "gate_closed");
                } else if (msg == "query_state") {
                    const char* state = getCompartmentGateState(compartmentId);
                    char payload[64];
                    snprintf(payload, sizeof(payload), "{\"compartmentId\":%d,\"gateState\":\"%s\"}", compartmentId, state);
                    uint32_t crc = computeCRC32(payload, strlen(payload));
                    char fullPayload[128];
                    snprintf(fullPayload, sizeof(fullPayload), "%s,\"crc\":%lu}", payload, crc);
                    fullPayload[strlen(fullPayload) - 1] = '}';  // Fix JSON
                    char topic[96];
                    vPortEnterCritical(&g_configMutex);
                    char locSlug[32];
                    strcpy(locSlug, g_config.location.slug);
                    char locCode[32];
                    strcpy(locCode, g_config.location.code);
                    vPortExitCritical(&g_configMutex);
                    snprintf(topic, sizeof(topic), "waterfront/%s/%s/compartments/%d/ack", locSlug, locCode, compartmentId);
                    int msg_id = esp_mqtt_client_publish(mqttClient, topic, fullPayload, 0, 1, 1);
                    if (msg_id >= 0) {
                        ESP_LOGI("MQTT", "Published query ack, msg_id=%d", msg_id);
                    }
                }
            }
            break;
        }
        case MQTT_EVENT_ERROR:
            ESP_LOGE("MQTT", "MQTT error occurred");
            break;
        default:
            ESP_LOGD("MQTT", "Unhandled MQTT event: %d", event_id);
            break;
    }
}

// Maintain MQTT connection and process messages
void mqtt_loop() {
    // Since esp_mqtt_client is event-driven, no loop needed
    // Just ensure connection
    if (!mqttConnected) {
        mqtt_connect();
    }
}

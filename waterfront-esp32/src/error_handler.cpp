/**
 * @file error_handler.cpp
 * @brief Implementation of global error handling for WATERFRONT ESP32.
 * @author BBXtreme + Grok
 * @date 2026-02-28
 * @note Logs fatal errors, publishes alerts to MQTT, and restarts ESP32.
 */

#include "error_handler.h"
#include <esp_log.h>
#include <esp_system.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config_loader.h"

// Extern MQTT client for publishing alerts
extern PubSubClient mqttClient;

// fatal_error function: Handles unrecoverable errors.
// Logs the error, publishes to MQTT if connected, and restarts the ESP32.
void fatal_error(const char* msg, esp_err_t code) {
    // Log error with code name for debugging
    ESP_LOGE("FATAL", "%s: %s (0x%x)", msg, esp_err_to_name(code), code);

    // Publish fatal error to debug topic if MQTT connected and debug mode enabled
    if (mqttClient.connected() && g_config.debugMode) {
        DynamicJsonDocument doc(256);
        doc["error"] = msg;
        doc["code"] = esp_err_to_name(code);  // Human-readable error code
        doc["timestamp"] = millis();
        String payload;
        serializeJson(doc, payload);
        char topic[96];
        snprintf(topic, sizeof(topic), "waterfront/%s/%s/debug", g_config.location.slug.c_str(), g_config.location.code.c_str());
        mqttClient.publish(topic, payload.c_str(), false);  // Not retained for debug
        ESP_LOGI("FATAL", "Published fatal error to debug topic");
    }

    // Publish to alert topic for critical alerts (always attempted)
    DynamicJsonDocument alertDoc(256);
    alertDoc["error"] = msg;
    alertDoc["code"] = code;  // Numeric code for alerts
    alertDoc["timestamp"] = millis();
    String alertPayload;
    serializeJson(alertDoc, alertPayload);
    char alertTopic[96];
    snprintf(alertTopic, sizeof(alertTopic), "waterfront/%s/%s/alert", g_config.location.slug.c_str(), g_config.location.code.c_str());
    mqttClient.publish(alertTopic, alertPayload.c_str(), false);  // Not retained
    ESP_LOGI("FATAL", "Published alert to %s", alertTopic);

    // Delay to allow publish to complete before restart
    delay(5000);
    esp_restart();  // Restart ESP32 to recover
}

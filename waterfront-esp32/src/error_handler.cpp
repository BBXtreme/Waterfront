/**
 * @file error_handler.cpp
 * @brief Global error handling implementation for WATERFRONT ESP32.
 * @author BBXtreme + Grok
 * @date 2026-02-28
 * @note Logs fatal errors and restarts ESP32.
 */

#include "error_handler.h"
#include <esp_log.h>
#include <esp_system.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config_loader.h"

// Extern MQTT client
extern PubSubClient mqttClient;

void fatal_error(const char* msg, esp_err_t code) {
    ESP_LOGE("FATAL", "%s: %s (0x%x)", msg, esp_err_to_name(code), code);

    // Publish fatal error to debug topic if MQTT connected
    if (mqttClient.connected() && g_config.debugMode) {
        DynamicJsonDocument doc(256);
        doc["error"] = msg;
        doc["code"] = esp_err_to_name(code);
        doc["timestamp"] = millis();
        String payload;
        serializeJson(doc, payload);
        char topic[96];
        snprintf(topic, sizeof(topic), "waterfront/%s/%s/debug", g_config.location.slug.c_str(), g_config.location.code.c_str());
        mqttClient.publish(topic, payload.c_str(), false);
        ESP_LOGI("FATAL", "Published fatal error to debug topic");
    }

    // Publish to alert topic
    DynamicJsonDocument alertDoc(256);
    alertDoc["error"] = msg;
    alertDoc["code"] = code;
    String alertPayload;
    serializeJson(alertDoc, alertPayload);
    char alertTopic[96];
    snprintf(alertTopic, sizeof(alertTopic), "waterfront/%s/%s/alert", g_config.location.slug.c_str(), g_config.location.code.c_str());
    mqttClient.publish(alertTopic, alertPayload.c_str(), false);
    ESP_LOGI("FATAL", "Published alert to %s", alertTopic);

    delay(5000);
    esp_restart();
}

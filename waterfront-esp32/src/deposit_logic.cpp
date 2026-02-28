// deposit_logic.cpp - Deposit hold/release logic for WATERFRONT
// This file manages deposit states based on booking and sensor events.
// It handles hold on take, release on timely return, and auto-charge on overdue.
// Integrates with MQTT for real-time sync and gate control.

#include "deposit_logic.h"
#include "config_loader.h"
#include <PubSubClient.h>

// External MQTT client
extern PubSubClient mqttClient;

// Current booking state
static bool deposit_held = false;
static unsigned long rental_start_time = 0;
static unsigned long rental_duration_ms = 0;

// Initialize deposit logic
void deposit_init() {
    deposit_held = false;
    ESP_LOGI("DEPOSIT", "Initialized");
}

// On kayak taken: hold deposit, start timer
void deposit_on_take() {
    deposit_held = true;
    rental_start_time = millis();
    rental_duration_ms = g_config.system.gracePeriodSec * 1000;
    ESP_LOGI("DEPOSIT", "Deposit held, rental started");
}

// On kayak returned: check timing, release deposit if on time
void deposit_on_return(PubSubClient* client) {
    if (!deposit_held) return;
    unsigned long elapsed = millis() - rental_start_time;
    if (elapsed <= rental_duration_ms) {
        // On-time return: release deposit
        deposit_held = false;
        ESP_LOGI("DEPOSIT", "Deposit released (on-time return)");
        // Publish deposit release event
        char topic[64];
        char payload[128];
        snprintf(topic, sizeof(topic), "waterfront/locations/%s/depositRelease", g_config.location.code.c_str());
        snprintf(payload, sizeof(payload), "{\"bookingId\":\"current\",\"release\":true}");
        client->publish(topic, payload);
    } else {
        // Overdue: keep held, publish for admin action
        ESP_LOGW("DEPOSIT", "Overdue return, deposit held");
        // TODO: Publish overdue event or charge via MQTT
    }
}

// Check for overdue (call periodically)
void deposit_check_overdue(PubSubClient* client) {
    if (!deposit_held) return;
    unsigned long elapsed = millis() - rental_start_time;
    if (elapsed > rental_duration_ms + 3600000) {  // 1 hour grace
        // Auto-lock or alert
        ESP_LOGW("DEPOSIT", "Rental overdue, triggering gate control");
        // Publish returnConfirm for auto-lock
        char topic[64];
        char payload[128];
        snprintf(topic, sizeof(topic), "waterfront/locations/%s/returnConfirm", g_config.location.code.c_str());
        snprintf(payload, sizeof(payload), "{\"bookingId\":\"current\",\"action\":\"autoLock\"}");
        client->publish(topic, payload);
    }
}

// Get deposit status
bool deposit_is_held() {
    return deposit_held;
}

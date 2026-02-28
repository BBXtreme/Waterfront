// gate_control.cpp - Gate control logic for servo and limit switches
// This file handles physical gate operations for kayak compartments.
// Uses servo for gate movement and limit switches for feedback.
// Integrates with MQTT for real-time control.
// Non-blocking: uses millis() state machine for servo control.
// Added state machine with timeouts for stuck gates.

#include "gate_control.h"
#include "config_loader.h"
#include <Servo.h>

// Pin definitions for compartments (adjust in config.h if needed)
#define SERVO_PIN_1 12  // GPIO for servo on compartment 1
#define LIMIT_OPEN_PIN_1 13  // Limit switch for open position
#define LIMIT_CLOSE_PIN_1 14  // Limit switch for closed position
// Add more for additional compartments as needed

// Servo objects (array for MAX_COMPARTMENTS)
#define MAX_COMPARTMENTS 10
Servo servos[MAX_COMPARTMENTS];

// State machine for each compartment
enum CompartmentState { IDLE, OPENING, CLOSING, OPEN, CLOSED, ERROR };
CompartmentState compartmentStates[MAX_COMPARTMENTS] = {CLOSED};
unsigned long compartmentStartTimes[MAX_COMPARTMENTS] = {0};
#define GATE_MOVE_TIMEOUT_MS 10000  // 10 seconds timeout for stuck gates
#define RETRY_ATTEMPTS 3  // Max retries before error

int retryCounts[MAX_COMPARTMENTS] = {0};

/**
 * @brief Initializes gate control for all compartments.
 * @note Sets up pins and servos for active compartments.
 */
void gate_init() {
    // Initialize pins and servos for active compartments
    for (int i = 0; i < g_config.compartments.size(); i++) {
        if (i < MAX_COMPARTMENTS) {
            pinMode(g_config.compartments[i].limitOpenPin, INPUT_PULLUP);
            pinMode(g_config.compartments[i].limitClosePin, INPUT_PULLUP);
            servos[i].attach(g_config.compartments[i].servoPin);
            compartmentStates[i] = CLOSED;  // Start closed
        }
    }
    ESP_LOGI("GATE", "Initialized for %d compartments", g_config.compartments.size());
}

/**
 * @brief Opens gate for compartment (non-blocking).
 * @param compartmentId The compartment ID to open.
 * @note Sets state to OPENING and starts servo movement.
 */
void openCompartmentGate(int compartmentId) {
    if (compartmentId < 1 || compartmentId > g_config.compartments.size()) return;
    int idx = compartmentId - 1;
    if (compartmentStates[idx] == CLOSED || compartmentStates[idx] == ERROR) {
        compartmentStates[idx] = OPENING;
        compartmentStartTimes[idx] = millis();
        servos[idx].write(90);  // Example angle for open; adjust
        retryCounts[idx] = 0;
        ESP_LOGI("GATE", "Starting open for compartment %d", compartmentId);
    }
}

/**
 * @brief Closes gate for compartment (non-blocking).
 * @param compartmentId The compartment ID to close.
 * @note Sets state to CLOSING and starts servo movement.
 */
void closeCompartmentGate(int compartmentId) {
    if (compartmentId < 1 || compartmentId > g_config.compartments.size()) return;
    int idx = compartmentId - 1;
    if (compartmentStates[idx] == OPEN || compartmentStates[idx] == ERROR) {
        compartmentStates[idx] = CLOSING;
        compartmentStartTimes[idx] = millis();
        servos[idx].write(0);  // Example angle for close; adjust
        retryCounts[idx] = 0;
        ESP_LOGI("GATE", "Starting close for compartment %d", compartmentId);
    }
}

/**
 * @brief Gets gate state for compartment.
 * @param compartmentId The compartment ID.
 * @return "open", "closed", or "unknown".
 * @note Checks limit switches for compartment 1.
 */
const char* getCompartmentGateState(int compartmentId) {
    if (compartmentId < 1 || compartmentId > g_config.compartments.size()) return "unknown";
    int idx = compartmentId - 1;
    if (digitalRead(g_config.compartments[idx].limitOpenPin)) return "open";
    if (digitalRead(g_config.compartments[idx].limitClosePin)) return "closed";
    return "unknown";
}

/**
 * @brief Task to handle servo movement and timeouts (call from main loop).
 * @note Checks if movement duration has passed, updates state, handles retries on timeout.
 */
void gate_task() {
    unsigned long now = millis();
    for (int i = 0; i < g_config.compartments.size(); i++) {
        if (compartmentStates[i] == OPENING || compartmentStates[i] == CLOSING) {
            if (now - compartmentStartTimes[i] > GATE_MOVE_TIMEOUT_MS) {
                // Timeout: retry or error
                if (retryCounts[i] < RETRY_ATTEMPTS) {
                    retryCounts[i]++;
                    ESP_LOGW("GATE", "Compartment %d stuck, retrying (attempt %d)", i + 1, retryCounts[i]);
                    if (compartmentStates[i] == OPENING) {
                        openCompartmentGate(i + 1);
                    } else {
                        closeCompartmentGate(i + 1);
                    }
                } else {
                    compartmentStates[i] = ERROR;
                    ESP_LOGE("GATE", "Compartment %d error: stuck after %d retries", i + 1, RETRY_ATTEMPTS);
                    // TODO: Publish MQTT alert or trigger admin notification
                }
            } else {
                // Check limit switches for completion
                if (compartmentStates[i] == OPENING) {
                    if (digitalRead(g_config.compartments[i].limitOpenPin)) {
                        compartmentStates[i] = OPEN;
                        ESP_LOGI("GATE", "Compartment %d gate opened", i + 1);
                    }
                } else if (compartmentStates[i] == CLOSING) {
                    if (digitalRead(g_config.compartments[i].limitClosePin)) {
                        compartmentStates[i] = CLOSED;
                        ESP_LOGI("GATE", "Compartment %d gate closed", i + 1);
                    }
                }
            }
        }
    }
}

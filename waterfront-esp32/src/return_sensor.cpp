// return_sensor.cpp - Ultrasonic sensor for kayak presence detection
// This file handles HC-SR04 ultrasonic sensor to detect if a kayak is present in the bay.
// It measures distance and determines presence based on threshold.
// Used for real-time slot booking sync and gate control.

#include "return_sensor.h"
#include "config_loader.h"
#include <Arduino.h>

// Threshold for kayak presence (distance in cm; adjust based on setup)
#define PRESENCE_THRESHOLD_CM 50.0f

// Initialize sensor pins
void sensor_init() {
    // Use pins from config for the first compartment (assuming single sensor for now)
    if (!g_config.compartments.empty()) {
        pinMode(g_config.compartments[0].ultrasonicTriggerPin, OUTPUT);
        pinMode(g_config.compartments[0].ultrasonicEchoPin, INPUT);
        ESP_LOGI("SENSOR", "Ultrasonic sensor initialized on TRIG %d, ECHO %d", g_config.compartments[0].ultrasonicTriggerPin, g_config.compartments[0].ultrasonicEchoPin);
    }
}

// Get distance in cm
float sensor_get_distance() {
    if (g_config.compartments.empty()) return -1.0f;
    int trigPin = g_config.compartments[0].ultrasonicTriggerPin;
    int echoPin = g_config.compartments[0].ultrasonicEchoPin;

    // Trigger pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure echo
    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2;  // Speed of sound: 343 m/s
    return distance;
}

// Check if kayak is present (distance < threshold)
bool sensor_is_kayak_present() {
    float dist = sensor_get_distance();
    return dist < PRESENCE_THRESHOLD_CM && dist > 0;  // >0 to filter invalid readings
}

// return_sensor.cpp - Ultrasonic sensor for kayak presence detection
// This file handles HC-SR04 ultrasonic sensor to detect if a kayak is present in the bay.
// It measures distance and determines presence based on threshold.
// Used for real-time slot booking sync and gate control (auto-lock on return).

#include "return_sensor.h"
#include "config.h"
#include <Arduino.h>

// Pin definitions (adjust in config.h if needed)
#define TRIG_PIN 5
#define ECHO_PIN 18

// Threshold for kayak presence (distance in cm; adjust based on setup)
#define PRESENCE_THRESHOLD_CM 50.0f

// Initialize sensor pins
void sensor_init() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    ESP_LOGI("SENSOR", "Ultrasonic sensor initialized on TRIG %d, ECHO %d", TRIG_PIN, ECHO_PIN);
}

// Get distance in cm
float sensor_get_distance() {
    // Trigger pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure echo
    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2;  // Speed of sound: 343 m/s
    return distance;
}

// Check if kayak is present (distance < threshold)
bool sensor_is_kayak_present() {
    float dist = sensor_get_distance();
    return dist < PRESENCE_THRESHOLD_CM && dist > 0;  // >0 to filter invalid readings
}

// return_sensor.cpp - Ultrasonic sensor for kayak presence detection
// This file handles HC-SR04 ultrasonic sensor to detect if a kayak is present in the bay.
// It measures distance and determines presence based on threshold.
// Used for real-time slot booking sync and gate control.

#include "return_sensor.h"
#include "config_loader.h"
#include <driver/gpio.h>
#include <esp_timer.h>

// Threshold for kayak presence (distance in cm; adjust based on setup)
#define PRESENCE_THRESHOLD_CM 50.0f

// Initialize sensor pins
void sensor_init() {
    // Use pins from config for the first compartment (assuming single sensor for now)
    if (g_config.compartmentCount > 0) {
        gpio_set_direction((gpio_num_t)g_config.compartments[0].ultrasonicTriggerPin, GPIO_MODE_OUTPUT);
        gpio_set_direction((gpio_num_t)g_config.compartments[0].ultrasonicEchoPin, GPIO_MODE_INPUT);
        ESP_LOGI("SENSOR", "Ultrasonic sensor initialized on TRIG %d, ECHO %d", g_config.compartments[0].ultrasonicTriggerPin, g_config.compartments[0].ultrasonicEchoPin);
    }
}

// Get distance in cm
float sensor_get_distance() {
    if (g_config.compartmentCount == 0) return -1.0f;
    gpio_num_t trigPin = (gpio_num_t)g_config.compartments[0].ultrasonicTriggerPin;
    gpio_num_t echoPin = (gpio_num_t)g_config.compartments[0].ultrasonicEchoPin;

    // Trigger pulse
    gpio_set_level(trigPin, 0);
    ets_delay_us(2);
    gpio_set_level(trigPin, 1);
    ets_delay_us(10);
    gpio_set_level(trigPin, 0);

    // Measure echo
    uint64_t start = esp_timer_get_time();
    while (gpio_get_level(echoPin) == 0) {
        if (esp_timer_get_time() - start > 1000000) return -1.0f;  // Timeout 1ms
    }
    start = esp_timer_get_time();
    while (gpio_get_level(echoPin) == 1) {
        if (esp_timer_get_time() - start > 1000000) return -1.0f;  // Timeout 1ms
    }
    uint64_t duration = esp_timer_get_time() - start;
    float distance = duration * 0.034 / 2 / 1000;  // Speed of sound: 343 m/s, convert to cm
    return distance;
}

// Check if kayak is present (distance < threshold)
bool sensor_is_kayak_present() {
    float dist = sensor_get_distance();
    return dist < PRESENCE_THRESHOLD_CM && dist > 0;  // >0 to filter invalid readings
}

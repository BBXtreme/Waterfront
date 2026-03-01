 /**
 * @file main.cpp
 * @brief Main entry point for WATERFRONT ESP32 Kayak Rental Controller.
 * @author BBXtreme + Grok
 * @date 2026-02-28
 * @note Initializes LittleFS, loads config, WiFi/MQTT/LTE, gates/sensors, tasks for non-blocking operation.
 * @note Includes solar voltage monitoring and deep sleep for low-power solar operation.
 */

#include "config
// lte_manager.cpp - LTE cellular fallback management using TinyGSM
// This file manages the LTE modem for cellular connectivity as a WiFi fallback.
// It uses TinyGSM library to control the modem, connect to GPRS, and provide TCP client for MQTT.
// Power management is included to conserve battery in solar-powered setups.

#include "lte_manager.h"
#include "config_loader.h"
#include <TinyGsmClient.h>
#include <HardwareSerial.h>
#include <PubSubClient.h>

// External references
extern PubSubClient mqttClient;

// TinyGSM setup
HardwareSerial SerialAT(2);
TinyGsm modem(SerialAT);
TinyGsmClient lteClient(modem);

// Initialize LTE modem
void lte_init() {
    SerialAT.begin(115200, SERIAL_8N1, 16, 17);  // Use config pins if needed
    pinMode(25, OUTPUT);  // PWRKEY
    digitalWrite(25, LOW);
    ESP_LOGI("LTE", "Initialized (powered off)");
}

// Power up and connect modem
void lte_power_up() {
    digitalWrite(25, HIGH);
    delay(1000);
    digitalWrite(25, LOW);
    delay(10000);
    modem.restart();
    modem.gprsConnect(g_config.lte.apn.c_str(), "", "");
    ESP_LOGI("LTE", "Powered up and connected to GPRS");
}

// Power down modem
void lte_power_down() {
    modem.poweroff();
    digitalWrite(25, LOW);
    ESP_LOGI("LTE", "Powered down");
}

// Switch MQTT to LTE
void lte_switch_to_lte() {
    lte_power_up();
    mqttClient.setClient(lteClient);
    if (mqttClient.connect((g_config.mqtt.clientIdPrefix + "-lte").c_str())) {
        ESP_LOGI("LTE", "MQTT switched to LTE");
    } else {
        ESP_LOGE("LTE", "MQTT connect failed over LTE");
    }
}

// Switch MQTT back to WiFi
void lte_switch_to_wifi() {
    mqttClient.setClient(wifiClient);
    ESP_LOGI("LTE", "MQTT switched back to WiFi");
    lte_power_down();
}

// Check LTE signal
int lte_get_signal() {
    return modem.getSignalQuality();
}

// Is LTE connected
bool lte_is_connected() {
    return modem.isGprsConnected();
}

// Read solar voltage (ADC)
float readSolarVoltage() {
    // Assume ADC pin from config (e.g., GPIO 35)
    int adcValue = analogRead(35);  // Example pin
    // Convert to voltage (calibrate based on divider)
    float voltage = (adcValue / 4095.0) * 3.3 * (10.0 / 3.3);  // Example divider 10k/3.3k
    return voltage;
}

// Check if LTE should be disabled due to low solar
bool shouldDisableLTE() {
    float solarVoltage = readSolarVoltage();
    return solarVoltage < g_config.system.solarVoltageMin;
}

// Power down modem if WiFi connected and idle, or low solar
void lte_power_management() {
    if (WiFi.status() == WL_CONNECTED && !mqttClient.connected() && shouldDisableLTE()) {
        lte_power_down();
        ESP_LOGI("LTE", "Powered down due to WiFi connected and low solar voltage");
    }
}

// lte_manager.cpp - LTE cellular fallback implementation using TinyGSM
// This file manages the LTE modem for cellular connectivity as a WiFi fallback.
// It uses TinyGSM library to control the modem, connect to GPRS, and provide TCP client for MQTT.
// Power management is included to conserve battery in solar-powered setups.

#include "lte_manager.h"      // Header for LTE functions
#include "config.h"           // Configuration constants
#include <PubSubClient.h>     // MQTT client

// External references from main.cpp
extern PubSubClient mqttClient;  // MQTT client instance
extern ConnectivityState currentConn;  // Current connectivity state
extern void reconnectMQTT();     // Function to reconnect MQTT

// TinyGSM setup for SIM7600/SIM7000 modem
HardwareSerial SerialAT(2);      // Use UART2 for modem communication
TinyGsm modem(SerialAT);         // Modem instance
TinyGsmClient lteClient(modem);  // TCP client over LTE

// Initialize LTE modem
// Sets up serial communication and configures PWRKEY pin.
void initLTE() {
    // Initialize UART2 with modem baud rate
    SerialAT.begin(LTE_MODEM_BAUD, SERIAL_8N1, LTE_MODEM_RX_PIN, LTE_MODEM_TX_PIN);
    // Configure PWRKEY pin as output (for power control)
    pinMode(LTE_MODEM_PWRKEY_PIN, OUTPUT);
    digitalWrite(LTE_MODEM_PWRKEY_PIN, LOW);  // Start powered off
    Serial.println("LTE modem initialized (powered off)");
}

// Power up the modem and establish connection
// Sends PWRKEY pulse, waits for boot, restarts modem, and connects to GPRS.
void powerUpModem() {
    // PWRKEY pulse to turn on modem
    digitalWrite(LTE_MODEM_PWRKEY_PIN, HIGH);
    delay(1000);  // Hold for 1 second
    digitalWrite(LTE_MODEM_PWRKEY_PIN, LOW);
    Serial.println("LTE modem powered up");
    // Wait for modem to boot (may take time)
    delay(10000);
    // Restart modem to ensure clean state
    modem.restart();
    // Connect to GPRS with APN (adjust for SIM provider)
    modem.gprsConnect(LTE_APN, "", "");  // Username/password empty for many providers
    Serial.println("LTE GPRS connected");
}

// Power down the modem
// Uses TinyGSM poweroff and sets PWRKEY low.
void powerDownModem() {
    modem.poweroff();  // Graceful shutdown
    digitalWrite(LTE_MODEM_PWRKEY_PIN, LOW);  // Ensure off
    Serial.println("LTE modem powered down");
}

// Switch to LTE connectivity
// Powers up modem, changes MQTT client to LTE, updates state, and reconnects.
void switchToLTE() {
    powerUpModem();  // Power and connect modem
    // Switch MQTT client to use LTE TCP client
    mqttClient.setClient(lteClient);
    currentConn = LTE;  // Update connectivity state
    Serial.println("Switched to LTE connectivity");
    // Reconnect MQTT over LTE
    reconnectMQTT();
}

// Switch back to WiFi connectivity
// Changes MQTT client to WiFi, updates state, and reconnects.
void switchToWiFi() {
    // Switch MQTT client back to WiFi TCP client
    mqttClient.setClient(wifiClient);
    currentConn = WIFI;  // Update connectivity state
    Serial.println("Switched to WiFi connectivity");
    // Reconnect MQTT over WiFi
    reconnectMQTT();
}

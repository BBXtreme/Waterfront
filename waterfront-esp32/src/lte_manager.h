// lte_manager.h - Header file for LTE cellular fallback management
// This header declares functions for initializing and controlling the LTE modem.
// It provides an interface for switching between WiFi and LTE connectivity.
// Used in conjunction with lte_manager.cpp for cellular operations.

#ifndef LTE_MANAGER_H
#define LTE_MANAGER_H

#include <TinyGsmClient.h>    // TinyGSM client for MQTT over LTE
#include <HardwareSerial.h>   // Serial for modem communication

// External TinyGSM instances for global access
extern TinyGsm modem;         // Modem object
extern TinyGsmClient lteClient;  // LTE TCP client
extern HardwareSerial SerialAT;  // Serial port for modem

// Initialize LTE modem (power off initially)
void initLTE();

// Power up and configure the modem
void powerUpModem();

// Power down the modem to save energy
void powerDownModem();

// Switch MQTT client to LTE and reconnect
void switchToLTE();

// Switch MQTT client back to WiFi and reconnect
void switchToWiFi();

#endif // LTE_MANAGER_H

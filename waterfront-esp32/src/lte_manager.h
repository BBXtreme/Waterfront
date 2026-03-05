#ifndef LTE_MANAGER_H
#define LTE_MANAGER_H

#include <esp_modem.h>        // ESP-IDF modem for cellular
#include <driver/uart.h>      // ESP-IDF UART for modem communication

// External ESP-IDF modem instances for global access
extern esp_modem_dce_t* dce;  // Modem DCE object
extern esp_netif_t* esp_netif; // Netif for LTE
extern uart_port_t uart_num; // UART port for modem

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

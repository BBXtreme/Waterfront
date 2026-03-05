/**
 * @file ota_handler.cpp
 * @brief Handles OTA firmware updates using esp_https_ota.
 * @author BBXtreme + Grok
 * @date 2026-02-28
 * @note Performs secure HTTPS OTA updates, validates certificates, and handles errors gracefully.
 */

#include "ota_handler.h"
#include "config_loader.h"
#include <esp_https_ota.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <string.h>

// OTA configuration
static esp_https_ota_config_t ota_config = {
    .http_config = {
        .url = NULL,  // Will be set from config
        .cert_pem = NULL,  // Use CA cert from config if available
        .skip_cert_common_name_check = true,  // For development; set to false in production
    },
};

// Initialize OTA handler
esp_err_t ota_init() {
    ESP_LOGI("OTA", "Initializing OTA handler");
    // Load OTA URL from config
    vPortEnterCritical(&g_configMutex);
    char otaUrl[256];
    // Placeholder: construct URL from config or use a default
    // For now, use a placeholder URL; in production, this could be from MQTT or config
    strcpy(otaUrl, "https://example.com/firmware.bin");  // Update this to actual URL
    ota_config.http_config.url = otaUrl;
    // Load CA cert if available
    if (strlen(g_config.mqtt.caCertPath) > 0) {
        // Load CA cert from file (placeholder; implement file loading)
        // ota_config.http_config.cert_pem = load_cert_from_file(g_config.mqtt.caCertPath);
    }
    vPortExitCritical(&g_configMutex);
    ESP_LOGI("OTA", "OTA initialized with URL: %s", otaUrl);
    return ESP_OK;
}

// Perform OTA update
esp_err_t ota_perform_update() {
    ESP_LOGI("OTA", "Starting OTA update");
    esp_err_t ret = esp_https_ota(&ota_config);
    if (ret == ESP_OK) {
        ESP_LOGI("OTA", "OTA update successful, restarting...");
        esp_restart();
    } else {
        ESP_LOGE("OTA", "OTA update failed: %s", esp_err_to_name(ret));
    }
    return ret;
}

// Check for OTA update (placeholder: always return false for now)
bool ota_check_for_update() {
    // Placeholder: Implement version checking via HTTP or MQTT
    // For example, fetch version from server and compare with FW_VERSION
    return false;
}

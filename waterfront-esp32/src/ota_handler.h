#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <esp_err.h>

/**
 * @brief Initializes OTA handler.
 * @return ESP_OK on success, error code otherwise.
 */
esp_err_t ota_init();

/**
 * @brief Performs OTA update from the configured URL.
 * @return ESP_OK on success, error code otherwise.
 */
esp_err_t ota_perform_update();

/**
 * @brief Checks if an OTA update is available (placeholder for future implementation).
 * @return true if update available, false otherwise.
 */
bool ota_check_for_update();

#endif // OTA_HANDLER_H

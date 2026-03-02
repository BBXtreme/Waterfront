#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <mqtt_client.h>

extern esp_mqtt_client_handle_t mqttClient;
extern bool mqttConnected;
extern unsigned long lastMqttActivity;

/**
 * @brief Initializes the MQTT client with server and callback.
 */
esp_err_t mqtt_init();

/**
 * @brief MQTT event handler for handling incoming messages and connection events.
 * @param event The MQTT event structure.
 */
void event_handler(void* args, esp_event_base_t base, int32_t event_id, void* data);

/**
 * @brief Publishes status update to MQTT broker.
 */
void mqtt_publish_status();

/**
 * @brief Publishes slot-specific status.
 * @param slotId The slot ID.
 * @param jsonPayload The JSON payload string.
 */
void mqtt_publish_slot_status(int slotId, const char* jsonPayload);

/**
 * @brief Returns the MQTT reconnect count for telemetry.
 */
int getMqttReconnectCount();

#endif // MQTT_CLIENT_H

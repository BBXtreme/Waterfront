void lte_power_management() {
    // Power down LTE if WiFi is connected and MQTT has been idle for >5 minutes
    if (WiFi.status() == WL_CONNECTED && mqttClient.connected() && (millis() - lastMqttActivity > 300000)) {  // 5 min idle
        lte_power_down();
        ESP_LOGI("LTE", "Powered down due to WiFi connected and idle > 5 min");
    }
    // Power down LTE if solar voltage is low to conserve power
    if (shouldDisableLTE()) {
        lte_power_down();
        ESP_LOGI("LTE", "Powered down due to low solar voltage");
    }
}

// Adapted from app_main in mdb-slave-esp32s3.c, integrated for MQTT base
#include "config.h"
#include "wifi_manager.h"
#include "mqtt_client.h"
#include "nimble.h"
#include "webui_server.h"
#include <esp_timer.h>

void ble_event_handler(char* payload) {
    // Handle BLE events, simplified
    ESP_LOGI("BLE", "Event: %s", payload);
}

void ble_pax_handler(int count) {
    ESP_LOGI("BLE", "Pax count: %d", count);
}

void setup() {
    Serial.begin(115200);
    wifi_init();
    ble_init(BLE_DEVICE_NAME, ble_event_handler, ble_pax_handler);
    start_softap();
    start_dns_server();
    start_rest_server();
    mqtt_init();

    // Timer for pax scan
    esp_timer_create_args_t pax_timer_args = {
        .callback = [](void*) { ble_scan_start(PAX_SCAN_DURATION_SEC); },
        .name = "pax_scan"
    };
    esp_timer_handle_t pax_timer;
    esp_timer_create(&pax_timer_args, &pax_timer);
    esp_timer_start_periodic(pax_timer, PAX_SCAN_INTERVAL_US);

    // Timer for pax report
    esp_timer_create_args_t report_timer_args = {
        .callback = [](void*) { pax_report_timer(); },
        .name = "pax_report"
    };
    esp_timer_handle_t report_timer;
    esp_timer_create(&report_timer_args, &report_timer);
    esp_timer_start_periodic(report_timer, PAX_REPORT_INTERVAL_SEC * 1000000ULL);
}

void loop() {
    mqttClient.loop();
    dnsServer.processNextRequest();
    server.handleClient();
    static unsigned long lastPublish = 0;
    if (millis() - lastPublish > 60000) {
        mqtt_publish_status();
        lastPublish = millis();
    }
}

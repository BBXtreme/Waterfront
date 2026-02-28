// Adapted from original webui_server.c, ported to Arduino
#include "webui_server.h"
#include "config.h"
#include <WiFi.h>
#include <ArduinoJson.h>

WebServer server(80);
DNSServer dnsServer;

void handleRoot() {
    String html = "<h1>WiFi Setup</h1><form action='/set' method='POST'>SSID: <input name='ssid'><br>Pass: <input name='pass'><br>MQTT: <input name='mqtt'><br><input type='submit'></form>";
    server.send(200, "text/html", html);
}

void handleSet() {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");
    String mqtt = server.arg("mqtt");
    // Save to config (simplified)
    server.send(200, "text/plain", "WiFi set, rebooting...");
    ESP.restart();
}

void handleSystemInfo() {
    DynamicJsonDocument doc(256);
    doc["version"] = "1.0";
    doc["cores"] = 2;
    doc["model"] = "ESP32";
    doc["wifi_ssid"] = WIFI_SSID;
    doc["wifi_password"] = WIFI_PASS;
    doc["mqtt_server"] = MQTT_SERVER;
    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

void handleCaptive() {
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
}

void start_softap() {
    WiFi.softAP(AP_SSID, AP_PASS);
    ESP_LOGI("SoftAP", "Started");
}

void start_rest_server() {
    server.on("/", handleRoot);
    server.on("/set", HTTP_POST, handleSet);
    server.on("/api/v1/system/info", HTTP_GET, handleSystemInfo);
    server.on("/generate_204", handleCaptive);
    server.on("/hotspot-detect.html", handleCaptive);
    server.begin();
}

void stop_rest_server() {
    server.stop();
}

void start_dns_server() {
    dnsServer.start(53, "*", WiFi.softAPIP());
}

void stop_dns_server() {
    dnsServer.stop();
}

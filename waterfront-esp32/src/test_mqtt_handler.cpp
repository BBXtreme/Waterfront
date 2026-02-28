// test_mqtt_handler.cpp - Unit tests for mqtt_handler.cpp using Catch2
// This file contains tests for MQTT handling functions, focusing on retained status payloads and ack publishing.
// Uses Catch2 for test framework (header-only, include via PlatformIO lib_deps).
// Mocks PubSubClient to simulate MQTT interactions without network.
// Run tests with PlatformIO: pio test

#define CATCH_CONFIG_MAIN  // Catch2 main entry point
#include "catch2/catch.hpp"  // Catch2 header (add to lib_deps in platformio.ini: "catchorg/Catch2@^3.4.0")

// Include headers under test
#include "mqtt_handler.h"
#include "mqtt_topics.h"
#include "config.h"
#include <PubSubClient.h>
#include <Client.h>

// Mock Client for PubSubClient
class MockClient : public Client {
public:
    virtual int connect(IPAddress ip, uint16_t port) override { return 1; }
    virtual int connect(const char *host, uint16_t port) override { return 1; }
    virtual size_t write(uint8_t) override { return 1; }
    virtual size_t write(const uint8_t *buf, size_t size) override { return size; }
    virtual int available() override { return 0; }
    virtual int read() override { return -1; }
    virtual int read(uint8_t *buf, size_t size) override { return -1; }
    virtual int peek() override { return -1; }
    virtual void flush() override {}
    virtual void stop() override {}
    virtual uint8_t connected() override { return 1; }
    virtual operator bool() override { return true; }
};

// Mock PubSubClient inheriting properly
class MockPubSubClient : public PubSubClient {
public:
    MockPubSubClient() : PubSubClient(mockClient) {}
    bool connect(const char* id, const char* willTopic, uint8_t willQos, bool willRetain, const char* willMessage) override {
        lastPublishedTopic = "";
        lastPublishedPayload = "";
        lastPublishedRetained = false;
        publishCount = 0;
        return true;
    }
    bool subscribe(const char* topic, uint8_t qos) override { return true; }
    bool publish(const char* topic, const char* payload, bool retained) override {
        lastPublishedTopic = topic;
        lastPublishedPayload = payload;
        lastPublishedRetained = retained;
        publishCount++;
        return true;
    }
    void setCallback(void (*callback)(char*, byte*, unsigned int)) override { this->callback = callback; }
    void loop() override {}

    // Test inspection
    std::string lastPublishedTopic;
    std::string lastPublishedPayload;
    bool lastPublishedRetained = false;
    int publishCount = 0;

private:
    void (*callback)(char*, byte*, unsigned int) = nullptr;
    MockClient mockClient;
};

// Global mock instance
MockPubSubClient mockMqttClient;

// Override extern mqttClient for tests
PubSubClient& mqttClient = mockMqttClient;

// Test retained status payload simulation
TEST_CASE("Simulate Retained Status Payload and Verify Ack Publish", "[mqtt]") {
    // Reset mock state
    mockMqttClient.publishCount = 0;
    mockMqttClient.lastPublishedTopic = "";
    mockMqttClient.lastPublishedPayload = "";
    mockMqttClient.lastPublishedRetained = false;

    // Simulate incoming retained status message for compartment 1
    const char* topic = "waterfront/locations/bremen-harbor-01/compartments/1/status";
    const char* payload = "{\"booked\":true,\"gateState\":\"locked\",\"crc\":1234567890}";
    unsigned int length = strlen(payload);

    // Call callback directly (simulates MQTT message receipt)
    mqtt_callback(const_cast<char*>(topic), reinterpret_cast<byte*>(const_cast<char*>(payload)), length);

    // Verify: Ack should be published for gate_opened
    REQUIRE(mockMqttClient.publishCount == 1);
    REQUIRE(mockMqttClient.lastPublishedTopic == "waterfront/locations/bremen-harbor-01/compartments/1/ack");
    REQUIRE(mockMqttClient.lastPublishedRetained == false);  // Acks not retained
    // Check payload contains action
    REQUIRE(mockMqttClient.lastPublishedPayload.find("\"action\":\"gate_opened\"") != std::string::npos);
}

// Test command payload simulation
TEST_CASE("Simulate Command Payload and Verify Ack", "[mqtt]") {
    // Reset mock state
    mockMqttClient.publishCount = 0;
    mockMqttClient.lastPublishedTopic = "";
    mockMqttClient.lastPublishedPayload = "";
    mockMqttClient.lastPublishedRetained = false;

    // Simulate command message
    const char* topic = "waterfront/locations/bremen-harbor-01/compartments/1/command";
    const char* payload = "\"open_gate\"";  // Simple string command
    unsigned int length = strlen(payload);

    // Call callback
    mqtt_callback(const_cast<char*>(topic), reinterpret_cast<byte*>(const_cast<char*>(payload)), length);

    // Verify ack published
    REQUIRE(mockMqttClient.publishCount == 1);
    REQUIRE(mockMqttClient.lastPublishedTopic == "waterfront/locations/bremen-harbor-01/compartments/1/ack");
    REQUIRE(mockMqttClient.lastPublishedPayload.find("\"action\":\"gate_opened\"") != std::string::npos);
}

// Test CRC validation failure
TEST_CASE("CRC Validation Failure", "[mqtt]") {
    // Reset mock state
    mockMqttClient.publishCount = 0;

    const char* topic = "waterfront/locations/bremen-harbor-01/compartments/1/status";
    const char* payload = "{\"booked\":true,\"crc\":999999999}";  // Invalid CRC
    unsigned int length = strlen(payload);

    // Call callback
    mqtt_callback(const_cast<char*>(topic), reinterpret_cast<byte*>(const_cast<char*>(payload)), length);

    // Verify no publish (CRC mismatch)
    REQUIRE(mockMqttClient.publishCount == 0);
}

// main.cpp - Main entry point for WATERFRONT ESP32 Kayak Rental Controller
// This file initializes the ESP32, sets up WiFi, MQTT, and provisioning, and runs the main loop.
// It integrates all modules: WiFi/MQTT connectivity, provisioning, LTE fallback, and hardware control.
// The code uses FreeRTOS tasks for concurrent execution (e.g., MQTT handling in background).
// Framework: Arduino (for simplicity, but with ESP-IDF components via PlatformIO).

#include <Arduino.h>          // Arduino core library for ESP32
#include <WiFi.h>             // WiFi library for STA/AP modes
#include <PubSubClient.h>     // MQTT client library
#include <ArduinoJson.h>      // JSON parsing for MQTT payloads
#include "config.h"           // Global configuration constants
#include "provisioning.h"     // Provisioning functions (BLE/SoftAP)
#include "webui_server.h"    // Web server for SoftAP provisioning
#include "lte_manager.h"      // LTE cellular fallback manager

// Global MQTT client instance
// Uses WiFiClient by default; switches to LTE client when needed.
PubSubClient mqttClient(wifiClient);
WiFiClient wifiClient;        // WiFi TCP client for MQTT
bool provisioningActive = false;  // Flag to indicate if provisioning is running

// Connectivity state enumeration
// Tracks current network mode: WiFi, LTE, or offline.
enum ConnectivityState { WIFI, LTE, OFFLINE };
ConnectivityState currentConn = WIFI;  // Current connectivity state

// Function prototypes for functions defined later in the file
// These allow calling functions before their definitions.
void setupWiFi();             // Setup WiFi in STA mode
void setupMQTT();             // Initialize MQTT client and connection
void mqttCallback(char* topic, byte* payload, unsigned int length);  // Handle incoming MQTT messages
void reconnectMQTT();         // Reconnect to MQTT broker if disconnected
void publishStatus();         // Publish current status via MQTT
void checkProvisioningButton();  // Check for provisioning button press
void checkConnectivity();     // Monitor connectivity and handle failover

// FreeRTOS task handles for background tasks
TaskHandle_t mqttTaskHandle;  // Handle for MQTT task

// Setup function: runs once at boot
// Initializes serial, pins, LTE modem, WiFi/MQTT, and starts tasks.
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  Serial.println("WATERFRONT-ESP32: Hello from Waterfront ESP32 Kayak Controller!");
  Serial.println("Build successful - base ESP-IDF setup ready for MQTT integration.");

  // Configure GPIO pins for hardware control
  pinMode(PROVISIONING_BUTTON_PIN, INPUT_PULLUP);  // Button with internal pull-up
  pinMode(STATUS_LED_PIN, OUTPUT);                 // LED for status indication
  pinMode(RELAY_PIN, OUTPUT);                      // Relay for lock control
  digitalWrite(RELAY_PIN, LOW);                    // Start with lock closed

  // Initialize LTE modem (powered off initially for power saving)
  initLTE();

  // Check if WiFi is already provisioned; if not, start provisioning
  if (WiFi.SSID() == "") {  // No saved SSID indicates not provisioned
    startBLEProvisioning();  // Prefer BLE provisioning
  } else {
    setupWiFi();  // Connect to saved WiFi
    setupMQTT();  // Setup MQTT connection
  }

  // Create FreeRTOS task for MQTT handling (runs in background)
  xTaskCreate(mqttTask, "MQTT", 4096, NULL, 1, &mqttTaskHandle);
}

// Main loop: runs repeatedly after setup
// Handles provisioning, connectivity checks, and other periodic tasks.
void loop() {
  // Check provisioning button for long-press trigger
  checkProvisioningButton();

  // If provisioning is active, handle SoftAP server requests
  if (provisioningActive) {
    server.handleClient();  // Process incoming HTTP requests for SoftAP
  }

  // Monitor connectivity and handle WiFi to LTE failover
  checkConnectivity();

  // Small delay to yield CPU to other tasks
  delay(100);
}

// Setup WiFi in Station mode
// Connects to the saved SSID/password from provisioning.
void setupWiFi() {
  WiFi.mode(WIFI_STA);  // Set to Station mode (connect to AP)
  WiFi.begin();         // Use saved credentials
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  currentConn = WIFI;  // Update connectivity state
}

// Setup MQTT client
// Configures the MQTT broker and callback for incoming messages.
void setupMQTT() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);  // Set broker address and port
  mqttClient.setCallback(mqttCallback);          // Set callback for received messages
}

// MQTT callback function: handles incoming messages
// Parses JSON payloads for unlock commands or provisioning triggers.
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Convert payload to string for parsing
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  // Handle unlock commands
  if (strcmp(topic, MQTT_UNLOCK_TOPIC) == 0) {
    // Parse JSON payload
    DynamicJsonDocument doc(256);
    deserializeJson(doc, msg);
    String pin = doc["pin"];  // Extract PIN from JSON
    // Simple PIN validation (replace with secure check in production)
    if (pin == "1234") {
      // Unlock: pulse relay high for duration
      digitalWrite(RELAY_PIN, HIGH);
      delay(RELAY_PULSE_DURATION_MS);
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("Relay pulsed: unlocked");
    } else {
      Serial.println("Invalid PIN received");
    }
  }
  // Handle provisioning trigger
  else if (strcmp(topic, MQTT_PROVISION_TOPIC) == 0) {
    startBLEProvisioning();  // Start provisioning on command
  }
}

// FreeRTOS task for MQTT handling
// Runs in background to maintain MQTT connection and process messages.
void mqttTask(void* pvParameters) {
  while (true) {
    // Check connection and reconnect if needed
    if (!mqttClient.connected()) {
      reconnectMQTT();
    }
    mqttClient.loop();  // Process incoming/outgoing MQTT traffic
    // Delay to allow other tasks to run
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Reconnect to MQTT broker
// Attempts to connect and subscribe to topics.
void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    // Attempt connection with machine ID as client ID
    if (mqttClient.connect(MACHINE_ID)) {
      Serial.println("Connected!");
      // Subscribe to unlock and provisioning topics
      mqttClient.subscribe(MQTT_UNLOCK_TOPIC);
      mqttClient.subscribe(MQTT_PROVISION_TOPIC);
      // Publish initial status
      publishStatus();
    } else {
      Serial.print("Failed, rc=");
      Serial.println(mqttClient.state());
      // Wait before retrying
      delay(5000);
    }
  }
}

// Publish current status via MQTT
// Sends JSON with WiFi state, connection type, RSSI, etc.
void publishStatus() {
  DynamicJsonDocument doc(256);  // JSON document for payload
  doc["wifiState"] = "connected";  // WiFi connection status
  doc["ssid"] = WiFi.SSID();       // Current SSID
  doc["ip"] = WiFi.localIP().toString();  // IP address
  // Add connectivity details
  doc["connType"] = (currentConn == WIFI) ? "WiFi" : (currentConn == LTE) ? "LTE" : "offline";
  doc["rssi"] = WiFi.RSSI();       // Signal strength
  doc["dataKB"] = 0;               // Placeholder for data usage (implement later)
  // Serialize to string and publish
  String msg;
  serializeJson(doc, msg);
  mqttClient.publish(MQTT_STATUS_TOPIC, msg.c_str());
}

// Check provisioning button for long-press
// Debounces and detects 3-second press to trigger provisioning.
void checkProvisioningButton() {
  static unsigned long pressStart = 0;  // Timestamp of press start
  static bool pressed = false;          // Flag for press state
  // Check if button is pressed (LOW due to pull-up)
  if (digitalRead(PROVISIONING_BUTTON_PIN) == LOW) {
    if (!pressed) {
      pressStart = millis();  // Record start time
      pressed = true;
    } else if (millis() - pressStart > 3000) {  // 3-second threshold
      startBLEProvisioning();  // Trigger provisioning
      pressed = false;         // Reset flag
    }
  } else {
    pressed = false;  // Reset on release
  }
}

// Check connectivity and handle failover
// Monitors WiFi status and switches to LTE if needed.
void checkConnectivity() {
  static unsigned long wifiCheckStart = 0;  // Timer for WiFi failure
  if (currentConn == WIFI) {
    // If WiFi disconnected
    if (WiFi.status() != WL_CONNECTED) {
      if (wifiCheckStart == 0) {
        wifiCheckStart = millis();  // Start timer
        Serial.println("WiFi disconnected, starting failover timer...");
      } else if (millis() - wifiCheckStart > WIFI_FAILOVER_TIMEOUT_MS) {
        Serial.println("WiFi failover timeout, switching to LTE...");
        switchToLTE();  // Switch to LTE
      }
    } else {
      wifiCheckStart = 0;  // Reset timer if connected
      // Power down modem when on WiFi
      powerDownModem();
    }
  } else if (currentConn == LTE) {
    // If WiFi back, switch back
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi back, switching from LTE...");
      switchToWiFi();  // Switch to WiFi
    }
  }
}

/**
 * @file config_loader.h
 * @brief Header for runtime configuration loading from LittleFS JSON.
 * @author BBXtreme + Grok
 * @date 2026-02-28
 * @note Defines structs for all config and functions to load/save with validation.
 */

#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <vector>

// Structs for configuration
struct MqttConfig {
    std::string broker;
    int port;
    std::string username;
    std::string password;
    std::string clientIdPrefix;
};

struct LocationConfig {
    std::string slug;
    std::string code;
};

struct WifiProvisioningConfig {
    std::string fallbackSsid;
    std::string fallbackPass;
};

struct LteConfig {
    std::string apn;
    std::string simPin;
    int rssiThreshold;
    int dataUsageAlertLimitKb;
};

struct CompartmentConfig {
    int number;
    int servoPin;
    int limitOpenPin;
    int limitClosePin;
    int ultrasonicTriggerPin;
    int ultrasonicEchoPin;
    int weightSensorPin;
};

struct SystemConfig {
    int maxCompartments;
    bool debugMode;
    int gracePeriodSec;
    int batteryLowThresholdPercent;
    float solarVoltageMin;
};

struct OtherConfig {
    int offlinePinTtlSec;
    int depositHoldAmountFallback;
};

// Global config struct (central place for ALL config)
struct GlobalConfig {
    MqttConfig mqtt;
    LocationConfig location;
    WifiProvisioningConfig wifiProvisioning;
    LteConfig lte;
    std::vector<CompartmentConfig> compartments;
    SystemConfig system;
    OtherConfig other;
};

// Global config instance
extern GlobalConfig g_config;

// Function to load config from LittleFS, with validation and fallback
bool loadConfig();

// Function to save config to LittleFS (for remote update), with validation
bool saveConfig();

// Function to update config from JSON payload, validate, save, reload
bool updateConfigFromJson(const std::string& jsonPayload);

// Function to get default config (static struct)
GlobalConfig getDefaultConfig();

// Function to validate config (pins 0-39, etc.)
bool validateConfig(const GlobalConfig& cfg);

#endif // CONFIG_LOADER_H

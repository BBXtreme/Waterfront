// provisioning.cpp - Provisioning coordination module
// This file coordinates WiFi provisioning by calling BLE or SoftAP functions.
// It acts as a bridge between the main application and specific provisioning implementations.
// Provisioning allows runtime configuration of WiFi credentials without reflashing.

#include "provisioning.h"
#include "nimble.h"
#include "webui_server.h"
#include "config_loader.h"

// Start BLE provisioning
// Calls BLE init function and sets active flag.
void startBLEProvisioning() {
    ble_init(g_config.location.slug.c_str());
    provisioningActive = true;    // Indicate provisioning is running
}

// Start SoftAP provisioning
// Calls SoftAP and server init functions, sets active flag.
void startSoftAPProvisioning() {
    start_softap();
    start_rest_server();
    provisioningActive = true;  // Indicate provisioning active
}

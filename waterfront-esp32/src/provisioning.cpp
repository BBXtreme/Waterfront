// provisioning.cpp - Provisioning coordination module
// This file coordinates WiFi provisioning by calling BLE or SoftAP functions.
// It acts as a bridge between the main application and specific provisioning implementations.
// Provisioning allows runtime configuration of WiFi credentials without reflashing.

#include "provisioning.h"     // Header for provisioning functions
#include "nimble.h"           // BLE provisioning functions
#include "webui_server.h"    // SoftAP provisioning functions

// Start BLE provisioning
// Calls BLE init function and sets active flag.
void startBLEProvisioning() {
    ble_init("WATERFRONT-PROV");  // Initialize BLE with provisioning name
    provisioningActive = true;    // Indicate provisioning is running
}

// Start SoftAP provisioning
// Calls SoftAP and server init functions, sets active flag.
void startSoftAPProvisioning() {
    start_softap();           // Start AP mode
    start_rest_server();     // Start web server
    provisioningActive = true;  // Indicate provisioning active
}

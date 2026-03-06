// offline_fallback.cpp
#include "offline_fallback.h"
#include "config_loader.h"
#include <nvs.h>
#include <ArduinoJson.h>   // ← change to "cJSON.h" later
#include <string.h>

#define NVS_NAMESPACE "pins"
#define MAX_BOOKINGS 10

struct BookingEntry {
    char bookingId[32];
    char pin[8];
    time_t expires;
};

static BookingEntry activeBookings[MAX_BOOKINGS];
static int numActiveBookings = 0;

void offline_init() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
    ESP_LOGI("OFFLINE", "NVS initialized");
}

void offline_sync_pins(const char* payload) {
    // TODO: replace with cJSON later
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        ESP_LOGE("OFFLINE", "JSON parse failed: %s", error.c_str());
        return;
    }

    numActiveBookings = 0;
    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr) {
        if (numActiveBookings >= MAX_BOOKINGS) break;
        strlcpy(activeBookings[numActiveBookings].bookingId, obj["bookingId"] | "", 32);
        strlcpy(activeBookings[numActiveBookings].pin, obj["pin"] | "", 8);
        // TODO: proper expires parsing
        activeBookings[numActiveBookings].expires = time(NULL) + 86400; // placeholder
        numActiveBookings++;
    }

    // Save to NVS
    nvs_handle_t h;
    nvs_open(NVS_NAMESPACE, NVS_READWRITE, &h);
    nvs_set_blob(h, "bookings", activeBookings, sizeof(BookingEntry) * numActiveBookings);
    nvs_commit(h);
    nvs_close(h);

    ESP_LOGI("OFFLINE", "Synced %d bookings", numActiveBookings);
}

bool offline_validate_pin(const char* enteredPin) {
    time_t now = time(NULL);
    for (int i = 0; i < numActiveBookings; i++) {
        if (strcmp(activeBookings[i].pin, enteredPin) == 0 && activeBookings[i].expires > now) {
            return true;
        }
    }
    return false;
}

void offline_load_pins() {
    nvs_handle_t h;
    if (nvs_open(NVS_NAMESPACE, NVS_READONLY, &h) != ESP_OK) return;

    size_t sz = sizeof(BookingEntry) * MAX_BOOKINGS;
    nvs_get_blob(h, "bookings", activeBookings, &sz);
    numActiveBookings = sz / sizeof(BookingEntry);
    nvs_close(h);
    ESP_LOGI("OFFLINE", "Loaded %d bookings from NVS", numActiveBookings);
}

void offline_cleanup_expired() {
    time_t now = time(NULL);
    int j = 0;
    for (int i = 0; i < numActiveBookings; i++) {
        if (activeBookings[i].expires > now) {
            activeBookings[j++] = activeBookings[i];
        }
    }
    numActiveBookings = j;
}
// main/offline_fallback.h
#ifndef OFFLINE_FALLBACK_H
#define OFFLINE_FALLBACK_H

#include <esp_log.h>
#include <nvs.h>
#include <time.h>

void offline_init();
void offline_sync_pins(const char* payload);
bool offline_validate_pin(const char* enteredPin);
void offline_load_pins();
void offline_cleanup_expired();

#endif // OFFLINE_FALLBACK_H
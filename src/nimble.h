// Adapted from original nimble.h
#ifndef NIMBLE_H
#define NIMBLE_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

void ble_init(const char* deviceName, void (*ble_event_handler)(char*), void (*ble_pax_handler)(int));
void ble_set_device_name(const char* deviceName);
void ble_notify_send(char* notification, int length);
void ble_scan_start(int duration_seconds);
void ble_scan_stop();

#endif // NIMBLE_H

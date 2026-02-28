// Adapted from original nimble.c, ported to Arduino BLE
#include "nimble.h"
#include "config.h"
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEAdvertising* pAdvertising = NULL;
BLEScan* pBLEScan = NULL;

bool scanning = false;

typedef struct {
    uint8_t addr[6];
} ble_device_t;

typedef struct {
    ble_device_t entries[MAX_DEVICES];
    uint16_t count;
    time_t count_begin_time;
} ble_device_list_t;

ble_device_list_t ble_device_list = {0};

void (*ble_event_report_handler)(char*) = NULL;
void (*ble_pax_report_handler)(int) = NULL;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        ESP_LOGI("BLE", "Device connected");
    }
    void onDisconnect(BLEServer* pServer) {
        ESP_LOGI("BLE", "Device disconnected");
        pAdvertising->start();
    }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
            char* data = (char*)value.c_str();
            ble_event_report_handler(data);
        }
    }
};

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // Simplified pax counter logic
        if (advertisedDevice.haveManufacturerData()) {
            std::string mfgData = advertisedDevice.getManufacturerData();
            // Check for phone manufacturers
            // Simplified: assume if mfg data present, it's a phone
            ble_device_list_add(&ble_device_list, (uint8_t*)advertisedDevice.getAddress().getNative());
        }
    }
};

void ble_device_list_add(ble_device_list_t* list, const uint8_t addr[6]) {
    for (int i = 0; i < list->count; i++) {
        if (memcmp(list->entries[i].addr, addr, 6) == 0) {
            return;
        }
    }
    if (list->count < MAX_DEVICES) {
        memcpy(list->entries[list->count++].addr, addr, 6);
    }
}

void ble_init(const char* deviceName, void (*ble_event_handler)(char*), void (*ble_pax_handler)(int)) {
    ble_event_report_handler = ble_event_handler;
    ble_pax_report_handler = ble_pax_handler;

    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService* pService = pServer->createService(BLEUUID("020012ac420278b8ed11da4642c6bbb2"));

    pCharacteristic = pService->createCharacteristic(
        BLEUUID("020012ac420278b8ed11de4649769cafc9"),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
    );
    pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());

    pService->start();

    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
}

void ble_set_device_name(const char* deviceName) {
    if (pServer) {
        pServer->setDeviceName(deviceName);
        pAdvertising->stop();
        pAdvertising->start();
    }
}

void ble_notify_send(char* notification, int length) {
    if (pCharacteristic && pServer->getConnectedCount() > 0) {
        pCharacteristic->setValue((uint8_t*)notification, length);
        pCharacteristic->notify();
    }
}

void ble_scan_start(int duration_seconds) {
    if (scanning) return;
    scanning = true;
    pBLEScan->start(duration_seconds);
}

void ble_scan_stop() {
    if (!scanning) return;
    scanning = false;
    pBLEScan->stop();
}

// Timer callback for pax report
void pax_report_timer() {
    if (ble_device_list.count > 0) {
        ble_pax_report_handler(ble_device_list.count);
    }
    memset(&ble_device_list, 0, sizeof(ble_device_list));
    ble_device_list.count_begin_time = time(NULL);
}

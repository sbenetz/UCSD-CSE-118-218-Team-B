#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>

BLEServer* pServer = NULL;
BLECharacteristic* pNotifyCharacteristic = NULL;
BLECharacteristic* pWifiCharacteristic = NULL;
bool deviceConnected = false;
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

class WiFiCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
            Serial.println("Received WiFi Credentials");
            Serial.println(value.c_str());

            // Parse WiFi credentials
            String wifiCredentials = String(value.c_str());
            int separatorIndex = wifiCredentials.indexOf(',');
            if (separatorIndex != -1) {
                //ssid = wifiCredentials.substring(0, separatorIndex).c_str();
                //password = wifiCredentials.substring(separatorIndex + 1).c_str();
            }
        }
    }
};

//     // Create the BLE Device
//     BLEDevice::init("ESP32_BLE_Device");

//     // Create the BLE Server
//     pServer = BLEDevice::createServer();
//     pServer->setCallbacks(new MyServerCallbacks());

//     // Create the BLE Service
//     BLEService *pService = pServer->createService(BLEUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"));

//     // Create a BLE Characteristic for notifications
//     pNotifyCharacteristic = pService->createCharacteristic(
//         BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"),
//         BLECharacteristic::PROPERTY_READ |
//         BLECharacteristic::PROPERTY_NOTIFY |
//         BLECharacteristic::PROPERTY_INDICATE
//     );

//     // Create a BLE Characteristic for WiFi credentials
//     pWifiCharacteristic = pService->createCharacteristic(
//         BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a9"),
//         BLECharacteristic::PROPERTY_WRITE
//     );
//     pWifiCharacteristic->setCallbacks(new WiFiCharacteristicCallbacks());

//     // Start the service
//     pService->start();

//     // Start advertising
//     pServer->getAdvertising()->start();
//     Serial.println("Waiting for a connection...");
// }
#ifndef RF_COMMS_H
#define RF_COMMS_H
#include <WiFi.h>
#include <Preferences.h>
#include <ArduinoJson.h>
// Includes for BLE
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>

#include "HttpComms.h"
#include "IO.h"

#define BLUETOOTH_NAME "Plant Waterer"
#define SERVICE_UUID "ab1fb9be-ce51-4b92-b0ec-f4ad17cafdda"
#define CHARACTERISTIC_UUID "67148bf4-ce72-4540-83c0-fcbea11c06a5"
#define STORAGE_NAME "device"

String ssids_array[50];
Preferences preferences;

String client_wifi_ssid;
String client_wifi_password;

String userID = "OaVlxIFhjBzfm2hARNZwOugQm";
String plantName = "greenie";
uint16_t plantType = 1;
uint8_t plantSize = 1; // 0=small, 1=medium, 2=large

String deviceID;

long start_wifi_millis;
long wifi_timeout = 10000; // 10 sec timeout

bool hasCredentials = false;       // If stored WiFi credentials are available
volatile bool isConnected = false; // WiFi connection status
bool connStatusChanged = false;    // Wifi connection change status
bool hasDeviceProps = false;       // Received info about this device over bluetooth

/** Bluetooth pointers */
BLECharacteristic *pCharacteristicWiFi; // Characteristic for digital output
BLEAdvertising *pAdvertising;           // BLE Advertiser
BLEService *pService;                   // BLE Service
BLEServer *pServer;                     // BLE Server

StaticJsonDocument<200> jsonBuffer;

enum setup_stages
{
  NONE,
  SCANNING,
  WIFI_CONNECTED,
  WIFI_FAILED,
  REGISTERING_DEVICE,
  SETUP_DONE
};
enum setup_stages setup_stage = NONE;

String scan_wifi_networks()
{
  Serial.println("Start scanning for networks");
  WiFi.disconnect(true);
  WiFi.enableSTA(true);
  WiFi.mode(WIFI_STA);

  String output;
  // Scan for networks
  int16_t apNum = WiFi.scanNetworks(false, false, false, 1000);
  if (apNum == 0)
  {
    Serial.println("Found no networks?????");
    output = "No Networks Found";
  }
  else
  {
    output = "Networks Found:\n";
    for (int i = 0; i < apNum; ++i)
    {
      ssids_array[i + 1] = WiFi.SSID(i);
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(ssids_array[i + 1] + " (Strength:" + String(WiFi.RSSI(i)) + ")");
      output += String(i + 1);
      output += ": " + WiFi.SSID(i) + " (Strength:" + WiFi.RSSI(i) + ")\n";
    }
  }
  return output;
}

void wipeDevice()
{
  // erase all data
  Serial.println("Wiping device info.");
  if (!deviceID.isEmpty() && isConnected)
  {
    postReset(deviceID);
  }

  preferences.begin(STORAGE_NAME, false);
  preferences.clear();
  preferences.end();
  connStatusChanged = true;
  hasCredentials = false;
  hasDeviceProps = false;

  memset(ssids_array, '\0', sizeof(ssids_array));
  userID = "";
  plantName = "";
  plantType = 0;
  plantSize = 0; // 0=small, 1=medium, 2=large

  deviceID = "";
  client_wifi_password = "";
  client_wifi_ssid = "";
}

/**
 * MyServerCallbacks
 * Callbacks for client connection and disconnection
 */
class MyServerCallbacks : public BLEServerCallbacks
{
  // TODO this doesn't take into account several clients being connected
  void onConnect(BLEServer *pServer)
  {
    builtinLED.setBlinkOnboardLED(6, 200);
    Serial.println("BLE client connected");
  };

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("BLE client disconnected");
    pAdvertising->start();
  }
};

/**
 * MyCallbackHandler
 * Callbacks for BLE client read/write requests
 */
class MyCallbackHandler : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    if (value.length() == 0)
    {
      return;
    }
    Serial.println("Received over BLE: " + String((char *)&value[0]));

    /** Json object for incoming data */
    auto error = deserializeJson(jsonBuffer, (char *)&value[0]);
    if (error)
    {
      Serial.print("deserializeJson() failed with code ");
      Serial.println(error.c_str());
      return;
    }
    else
    {
      JsonObject data = jsonBuffer.as<JsonObject>();
      if (data.containsKey("ssid") && data.containsKey("password"))
      {
        client_wifi_ssid = data["ssid"].as<String>();
        client_wifi_password = data["password"].as<String>();

        preferences.begin(STORAGE_NAME, false);
        preferences.putString("pref_ssid", client_wifi_ssid);
        preferences.putString("pref_pass", client_wifi_password);
        preferences.putBool("valid", true);
        preferences.end();
        Serial.println("Received over bluetooth:");
        Serial.println("primary SSID: " + client_wifi_ssid + " password: " + client_wifi_password);
        connStatusChanged = true;
        hasCredentials = true;
      }
      else if (setup_stage == WIFI_CONNECTED && data.containsKey("user_id") && data.containsKey("plant_name") && data.containsKey("plant_type") && data.containsKey("plant_size"))
      {
        userID = data["user_id"].as<String>();
        plantName = data["plant_name"].as<String>();
        plantSize = data["plant_size"].as<uint16_t>();
        plantType = data["plant_type"].as<uint8_t>();
        hasDeviceProps = true;
        setup_stage = REGISTERING_DEVICE;
      }
      else if (data.containsKey("search_wifi"))
      {
        setup_stage = SCANNING;
        Serial.println("Starting WiFi Scan...");
      }
      else if (data.containsKey("erase"))
      {
        Serial.println("Received erase command");
        wipeDevice();
        WiFi.disconnect();
        esp_restart();
      }
      else if (data.containsKey("reset"))
      {
        Serial.println("Received reset command");
        esp_restart();
      }
      else
      {
        Serial.println("Received invalid JSON");
      }
    }
    jsonBuffer.clear();
  };

  void onRead(BLECharacteristic *pCharacteristic)
  {
    Serial.println("BLE onRead request");
    String message;
    switch (setup_stage)
    {
    case (NONE):
      message = "Hello from Plant Waterer";
      break;
    case (SCANNING):
      message = scan_wifi_networks();
      break;
    case (WIFI_CONNECTED):
      message = "WiFi Connection is established";
      break;
    case (WIFI_FAILED):
      message = "WiFi Connection failed";
      break;
    case (REGISTERING_DEVICE):
      message = "Recieved necessary info. Attempting device registration.";
      break;
    case (SETUP_DONE):
      message = "Device has been setup!";
      break;
    }
    // /** Json object for outgoing data */
    // JsonObject& jsonOut = jsonBuffer.createObject();
    // jsonOut["ssid"] = ssidPrim;
    // jsonOut["passwrod"] = pwPrim;
    // Convert JSON object into a string
    // jsonOut.printTo(message);
    pCharacteristicWiFi->setValue(std::string(message.c_str()));
    // jsonBuffer.clear();
  }
};

/** Callback for receiving IP address from AP */
void gotIP(WiFiEvent_t event)
{
  isConnected = true;
  connStatusChanged = true;
}

/** Callback for connection loss */
void lostCon(WiFiEvent_t event)
{
  isConnected = false;
  connStatusChanged = true;
}

void init_wifi()
{
  // Setup callback function for successful connection
  WiFi.onEvent(gotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  // Setup callback function for lost connection
  WiFi.onEvent(lostCon, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.disconnect(true);
  WiFi.enableSTA(true);
  WiFi.mode(WIFI_STA);
  preferences.begin(STORAGE_NAME, true);
  String pref_ssid;
  String pref_pass;
  if (preferences.isKey("pref_ssid") && preferences.isKey("pref_pass"))
  {
    pref_ssid = preferences.getString("pref_ssid");
    pref_pass = preferences.getString("pref_pass");
  }
  preferences.end();

  if (pref_ssid.length() > 0)
  {
    Serial.println(pref_ssid);
    Serial.println(pref_pass);

    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);

    start_wifi_millis = millis();
    WiFi.begin(pref_ssid, pref_pass);
    Serial.print("Attempting to connect WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
      if (millis() - start_wifi_millis > wifi_timeout)
      {
        Serial.println("\nAttempt timed out");
        WiFi.disconnect(true, true);
        setup_stage = WIFI_FAILED;
        return;
      }
    }
    Serial.println("\nConnected to " + pref_ssid);
    setup_stage = WIFI_CONNECTED;
    builtinLED.setBlinkOnboardLED(5, 200);
  }
  else
  {
    Serial.println("No credentials stored.");
  }
}

/**
 * Initialize BLE service and characteristic
 * Start BLE server and service advertising
 */
void initBLE()
{
  // Initialize BLE and set output power
  BLEDevice::init(BLUETOOTH_NAME);
  BLEDevice::setPower(ESP_PWR_LVL_P7);
  pServer = BLEDevice::createServer();
  // Set server callbacks
  pServer->setCallbacks(new MyServerCallbacks());
  // Create BLE Service
  pService = pServer->createService(BLEUUID(SERVICE_UUID), 20);
  // Create BLE Characteristic for WiFi settings
  pCharacteristicWiFi = pService->createCharacteristic(
      BLEUUID(CHARACTERISTIC_UUID),
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharacteristicWiFi->setCallbacks(new MyCallbackHandler());
  // Start the service
  pService->start();
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}
/// @brief Start up the Bluetooth and WiFi communication streams for the device
void beginRFServices()
{
  Serial.println("Booting Communications...");
  if (esp_reset_reason() == ESP_RST_POWERON)
  {
    Serial.println("Reset button pressed");
    wipeDevice();
  }
  else
  {
    preferences.begin(STORAGE_NAME, false);
    if (preferences.isKey("device_id"))
    {
      deviceID = preferences.getString("device_id");
    }
    preferences.end();
  }
  init_wifi();
  if (!deviceID.isEmpty() && isConnected)
  {
    setup_stage = SETUP_DONE;
  }
  else // only need bluetooth if we need to setup device again
  {
    initBLE();
  }
}

/// @brief communications to do in the main loop
void inLoop()
{
  if (connStatusChanged)
  {
    if (isConnected)
    {
      Serial.print("Connected to AP: ");
      Serial.print(WiFi.SSID());
      Serial.print(" with IP: ");
      Serial.print(WiFi.localIP());
      Serial.print(", RSSI: ");
      Serial.println(WiFi.RSSI());
    }
    else
    {
      if (hasCredentials)
      {
        init_wifi();
      }
    }
    connStatusChanged = false;
  }
  if (setup_stage == REGISTERING_DEVICE && hasDeviceProps && isConnected)
  {
    deviceID = postNewDevice(userID, plantName, plantType, plantSize);
    Serial.println("Device ID set: " + deviceID);
    preferences.begin(STORAGE_NAME, false);
    preferences.putString("device_id", deviceID);
    preferences.end();
    setup_stage = SETUP_DONE;
  }
}
#endif
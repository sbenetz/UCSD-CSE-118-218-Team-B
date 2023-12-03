#include <WiFi.h>
#include <Preferences.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <ArduinoJson.h>
// Includes for BLE
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>

#include "Credentials.h"

String ssids_array[50];
String network_string;
String connected_string;
Preferences preferences;

const char *pref_ssid = "";
const char *pref_pass = "";
String client_wifi_ssid;
String client_wifi_password;

const char *bluetooth_name = "Plant Waterer";
String deviceID;

long start_wifi_millis;
long wifi_timeout = 10000; // 10 sec timeout

/** Flag if stored AP credentials are available */
bool hasCredentials = false;
/** Connection status */
volatile bool isConnected = false;
/** Connection change status */
bool connStatusChanged = false;

/** Characteristic for digital output */
BLECharacteristic *pCharacteristicWiFi;
/** BLE Advertiser */
BLEAdvertising *pAdvertising;
/** BLE Service */
BLEService *pService;
/** BLE Server */
BLEServer *pServer;

StaticJsonDocument<200> jsonBuffer;

#define SERVICE_UUID "ab1fb9be-ce51-4b92-b0ec-f4ad17cafdda"
#define CHARACTERISTIC_UUID "67148bf4-ce72-4540-83c0-fcbea11c06a5"
#define STORAGE_NAME "device"
enum wifi_setup_stages
{
  NONE,
  SCANNING,
  SSID_ENTERED,
  WAIT_PASS,
  PASS_ENTERED,
  WAIT_CONNECT,
  LOGIN_FAILED,
  CONNECTED
};
enum wifi_setup_stages wifi_stage = NONE;

String scan_wifi_networks()
{
  Serial.println("Start scanning for networks");
  WiFi.disconnect(true);
  WiFi.enableSTA(true);
  WiFi.mode(WIFI_STA);

  String output;
  // Scan for networks
  int16_t apNum = WiFi.scanNetworks(false, true, false, 1000);
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
      Serial.println(ssids_array[i + 1]);
      output += String(i + 1);
      output += network_string + ": " + WiFi.SSID(i) + " (Strength:" + WiFi.RSSI(i) + ")\n";
    }
  }
  return output;
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
      else if (data.containsKey("search_wifi"))
      {
        wifi_stage = SCANNING;
      }
      else if (data.containsKey("erase"))
      {
        Serial.println("Received erase command");

        preferences.begin(STORAGE_NAME, false);
        preferences.clear();
        preferences.end();
        connStatusChanged = true;
        hasCredentials = false;
        client_wifi_password = "";
        client_wifi_ssid = "";

        int err;
        err = nvs_flash_init();
        Serial.println("nvs_flash_init: " + err);
        err = nvs_flash_erase();
        Serial.println("nvs_flash_erase: " + err);
      }
      else if (data.containsKey("reset"))
      {
        WiFi.disconnect();
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
    switch (wifi_stage)
    {
    case (NONE):
      message = "Hello from Plant Waterer";
      break;
    case (SCANNING):
      message = scan_wifi_networks();
      break;
      // case (SCAN_COMPLETE):
      // case (SSID_ENTERED):
      // case (WAIT_PASS):
      // case (PASS_ENTERED):
      // case (WAIT_CONNECT):
      // case (LOGIN_FAILED):
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

bool init_wifi()
{
  // Setup callback function for successful connection
  WiFi.onEvent(gotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  // Setup callback function for lost connection
  WiFi.onEvent(lostCon, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.disconnect(true);
  WiFi.enableSTA(true);
  WiFi.mode(WIFI_STA);
  preferences.begin(STORAGE_NAME, true);
  String temp_pref_ssid;
  String temp_pref_pass;
  if (preferences.isKey("pref_ssid") && preferences.isKey("pref_pass"))
  {
    temp_pref_ssid = preferences.getString("pref_ssid");
    temp_pref_pass = preferences.getString("pref_pass");
    pref_ssid = temp_pref_ssid.c_str();
    pref_pass = temp_pref_pass.c_str();
  }
  preferences.end();

  if (strlen(pref_ssid) > 0)
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
        Serial.println();
        Serial.println("Attempt timed out");
        WiFi.disconnect(true, true);
        return false;
      }
    }
    return true;
  }
  else
  {
    Serial.println("No credentials stored.");
    return false;
  }
}

/**
 * Initialize BLE service and characteristic
 * Start BLE server and service advertising
 */
void initBLE()
{
  // Initialize BLE and set output power
  BLEDevice::init(bluetooth_name);
  BLEDevice::setPower(ESP_PWR_LVL_P7);

  // Create BLE Server
  pServer = BLEDevice::createServer();
  // Set server callbacks
  pServer->setCallbacks(new MyServerCallbacks());
  // Create BLE Service
  pService = pServer->createService(BLEUUID(SERVICE_UUID), 20);
  // Create BLE Characteristic for WiFi settings
  pCharacteristicWiFi = pService->createCharacteristic(
      BLEUUID(CHARACTERISTIC_UUID),
      // WIFI_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharacteristicWiFi->setCallbacks(new MyCallbackHandler());
  // Start the service
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}
/**
 * @brief Start up the Bluetooth and WiFi communication streams for the device
 *
 */
void beginRFServices()
{
  Serial.println("Booting Communications...");
  preferences.begin(STORAGE_NAME, false);
  // preferences.putString("pref_ssid", WIFI_SSID);
  // preferences.putString("pref_pass", WIFI_PASS);
  deviceID = preferences.getString("device_id");
  bool connected = init_wifi();
  if (deviceID.isEmpty() && connected)
  {
    deviceID = postNewDevice("shane", "greenie", 1);
    Serial.println("Device ID set: " + deviceID);
    preferences.putString("device_id", deviceID);
  }
  preferences.end();
  initBLE();
}

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
      Serial.print(" RSSI: ");
      Serial.println(WiFi.RSSI());
    }
    else
    {
      if (hasCredentials)
      {
        Serial.println("Lost WiFi connection");
        // Received WiFi credentials
        // if (!scanWiFi) { // Check for available AP's
        // 	Serial.println("Could not find any AP");
        // } else { // If AP was found, start connection
        // 	connectWiFi();
        // }
      }
    }
    connStatusChanged = false;
  }

  // switch (wifi_stage)
  // {
  //   case SCAN_START:
  //     //SerialBT.println("Scanning Wi-Fi networks");
  //     Serial.println("Scanning Wi-Fi networks");
  //     scan_wifi_networks();
  //     //SerialBT.println("Please enter the number for your Wi-Fi");
  //     wifi_stage = SCAN_COMPLETE;
  //     break;

  //   case SSID_ENTERED:
  //     SerialBT.println("Please enter your Wi-Fi password");
  //     Serial.println("Please enter your Wi-Fi password");
  //     wifi_stage = WAIT_PASS;
  //     break;

  //   case PASS_ENTERED:
  //     SerialBT.println("Please wait for Wi-Fi connection...");
  //     Serial.println("Please wait for Wi_Fi connection...");
  //     wifi_stage = WAIT_CONNECT;
  //     preferences.putString("pref_ssid", client_wifi_ssid);
  //     preferences.putString("pref_pass", client_wifi_password);
  //     if (init_wifi()) { // Connected to WiFi
  //       connected_string = "ESP32 IP: ";
  //       connected_string = connected_string + WiFi.localIP().toString();
  //       SerialBT.println(connected_string);
  //       Serial.println(connected_string);
  //       bluetooth_disconnect = true;
  //     } else { // try again
  //       wifi_stage = LOGIN_FAILED;
  //     }
  //     break;

  //   case LOGIN_FAILED:
  //     SerialBT.println("Wi-Fi connection failed");
  //     Serial.println("Wi-Fi connection failed");
  //     delay(2000);
  //     wifi_stage = SCAN_START;
  //     break;
  // }
}
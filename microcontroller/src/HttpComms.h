#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Certificate.h"

#define SERVER_URL "http://fit-glowworm-promptly.ngrok-free.app/"

/**
 * @brief Sends a request to the server to setup this device and gets an Id back
 *
 * @param user_id which user is using this device
 * @param plant_name name given to this device
 * @param plant_type_id what type of plant this is
 * @return device ID returned from server, empty if failed
 */
String postNewDevice(String user_id, String plant_name, uint16_t plant_type_id)
{
    HTTPClient http;
    WiFiClient client;
    // client.setCACert(CERTIFICATE);
    //  client.setInsecure();
    //  http.setReuse(true);
    if (!http.begin(client, SERVER_URL + String("device/initialization")))
    {
        Serial.println("Can't reach server");
        return "";
    }
    http.addHeader("Content-Type", "application/json");
    String post_body = "{\"userId\":\"" + user_id + "\",\"plantName\":\"" + plant_name + "\",\"plantType\":" + String(plant_type_id) + "}";
    Serial.println(post_body);
    int httpResponseCode = http.POST(post_body);
    String payload = http.getString();
    Serial.printf("New Device HTTP Response: [%d] %s %s\n", httpResponseCode, HTTPClient::errorToString(httpResponseCode).c_str(), payload.c_str());
    http.end(); // Free resources
    if (payload.length() > 0 && httpResponseCode == 200)
    {
        return payload;
    }
    else
    {
        Serial.println("Cannot setup device");
        return "";
    }
}

/**
 * @brief Posts updates to the server for providing sensor readings and getting watering settings back
 *
 * @param device_id id of this device
 * @param soil_moisture percentage moisture in soil
 * @param sunlight integer sunlight level to report
 * @return integer watering level for plant
 */
int postSensorReadings(String device_id, uint32_t soil_moisture, uint32_t sunlight)
{
    HTTPClient http;
    WiFiClient client;
    // client.setCACert(CERTIFICATE);
    //  http.setReuse(true);
    if (!http.begin(client, SERVER_URL + String("device/check-in")))
    {
        Serial.println("Can't reach server");
        return -1;
    };
    http.addHeader("Content-Type", "application/json");
    String post_body = "{\"deviceId\":" + device_id + ",\"soilMoisture\":" + String(soil_moisture) + ",\"sunlight\":" + String(sunlight) + "}";
    Serial.println(post_body);
    int httpResponseCode = http.POST(post_body);
    String payload = http.getString();
    Serial.printf("Post Sensors HTTP Response: [%d] %s %s\n", httpResponseCode, HTTPClient::errorToString(httpResponseCode).c_str(), payload.c_str());
    http.end(); // Free resources
    if (payload.length() > 0 && httpResponseCode == 200)
        return payload.toInt();
    else
        return -1;
}
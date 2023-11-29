#include <WiFi.h>
#include <HTTPClient.h>

#define SERVER_URL "https://2638-2603-8001-8d00-7e27-00-19c9.ngrok-free.app"

String postNewDevice(String user_id, String plant_name, uint16_t plant_type_id)
{
    WiFiClient client;
    HTTPClient http;
    http.begin(client, SERVER_URL + String("/device/initialization"));
    http.addHeader("Content-Type", "application/json");
    String post_body = "{\"userId\":\"" + user_id + "\",\"plantName\":\"" + plant_name + "\",\"plantType\":" + String(plant_type_id) + "}";
    int httpResponseCode = http.POST(post_body);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
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

int postSensorReadings(String device_id, uint32_t soil_moisture, uint32_t sunlight)
{
    WiFiClient client;
    HTTPClient http;
    http.begin(client, SERVER_URL + String("/device/check-in"));
    http.addHeader("Content-Type", "application/json");
    String post_body = "{\"deviceId\":" + device_id + ",\"soilMoisture\":" + String(soil_moisture) + ",\"sunlight\":" + String(sunlight) + "}";
    Serial.println(post_body);
    int httpResponseCode = http.POST(post_body);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    http.end(); // Free resources
    if (httpResponseCode == 200)
        return payload.toInt();
    else
        return -1;
}
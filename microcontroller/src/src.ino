#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <BH1750.h>
#include "BlinkLED.h"
#include "Credentials.h"

BH1750 light_meter; 
OnboardLED onBoardLED;
HTTPClient http;

#ifndef  ARRAY_SIZE
#define  ARRAY_SIZE 10
#endif
uint32_t last_soil_readings [ARRAY_SIZE];
uint32_t last_sunlight_readings [ARRAY_SIZE];
void setup() {
    // start up objects
    Serial.begin(115200);
    // BH1750 light meter
    Wire.begin(); 
    light_meter.begin();
    // startup onboard LED code
    onBoardLED.begin();
    //onBoardLED.setBlinkOnboardLED();
    // wifi initialization
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected to WiFi");
    // setup pins
    pinMode(25,OUTPUT); // powers soil moisture sensor
    pinMode(26,OUTPUT); // grounds soil moisture sensor
    pinMode(33,ANALOG); // reads value from soil moisture sensor
    // initalize pin values
    digitalWrite(25,HIGH); 
    digitalWrite(26,LOW); 
}

void loop() {
    static uint8_t i = 0;
    // if(WiFi.status()== WL_CONNECTED){
    //     HTTPClient http;
    //     http.begin("https://api.weather.gov/points/39.7456,-97.0892");
    //     int httpResponseCode = http.GET();
    //     if (httpResponseCode>0) {
    //         Serial.print("HTTP Response code: ");
    //         Serial.println(httpResponseCode);
    //         String payload = http.getString();
    //         Serial.println(payload);
    //     }
    //     else {
    //         Serial.print("Error code: ");
    //         Serial.println(httpResponseCode);
    //     }
    //     // Free resources
    //     http.end();
    // }
    if (i < ARRAY_SIZE){
        last_soil_readings[i] = analogReadMilliVolts(33);
        last_sunlight_readings[i] = (uint32_t)light_meter.readLightLevel();
        i++;
    }
    else {
        uint32_t soil_average = getAverage(last_soil_readings);
        uint32_t light_average = getAverage(last_sunlight_readings);
        Serial.printf("Soil_Moisture_(mV): %d, ",soil_average);
        Serial.printf("Light_(lx): %d \n",light_average);
        i = 0;
        onBoardLED.setBlinkOnboardLED(3);
    }
    

    onBoardLED.doBlink(); // run blinks if setup
    delay(100);
}

uint32_t getAverage(uint32_t array[ARRAY_SIZE]){
    uint64_t sum = 0;
    for(uint8_t j=0; j<ARRAY_SIZE;j++)
        sum += array[j];
    return (uint32_t)(sum/ARRAY_SIZE);
}
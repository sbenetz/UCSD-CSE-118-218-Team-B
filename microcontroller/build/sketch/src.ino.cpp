#line 1 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino"
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

#include "BlinkLED.h"
#include "Credentials.h"
#include "HttpComms.h"
#include "BluetoothComms.h"

BH1750 lightMeter;
OnboardLED builtinLED;

#define ARRAY_SIZE 10

uint32_t soilReadingHistory[ARRAY_SIZE];
uint32_t sunlightReadingHistory[ARRAY_SIZE];
String deviceID;

int watering_level = -1;
uint32_t soil_moisture_max = 2800;
uint32_t soil_moisture_min = 1000;

#line 23 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino"
void setup();
#line 45 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino"
void loop();
#line 73 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino"
uint32_t getAverage(uint32_t array[10]);
#line 23 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino"
void setup()
{
    // start up objects
    Serial.begin(115200);
    // BH1750 light meter
    Wire.begin();
    lightMeter.begin();
    // startup onboard LED code
    builtinLED.begin();
    // begin Bluetooth server and wifi if available
    preferences.putString("pref_ssid", WIFI_SSID);
    preferences.putString("pref_pass", WIFI_PASS);
    beginRFServices();
    deviceID = postNewDevice("Shane","greenie",1);
    // setup pins
    pinMode(25, OUTPUT); // powers soil moisture sensor
    pinMode(26, OUTPUT); // grounds soil moisture sensor
    pinMode(33, ANALOG); // reads value from soil moisture sensor
    // initalize pin values
    digitalWrite(25, HIGH);
    digitalWrite(26, LOW);
}
void loop()
{
    static uint8_t i = 0;
    inLoop();
    if (i < ARRAY_SIZE)
    {
        soilReadingHistory[i] = (uint8_t)((double)(soil_moisture_max-analogReadMilliVolts(33))/(double)(soil_moisture_max-soil_moisture_min)*100.0);
        sunlightReadingHistory[i] = (uint32_t)lightMeter.readLightLevel();
        i++;
    }
    else
    {
        uint32_t soil_average = getAverage(soilReadingHistory);
        uint32_t light_average = getAverage(sunlightReadingHistory);
        Serial.printf("Soil_Moisture_(mV): %d, ", soil_average);
        Serial.printf("Light_(lx): %d \n", light_average);
        builtinLED.setBlinkOnboardLED(3);
        watering_level = postSensorReadings(deviceID,soil_average,light_average);
        if((int)soil_average < watering_level){
            Serial.println("Watering");
        }
        i = 0;
    }

    builtinLED.doBlink(); // run blinks if setup
    delay(1000);
}

uint32_t getAverage(uint32_t array[ARRAY_SIZE])
{
    uint64_t sum = 0;
    for (uint8_t j = 0; j < ARRAY_SIZE; j++)
        sum += array[j];
    return (uint32_t)(sum / ARRAY_SIZE);
}

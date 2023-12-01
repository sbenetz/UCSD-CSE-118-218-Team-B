#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

#include "BlinkLED.h"
#include "Credentials.h"
#include "HttpComms.h"
#include "BluetoothComms.h"

#define WATER_PUMP_VCC 14
#define WATER_PUMP_GND 12
#define WATER_PUMP_SIG 27
#define SOIL_SENSOR_VCC 25
#define SOIL_SENSOR_GND 26
#define SOIL_SENSOR_SIG 33

int watering_level = 10;
uint32_t soil_moisture_max = 2750;
uint32_t soil_moisture_min = 1300;

#define ARRAY_SIZE 10
uint32_t sunlightReadingHistory[ARRAY_SIZE]; // 0 - ~65000(lumens)
uint32_t soilReadingHistory[ARRAY_SIZE];     // 0-100(%)

BH1750 lightMeter;
OnboardLED builtinLED;
HTTPClient http;

void waterPumpOn(uint time_ms)
{
    digitalWrite(WATER_PUMP_VCC, HIGH);
    digitalWrite(WATER_PUMP_SIG, HIGH);
    delay(time_ms);
    digitalWrite(WATER_PUMP_VCC, LOW);
    digitalWrite(WATER_PUMP_SIG, LOW);
}
void IOBegin()
{
    // BH1750 light meter
    Wire.begin();
    // pins are already set up through SCL & SDA
    lightMeter.begin();
    // startup onboard LED code
    builtinLED.begin();
    // begin Bluetooth server and wifi if available
    beginRFServices();
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
        soilReadingHistory[i] = (uint8_t)((double)(soil_moisture_max - analogReadMilliVolts(33)) / (double)(soil_moisture_max - soil_moisture_min) * 100.0);
        sunlightReadingHistory[i] = (uint32_t)lightMeter.readLightLevel();
        i++;
    }
    else
    {
        uint32_t soil_average = getAverage(soilReadingHistory);
        uint32_t light_average = getAverage(sunlightReadingHistory);
        Serial.printf("Soil_Moisture (%): %d, ", soil_average);
        Serial.printf("Soil Moisture (mV): %d, ", analogReadMilliVolts(SOIL_SENSOR_SIG));
        Serial.printf("Light_(lx): %d \n", light_average);
        builtinLED.setBlinkOnboardLED(3);
        watering_level = postSensorReadings(deviceID, soil_average, light_average);
        if ((int)soil_average < watering_level)
        {
            Serial.println("Watering");
            waterPumpOn(3000);
        }
        i = 0;
    }

    builtinLED.doBlink(); // run blinks if setup
    delay(500);
}

uint32_t getAverage(uint32_t array[ARRAY_SIZE])
{
    uint64_t sum = 0;
    for (uint8_t j = 0; j < ARRAY_SIZE; j++)
        sum += array[j];
    return (uint32_t)(sum / ARRAY_SIZE);
}
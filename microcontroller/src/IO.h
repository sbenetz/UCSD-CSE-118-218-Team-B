#ifndef IO_H
#define IO_H
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include <limits>

#include "Waterer.h"
#include "OnboardLED.h"

#define SOIL_SENSOR_VCC 25
#define SOIL_SENSOR_GND 26
#define SOIL_SENSOR_SIG 33

uint soil_moisture_max = 2675; // pretty consistent (around same as air)
uint soil_moisture_min = 1775; // want this to be adjustable since it varies a lot
bool self_calibrate = true;

#define BATTERY_PIN 35
#define MIN_BATTERY 3100 // min mV for Lolin D32 to operate
#define MAX_BATTERY 4200 // max mV from datasheet

OnboardLED builtinLED;
Waterer waterer;
BH1750 lightMeter;

/**
 * @brief Get the battery percentage
 *
 * @return (0-100)%
 *
 * @note The battery mV at pin 35 is halved so you have to multiply by 2
 */
uint8_t batteryPercent()
{
    return (uint8_t)100 - (uint8_t)(((double)(MAX_BATTERY - analogReadMilliVolts(BATTERY_PIN) * 2.0)) / (double)(MAX_BATTERY - MIN_BATTERY) * 100.0);
}

/**
 * @brief Get the current percentage soil moisture
 *
 * @return (0-100)%
 */
uint8_t readSoilMoisturePercent()
{
    uint soil_mV = readSoilMoistureMilliVolts();
    if (self_calibrate)
    {
        if (soil_moisture_min > soil_mV)
        {
            soil_moisture_min = soil_mV;
        }
        if (soil_moisture_max < soil_mV)
        {
            soil_moisture_max = soil_mV;
        }
    }
    return (uint8_t)((double)(soil_moisture_max - soil_mV) / (double)(soil_moisture_max - soil_moisture_min) * 100.0);
}
/**
 * @brief Read the soil moisture
 *
 * @return mV reading of soil moisture sensor
 */
uint readSoilMoistureMilliVolts()
{
    return analogReadMilliVolts(SOIL_SENSOR_SIG);
}

/**
 * @brief Read light level from sensor
 * @return Light level in lux (0 ~ 54612)
 * 	   0 : no valid return value
 */
uint readLightLevel()
{
    float light = lightMeter.readLightLevel();
    if (light > 0)
        return (uint)light;
    else
        return uint(0);
}

/// @brief Declare pins and initialize their values
void IOBegin()
{
    // BH1750 light meter
    Wire.begin();
    // pins are already set up through SCL & SDA
    lightMeter.begin();
    // startup onboard LED code
    builtinLED.begin();
    // startup the water pump code
    waterer.begin();
    // setup pins
    pinMode(SOIL_SENSOR_VCC, OUTPUT);
    pinMode(SOIL_SENSOR_GND, OUTPUT);
    pinMode(SOIL_SENSOR_SIG, ANALOG);
    // initalize pin values
    digitalWrite(SOIL_SENSOR_VCC, HIGH);
    digitalWrite(SOIL_SENSOR_GND, LOW);
}
#endif
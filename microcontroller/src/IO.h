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
 * @brief Read the soil moisture
 *
 * @return mV reading of soil moisture sensor
 */
uint readSoilMoisture()
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
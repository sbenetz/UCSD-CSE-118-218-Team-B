#ifndef IO_H
#define IO_H
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include <limits>

#define WATER_PUMP_GND 14
#define WATER_PUMP_SIG 27
#define SOIL_SENSOR_VCC 25
#define SOIL_SENSOR_GND 26
#define SOIL_SENSOR_SIG 33

int SLEEP_FOR = 60;
unsigned long long uS_TO_S_FACTOR = 1000000; // Conversion microsecs to secs

/// Controller for the built in LED for Lolin D32 board
class OnboardLED
{
public:
    bool ON;
    uint8_t count_;
    uint8_t max_;
    uint32_t period_;
    long next_toggle_;
    void begin()
    {
        ON = false;
        pinMode(BUILTIN_LED, OUTPUT);
        digitalWrite(BUILTIN_LED, !ON);
    }
    // HIGH = OFF, LOW = ON
    void toggleLED()
    {
        digitalWrite(BUILTIN_LED, !ON); // write inversed state back
        ON = !ON;
    }
    void doBlink()
    {
        if (count_ < max_)
        {
            if (millis() > next_toggle_)
            {
                toggleLED();
                next_toggle_ += period_;
                if (ON && max_ != std::numeric_limits<uint8_t>::max())
                    count_++;
                return;
            }
        }
    }
    // default blink once per second infinitely
    void setBlinkOnboardLED(uint8_t times = std::numeric_limits<uint8_t>::max(), uint32_t period = 500 /*ms*/)
    {
        next_toggle_ = millis() + period;
        period_ = period;
        max_ = times;
        count_ = 0;
    }
};

OnboardLED builtinLED;

class Waterer
{
public:
    bool on_ = false;
    uint32_t period_;
    long next_check_;
    int goal_moisture_;
    uint16_t attempts_;
    uint16_t max_attempts_;
    void begin()
    {
        pinMode(WATER_PUMP_GND, OUTPUT);
        pinMode(WATER_PUMP_SIG, OUTPUT);
        digitalWrite(WATER_PUMP_GND, LOW);
        digitalWrite(WATER_PUMP_SIG, LOW);
    }
    bool wateringOn()
    {
        return on_;
    }
    /**
     * @brief Turn on the water pump
     *
     * @param time_ms how long to run pump for (ms)
     */
    void waterPumpOn(uint time_ms)
    {
        digitalWrite(WATER_PUMP_SIG, HIGH);
        delay(time_ms);
        digitalWrite(WATER_PUMP_SIG, LOW);
    }

    void handleWatering(uint32_t current_moisture)
    {
        if (on_)
        {
            long now = millis();
            if (now > next_check_)
            {
                if ((int)current_moisture < goal_moisture_)
                {
                    // 3s max
                    uint time = 300 * (goal_moisture_ - current_moisture);
                    Serial.printf("Soil Moisture(%): %d < Watering Goal(%): %d\n", current_moisture, goal_moisture_);
                    Serial.printf("Watering for %d ms\n", time);
                    builtinLED.setBlinkOnboardLED(3);
                    waterPumpOn(time);
                    if (++attempts_ >= max_attempts_)
                    {
                        stopWatering();
                    }
                }
                else
                {
                    stopWatering();
                    Serial.printf("No watering necessary at this time. Sleeping for %d seconds\n", SLEEP_FOR);
                    esp_sleep_enable_timer_wakeup(SLEEP_FOR * uS_TO_S_FACTOR);
                    Serial.flush();
                    esp_deep_sleep_start();
                }
                next_check_ = now + period_;
            };
        }
    }

    void startWatering(int goal_moisture, uint32_t period = 20000 /*ms*/, uint16_t max_attempts = 10)
    {
        attempts_ = 0;
        on_ = true;
        goal_moisture_ = goal_moisture;
        next_check_ = millis();
        period_ = period;
        max_attempts_ = max_attempts;
    }
    void stopWatering()
    {
        goal_moisture_ = -1;
        on_ = false;
    }
};
Waterer waterer;
BH1750 lightMeter;

/**
 * @brief Read the soil moisture
 *
 * @return mV reading of soil moisture sensor
 */
uint32_t readSoilMoisture()
{
    return analogReadMilliVolts(SOIL_SENSOR_SIG);
}

/**
 * @brief Read light level from sensor
 * @return Light level in lux (0 ~ 54612)
 * 	   0 : no valid return value
 */
uint32_t readLightLevel()
{
    float light = lightMeter.readLightLevel();
    if (light > 0)
        return (uint32_t)light;
    else
        return uint32_t(0);
}

/**
 * @brief Declare pins and initialize their values
 *
 */
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
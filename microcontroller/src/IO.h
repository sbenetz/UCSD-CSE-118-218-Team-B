#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include <limits>

#define WATER_PUMP_VCC 14
#define WATER_PUMP_GND 12
#define WATER_PUMP_SIG 27
#define SOIL_SENSOR_VCC 25
#define SOIL_SENSOR_GND 26
#define SOIL_SENSOR_SIG 33

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

class Waterer
{
public:
    bool on_ = false;
    uint32_t period_;
    long next_check_;
    int goal_moisture_;
    void begin()
    {
        pinMode(WATER_PUMP_GND, OUTPUT);
        pinMode(WATER_PUMP_VCC, OUTPUT);
        pinMode(WATER_PUMP_SIG, OUTPUT);
        digitalWrite(WATER_PUMP_GND, LOW);
        digitalWrite(WATER_PUMP_VCC, LOW);
        digitalWrite(WATER_PUMP_SIG, LOW);
    }
    /**
     * @brief Turn on the water pump
     *
     * @param time_ms how long to run pump for (ms)
     */
    void waterPumpOn(uint time_ms)
    {
        // digitalWrite(WATER_PUMP_VCC, HIGH);
        digitalWrite(WATER_PUMP_SIG, HIGH);
        delay(time_ms);
        // digitalWrite(WATER_PUMP_VCC, LOW);
        digitalWrite(WATER_PUMP_SIG, LOW);
    }

    void water(int current_moisture)
    {
        if (on_)
        {
            long now = millis();
            if (now > next_check_)
            {
                if (current_moisture < goal_moisture_)
                {
                    // waterPumpOn(1000);
                }
                next_check_ = now + period_;
                return;
            }
        }
    }

    void startWatering(int goal_moisture, uint32_t period = 20000 /*ms*/)
    {
        on_ = goal_moisture > 0;
        goal_moisture_ = goal_moisture;
        next_check_ = millis() + period;
        period_ = period;
    }
    void stopWatering()
    {
        goal_moisture_ = -1;
        on_ = false;
    }
};
BH1750 lightMeter;
OnboardLED builtinLED;
Waterer waterer;

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
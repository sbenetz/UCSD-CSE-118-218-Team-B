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

BH1750 lightMeter;
OnboardLED builtinLED;

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
    // setup pins
    pinMode(SOIL_SENSOR_VCC, OUTPUT);
    pinMode(SOIL_SENSOR_GND, OUTPUT);
    pinMode(SOIL_SENSOR_SIG, ANALOG);
    pinMode(WATER_PUMP_GND, OUTPUT);
    pinMode(WATER_PUMP_VCC, OUTPUT);
    pinMode(WATER_PUMP_SIG, OUTPUT);
    // initalize pin values
    digitalWrite(WATER_PUMP_GND, LOW);
    digitalWrite(WATER_PUMP_VCC, LOW);
    digitalWrite(WATER_PUMP_SIG, LOW);
    digitalWrite(SOIL_SENSOR_VCC, HIGH);
    digitalWrite(SOIL_SENSOR_GND, LOW);
}
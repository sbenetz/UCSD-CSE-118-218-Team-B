#include <Arduino.h>
#include <limits>

#ifndef BUILTIN_LED
#define BUILTIN_LED 5
#endif
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
                Serial.printf("Millis: %f, Next: %f\n", millis(), next_toggle_);
                toggleLED();
                next_toggle_ += (long)period_;
                if (!ON && max_ != std::numeric_limits<uint8_t>::max())
                    count_++;
            }
            Serial.printf("Count: %d, Max: %d, Next: %f\n", count_, max_, next_toggle_);
            return;
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

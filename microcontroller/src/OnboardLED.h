#ifndef ONBOARD_LED_H
#define ONBOARD_LED_H
#include <Arduino.h>
/// Controller for the built in LED for Lolin D32 board
class OnboardLED
{
public:
    bool ON;
    uint8_t count_;
    uint8_t max_;
    uint period_;
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
    void setBlinkOnboardLED(uint8_t times = std::numeric_limits<uint8_t>::max(), uint period = 500 /*ms*/)
    {
        next_toggle_ = millis() + period;
        period_ = period;
        max_ = times;
        count_ = 0;
    }
};
#endif
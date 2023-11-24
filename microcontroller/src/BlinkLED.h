#include <Arduino.h>
#include <limits>
class OnboardLED {
    public:
    bool ON;
    uint8_t count_;
    uint8_t max_;
    uint32_t period_;
    long next_toggle_;
    void begin(){
        ON = false;
        pinMode(5,OUTPUT);
        digitalWrite(5,LOW);
    }
    void toggleLED() {
        digitalWrite(5, !ON); // write inversed state back
        ON = !ON;
    }
    void doBlink(){
        if (count_< max_ || max_ == std::numeric_limits<uint8_t>::max()){
            if (millis() > next_toggle_){
                toggleLED();
                next_toggle_ += period_;
                count_++;
            }
        }
    }
    // default blink once per second infinitely
    void setBlinkOnboardLED(uint8_t times = std::numeric_limits<uint8_t>::max(), uint32_t period = 500 /*ms*/){
        next_toggle_ = millis() + period;
        period_ = period;
        max_ = times * 2;
        count_ = 0;
    }
};

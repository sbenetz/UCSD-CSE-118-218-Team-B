#ifndef WATERER_H
#define WATERER_H
#define WATER_PUMP_GND 14
#define WATER_PUMP_SIG 27
#include <Arduino.h>

unsigned long long uS_TO_S_FACTOR = 1000000; // Conversion microsecs to secs

class Waterer
{
public:
    bool on_ = false;
    uint period_;
    long next_check_;
    int goal_moisture_;
    uint16_t attempts_;
    uint16_t max_attempts_;
    uint sleep_time_;
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

    void handleWatering(uint current_moisture)
    {
        if (on_)
        {
            long now = millis();
            if (now > next_check_)
            {
                if ((int)current_moisture < goal_moisture_)
                {
                    // 3s max, .75s min (still need to tune)
                    uint time = max(30 * ((uint)goal_moisture_ - current_moisture), (uint)750);
                    Serial.printf("Soil Moisture(%): %d < Watering Goal(%): %d\n", current_moisture, goal_moisture_);
                    Serial.printf("Watering for %d ms\n", time);
                    waterPumpOn(time);
                    if (++attempts_ > max_attempts_)
                    {
                        Serial.printf("Reached max attempts to reach watering goal (%d%%). Stoping attempts.\n", goal_moisture_);
                        stopWatering();
                    }
                }
                else
                {
                    stopWatering();

                    Serial.println("No watering necessary at this time.");
                    if (sleep_time_ > 0)
                    {
                        Serial.printf("Sleeping for %d seconds\n", sleep_time_);
                        esp_sleep_enable_timer_wakeup(sleep_time_ * uS_TO_S_FACTOR);
                        Serial.flush();
                        esp_deep_sleep_start();
                    }
                }
                next_check_ = now + period_;
            };
        }
    }

    void startWatering(int goal_moisture, uint sleep_time, uint period = 20000 /*ms*/, uint16_t max_attempts = 10)
    {
        Serial.printf("Watering until %d %%. Sleeping for %d seconds after.\n", goal_moisture, sleep_time);
        sleep_time_ = sleep_time;
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
#endif
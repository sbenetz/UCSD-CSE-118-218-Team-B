
#include "RFComms.h"
#include "IO.h"

#define MAX_UP_TIME 120000 // 2 mins
long max_setup_time;

uint32_t soil_moisture_max = 2750;
uint32_t soil_moisture_min = 1350;

#define ARRAY_SIZE 50
uint32_t sunlightReadingHistory[ARRAY_SIZE]; // 0-~60000(lumens)
uint32_t soilReadingHistory[ARRAY_SIZE];     // 0-100(%)

uint32_t getAverage(uint32_t array[ARRAY_SIZE])
{
  uint64_t sum = 0;
  for (uint8_t j = 0; j < ARRAY_SIZE; j++)
    sum += array[j];
  return (uint32_t)(sum / ARRAY_SIZE);
}

void setup()
{
  Serial.begin(115200);
  max_setup_time = millis() + MAX_UP_TIME;
  IOBegin();
  builtinLED.setBlinkOnboardLED(2, 200);
  beginRFServices();
}

void loop()
{
  static uint8_t i = 0;
  inLoop();
  if (isConnected && setup_stage == SETUP_DONE)
  {

    if (i < ARRAY_SIZE)
    {
      soilReadingHistory[i] = (uint8_t)((double)(soil_moisture_max - readSoilMoisture()) / (double)(soil_moisture_max - soil_moisture_min) * 100.0);
      sunlightReadingHistory[i] = readLightLevel();
      i++;
    }
    else
    {
      uint32_t soil_average = getAverage(soilReadingHistory);
      uint32_t light_average = getAverage(sunlightReadingHistory);
      uint8_t battery_level = batteryPercent();
      Serial.printf("Soil Moisture (%%): %d, ", soil_average);
      Serial.printf("Soil Moisture (mV): %d, ", readSoilMoisture());
      Serial.printf("Light Level (lx): %d, ", light_average);
      Serial.printf("Battery level (%%): %d\n", battery_level);
      if (!waterer.wateringOn()) // only request new watering level if we don't have one
      {
        int response[2];
        if (postSensorReadings(response, deviceID, soil_average, light_average, battery_level))
        {
          waterer.startWatering(response[0], (uint64_t)response[1]);
        }
      }
      i = 0;
    }
  }
  else if (setup_stage != SETUP_DONE)
  {
    if (millis() > max_setup_time)
    {
      Serial.println("Turning off to save power. Press reset to try setup again");
      Serial.flush();
      esp_deep_sleep_start();
    }
  }
  builtinLED.doBlink(); // run blinks if setup
  waterer.handleWatering(soilReadingHistory[i]);
  delay(200);
}
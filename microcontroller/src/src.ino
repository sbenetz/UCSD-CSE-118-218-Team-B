
#include "RFComms.h"
#include "IO.h"

#define MAX_UP_TIME 120000 // 2 mins
unsigned long max_setup_time;

uint soil_moisture_max = 2750;
uint soil_moisture_min = 1350;

#define ARRAY_SIZE 50
uint sunlightReadingHistory[ARRAY_SIZE]; // 0-~60000(lumens)
uint soilReadingHistory[ARRAY_SIZE];     // 0-100(%)

uint getAverage(uint array[ARRAY_SIZE])
{
  uint64_t sum = 0;
  for (uint j = 0; j < ARRAY_SIZE; j++)
    sum += array[j];
  return (uint)(sum / ARRAY_SIZE);
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
      uint soil_average = getAverage(soilReadingHistory);
      uint light_average = getAverage(sunlightReadingHistory);
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
          if (response[1] >= 0) // no negative sleeps
          {
            waterer.startWatering(response[0], (uint)response[1]);
          }
          else
          {
            Serial.println("Sleep cannot be negative amount of time.");
          }
        }
      }
      memset(soilReadingHistory, 0, sizeof(soilReadingHistory));
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
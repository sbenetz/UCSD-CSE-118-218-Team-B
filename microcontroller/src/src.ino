
#include "HttpComms.h"
#include "RFComms.h"
#include "IO.h"

int watering_goal = -1;
uint32_t soil_moisture_max = 2750;
uint32_t soil_moisture_min = 1350;

int SLEEP_FOR = 60;
unsigned long long uS_TO_S_FACTOR = 1000000; // Conversion microsecs to secs

#define ARRAY_SIZE 50
uint32_t sunlightReadingHistory[ARRAY_SIZE]; // 0-~60000(lumens)
uint32_t soilReadingHistory[ARRAY_SIZE];     // 0-100(%)

void setup()
{
  Serial.begin(115200);
  IOBegin();
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
      Serial.printf("Soil_Moisture (%): %d, ", soil_average);
      Serial.printf("Soil Moisture (mV): %d, ", readSoilMoisture());
      Serial.printf("Light_(lx): %d \n", light_average);
      if (watering_goal == -1) // only request new watering level if you dont have one
      {
        watering_goal = postSensorReadings(deviceID, soil_average, light_average);
      }
      waterer.startWatering(watering_goal);
      i = 0;
    }
  }
  int current_moisture = soilReadingHistory[i];
  if (current_moisture < watering_goal)
  {
    Serial.printf("Soil Moisture(%): %d < Watering Goal(%): %d\n", current_moisture, watering_goal);
    Serial.println("Watering");
    builtinLED.setBlinkOnboardLED(3);
  }
  else
  {
    watering_goal = -1;
    waterer.stopWatering();
    Serial.printf("Watering Complete. Sleeping for %d seconds\n", SLEEP_FOR);
    esp_sleep_enable_timer_wakeup(SLEEP_FOR * uS_TO_S_FACTOR);
    Serial.flush();
    esp_deep_sleep_start();
  }

  builtinLED.doBlink(); // run blinks if setup
  waterer.water(current_moisture);
  delay(200);
}

uint32_t getAverage(uint32_t array[ARRAY_SIZE])
{
  uint64_t sum = 0;
  for (uint8_t j = 0; j < ARRAY_SIZE; j++)
    sum += array[j];
  return (uint32_t)(sum / ARRAY_SIZE);
}
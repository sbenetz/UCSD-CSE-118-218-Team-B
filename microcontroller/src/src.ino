
#include "RFComms.h"
#include "IO.h"

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
      if (waterer.goal_moisture_ == -1) // only request new watering level if we don't have one
      {
        waterer.startWatering(postSensorReadings(deviceID, soil_average, light_average));
      }
      i = 0;
    }
  }
  builtinLED.doBlink(); // run blinks if setup
  waterer.handleWatering(soilReadingHistory[i]);
  delay(200);
}
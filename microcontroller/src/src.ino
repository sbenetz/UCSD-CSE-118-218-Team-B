
#include "HttpComms.h"
#include "RFComms.h"
#include "IO.h"

int watering_level = -1;
uint32_t soil_moisture_max = 2750;
uint32_t soil_moisture_min = 1300;

#define ARRAY_SIZE 10
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
  inLoop();
  static uint8_t i = 0;
  if (i < ARRAY_SIZE)
  {
    soilReadingHistory[i] = (uint8_t)((double)(soil_moisture_max - readSoilMoisture()) / (double)(soil_moisture_max - soil_moisture_min) * 100.0);
    sunlightReadingHistory[i] = (uint32_t)readLightLevel();
    i++;
  }
  else
  {
    uint32_t soil_average = getAverage(soilReadingHistory);
    uint32_t light_average = getAverage(sunlightReadingHistory);
    Serial.printf("Soil_Moisture (%): %d, ", soil_average);
    Serial.printf("Soil Moisture (mV): %d, ", readSoilMoisture());
    Serial.printf("Light_(lx): %d \n", light_average);
    builtinLED.setBlinkOnboardLED(3);
    watering_level = postSensorReadings(deviceID, soil_average, light_average);
    if ((int)soil_average < watering_level)
    {
      Serial.printf("Soil: %d, Watering Level: %d\n", soil_average, watering_level);
      Serial.println("Watering");
      // waterPumpOn(3000);
    }
    i = 0;
  }
  builtinLED.doBlink(); // run blinks if setup
  delay(500);
}

uint32_t getAverage(uint32_t array[ARRAY_SIZE])
{
  uint64_t sum = 0;
  for (uint8_t j = 0; j < ARRAY_SIZE; j++)
    sum += array[j];
  return (uint32_t)(sum / ARRAY_SIZE);
}
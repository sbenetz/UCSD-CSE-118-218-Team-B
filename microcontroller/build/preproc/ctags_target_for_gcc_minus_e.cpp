# 1 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino"
# 2 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino" 2
# 3 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino" 2
# 4 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino" 2

# 6 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino" 2
# 7 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino" 2
# 8 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino" 2
# 9 "C:\\Users\\shane\\OneDrive\\Documents\\UCSD-CSE-118-218-Team-B\\microcontroller\\src\\src.ino" 2

BH1750 lightMeter;
OnboardLED builtinLED;



uint32_t soilReadingHistory[10];
uint32_t sunlightReadingHistory[10];
String deviceID;

int watering_level = -1;
uint32_t soil_moisture_max = 2800;
uint32_t soil_moisture_min = 1000;

void setup()
{
    // start up objects
    Serial.begin(115200);
    // BH1750 light meter
    Wire.begin();
    lightMeter.begin();
    // startup onboard LED code
    builtinLED.begin();
    // begin Bluetooth server and wifi if available
    preferences.putString("pref_ssid", WIFI_SSID);
    preferences.putString("pref_pass", WIFI_PASS);
    beginRFServices();
    deviceID = postNewDevice("Shane","greenie",1);
    // setup pins
    pinMode(25, 0x03); // powers soil moisture sensor
    pinMode(26, 0x03); // grounds soil moisture sensor
    pinMode(33, 0xC0); // reads value from soil moisture sensor
    // initalize pin values
    digitalWrite(25, 0x1);
    digitalWrite(26, 0x0);
}
void loop()
{
    static uint8_t i = 0;
    inLoop();
    if (i < 10)
    {
        soilReadingHistory[i] = (uint8_t)((double)(soil_moisture_max-analogReadMilliVolts(33))/(double)(soil_moisture_max-soil_moisture_min)*100.0);
        sunlightReadingHistory[i] = (uint32_t)lightMeter.readLightLevel();
        i++;
    }
    else
    {
        uint32_t soil_average = getAverage(soilReadingHistory);
        uint32_t light_average = getAverage(sunlightReadingHistory);
        Serial.printf("Soil_Moisture_(mV): %d, ", soil_average);
        Serial.printf("Light_(lx): %d \n", light_average);
        builtinLED.setBlinkOnboardLED(3);
        watering_level = postSensorReadings(deviceID,soil_average,light_average);
        if((int)soil_average < watering_level){
            Serial.println("Watering");
        }
        i = 0;
    }

    builtinLED.doBlink(); // run blinks if setup
    delay(1000);
}

uint32_t getAverage(uint32_t array[10])
{
    uint64_t sum = 0;
    for (uint8_t j = 0; j < 10; j++)
        sum += array[j];
    return (uint32_t)(sum / 10);
}

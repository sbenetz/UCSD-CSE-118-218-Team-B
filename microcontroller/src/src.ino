#include <WiFi.h>
#include <Wire.h>
#include <BH1750.h>
#include "BlinkLED.h"

BH1750 light_meter; 
OnboardLED onBoardLED;
bool onboard_LED_on = true;
void setup() {
    // start up objects
    Serial.begin(115200);
    // BH1750 light meter
    Wire.begin(); 
    light_meter.begin();
    // startup onboard LED code
    onBoardLED.begin();
    onBoardLED.setBlinkOnboardLED();
    // wifi initialization
    WiFi.begin("StellaRosaGuest", "gentlecarrot408");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    // setup pins
    pinMode(5,OUTPUT); // onboard LED
    pinMode(25,OUTPUT); // powers soil moisture sensor
    pinMode(26,OUTPUT); // grounds soil moisture sensor
    pinMode(33,ANALOG); // reads value from soil moisture sensor
    // initalize pin values
    digitalWrite(25,HIGH); 
    digitalWrite(26,LOW); 
}

void loop() {
    
    String mv = String(analogReadMilliVolts(33));
    Serial.print("Soil_Moisture_(mV):"+mv +", " );
    float lux = light_meter.readLightLevel();
    Serial.print("Light_(lx):");
    Serial.print(lux);
    Serial.println();
    onBoardLED.doBlink(); // run blinks if setup
    delay(1000); // Delay for a second, adjust as needed
}
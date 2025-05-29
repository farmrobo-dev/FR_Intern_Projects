#include "FR_Temp_Sensor.h"

#define TEMP1_PIN PC3
#define TEMP2_PIN PC0

#define IS_ONE_WIRE 1

FR_Temp_Sensor temp1_sensor(TEMP1_PIN, IS_ONE_WIRE);
FR_Temp_Sensor temp2_sensor(TEMP2_PIN, IS_ONE_WIRE);

float temp1, temp2; 

void setup() {
    Serial.begin(115200);
    analogWriteFrequency(490);
    analogWriteResolution(8);
    analogReadResolution(10);
}

void loop() {
    temp1_sensor.readTemperature(temp1);
    temp2_sensor.readTemperature(temp2);
    Serial.print("Temperature 1: ");
    Serial.print(temp1);
    Serial.print(" °C, Temperature 2: ");
    Serial.print(temp2);
    Serial.println(" °C");
    delay(1000);
}
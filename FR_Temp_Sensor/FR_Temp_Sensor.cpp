#include "FR_Temp_Sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Constructor
FR_Temp_Sensor::FR_Temp_Sensor(uint8_t tPin, bool isOneWire, float knownResistor, float vcc)
    : tempPin(tPin), isOneWire(isOneWire), knownResistor(knownResistor), vcc(vcc), lastTempRequestTime(0) {
    if (isOneWire) {
        tempSensor = new OneWire(tempPin);
        dallasSensor = new DallasTemperature(tempSensor);
        dallasSensor->begin();
        dallasSensor->setWaitForConversion(false);
    } else {
        tempSensor = nullptr;
        dallasSensor = nullptr;
    }
}

// Destructor
FR_Temp_Sensor::~FR_Temp_Sensor() {
    if (isOneWire) {
        delete dallasSensor;
        delete tempSensor;
    }
}

// Calculate temperature from resistance
float FR_Temp_Sensor::calculateTemperature(float resistance) {
    for (int i = 0; i < 100; i++) {
        if (resistance >= resistanceValues[i] && resistance < resistanceValues[i + 1]) {
            float slope = (temperatureValues[i + 1] - temperatureValues[i]) /
                          (resistanceValues[i + 1] - resistanceValues[i]);
            return temperatureValues[i] + slope * (resistance - resistanceValues[i]);
        }
    }
    return -127.0; // Error value
}

// Read temperature
void FR_Temp_Sensor::readTemperature(float &temp) {
    if (!isOneWire) {
        int tSensorValue = analogRead(tempPin); // Analog sensor value
        if (tSensorValue == 0 || tSensorValue >= 1023) {
            temp = -127.0; // Error value
            return;
        }

        float t1Vout = tSensorValue * (vcc / 1023.0); // Calculate voltage
        float resistanceT = knownResistor / ((vcc / t1Vout) - 1); // Calculate resistance
        temp = calculateTemperature(resistanceT);
    } else {
        if (millis() - lastTempRequestTime >= 750) {
            float readTemp = dallasSensor->getTempCByIndex(0); // Read temperature in Celsius
            if (readTemp != DEVICE_DISCONNECTED_C && readTemp != -127.0) {
                temp = readTemp;
            }
            dallasSensor->requestTemperatures(); // Request new temperature
            lastTempRequestTime = millis(); // Update last request time
        }
    }
}

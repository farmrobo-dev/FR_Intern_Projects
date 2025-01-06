#ifndef FR_TEMP_SENSOR_H
#define FR_TEMP_SENSOR_H

#include <Arduino.h>
#include "Filter.h"

class FR_Temp_Sensor {
private:
    int tempPin;
    float knownResistor;
    float vcc;

    ExponentialFilter<long> tempFilter;

    float calculateTemperature(float resistance);
    
public:
    FR_Temp_Sensor(int tPin, float knownResistor = 100, float vcc = 3.3);
    void readTemperature(float &temp);
};

#endif // FR_TEMP_SENSOR_H

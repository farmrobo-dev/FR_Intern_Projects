#ifndef FR_TEMP_SENSOR_H
#define FR_TEMP_SENSOR_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class FR_Temp_Sensor {
public:
    // Constructor
    FR_Temp_Sensor(uint8_t tPin, bool isOneWire = true, float knownResistor = 100.0, float vcc = 3.3);

    // Destructor
    ~FR_Temp_Sensor();

    // Reads temperature
    void readTemperature(float &temp);

private:
    uint8_t tempPin;          // Pin connected to the sensor
    bool isOneWire;           // Indicates if it's a OneWire sensor
    float knownResistor;      // Known resistor value (for analog sensors)
    float vcc;                // Supply voltage
    unsigned long lastTempRequestTime; // Tracks the last request time for DallasTemperature

    OneWire *tempSensor;      // Pointer to OneWire instance
    DallasTemperature *dallasSensor; // Pointer to DallasTemperature instance

    // Calculates temperature from resistance (for analog sensors)
    float calculateTemperature(float resistance);

    // Lookup tables for resistance and temperature
    static constexpr float temperatureValues[101] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100
    };

    static constexpr float resistanceValues[101] = {
        100.00, 100.39, 100.78, 101.17, 101.56, 101.95, 102.34, 102.73, 103.12, 103.51,
        103.90, 104.29, 104.68, 105.07, 105.46, 105.85, 106.24, 106.63, 107.02, 107.40,
        107.79, 108.18, 108.57, 108.96, 109.35, 109.73, 110.12, 110.51, 110.90, 111.29,
        111.67, 112.06, 112.45, 112.83, 113.22, 113.61, 114.00, 114.38, 114.77, 115.15,
        115.54, 115.92, 116.31, 116.70, 117.08, 117.47, 117.86, 118.24, 118.63, 119.01,
        119.40, 119.78, 120.17, 120.55, 120.94, 121.32, 121.71, 122.09, 122.47, 122.86,
        123.24, 123.63, 124.01, 124.39, 124.78, 125.16, 125.54, 125.93, 126.31, 126.69,
        127.08, 127.46, 127.84, 128.22, 128.61, 128.99, 129.37, 129.75, 130.13, 130.52,
        130.90, 131.28, 131.66, 132.04, 132.42, 132.80, 133.18, 133.56, 133.95, 134.33,
        134.71, 135.09, 135.47, 135.85, 136.23, 136.61, 136.99, 137.37, 137.75, 138.13, 138.51
    };
};

#endif // FR_TEMP_SENSOR_H

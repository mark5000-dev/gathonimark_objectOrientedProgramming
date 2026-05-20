#include "sensor.h"

// Base Sensor Constructor
Sensor::Sensor(string id, float min, float max)
    : sensorID(id), minRange(min), maxRange(max), currentValue(0) {
    lastReadTime = time(0);
}

// Moisture Sensor Implementation
MoistureSensor::MoistureSensor(string id)
    : Sensor(id, 0.0f, 100.0f) {
}

float MoistureSensor::readValue() {
    // Simulate reading with significant variations to trigger actuators
    float variation = (rand() % 40 - 20) / 1.0f; // -20 to +20
    currentValue = 40.0f + variation;
    if (currentValue < minRange) currentValue = minRange;
    if (currentValue > maxRange) currentValue = maxRange;
    lastReadTime = time(0);
    return currentValue;
}

// Temperature Sensor Implementation
TemperatureSensor::TemperatureSensor(string id)
    : Sensor(id, -10.0f, 50.0f) {
}

float TemperatureSensor::readValue() {
    // Simulate temperature variations to cross thresholds (15°C and 28°C)
    float variation = (rand() % 160 - 80) / 10.0f; // -8.0 to +8.0
    currentValue = 25.0f + variation;
    if (currentValue < minRange) currentValue = minRange;
    if (currentValue > maxRange) currentValue = maxRange;
    lastReadTime = time(0);
    return currentValue;
}

// Humidity Sensor Implementation
HumiditySensor::HumiditySensor(string id)
    : Sensor(id, 0.0f, 100.0f) {
}

float HumiditySensor::readValue() {
    // Simulate humidity variations
    float variation = (rand() % 40 - 20) / 1.0f; // -20 to +20
    currentValue = 60.0f + variation;
    if (currentValue < minRange) currentValue = minRange;
    if (currentValue > maxRange) currentValue = maxRange;
    lastReadTime = time(0);
    return currentValue;
}

// Light Sensor Implementation
LightSensor::LightSensor(string id)
    : Sensor(id, 0.0f, 10000.0f) {
}

float LightSensor::readValue() {
    // Simulate light intensity variations to trigger grow lights
    float variation = (rand() % 6000 - 3000); // -3000 to +3000
    currentValue = 5000.0f + variation;
    if (currentValue < minRange) currentValue = minRange;
    if (currentValue > maxRange) currentValue = maxRange;
    lastReadTime = time(0);
    return currentValue;
}

// Water Level Sensor Implementation
WaterLevelSensor::WaterLevelSensor(string id)
    : Sensor(id, 0.0f, 100.0f) {
}

float WaterLevelSensor::readValue() {
    // Simulate water level variations
    float variation = (rand() % 50 - 25) / 1.0f; // -25 to +25
    currentValue = 75.0f + variation;
    if (currentValue < minRange) currentValue = minRange;
    if (currentValue > maxRange) currentValue = maxRange;
    lastReadTime = time(0);
    return currentValue;
}

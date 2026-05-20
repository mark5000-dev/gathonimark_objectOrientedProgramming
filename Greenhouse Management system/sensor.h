#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

// Abstract Sensor Base Class
class Sensor {
protected:
    string sensorID;
    float currentValue;
    float minRange;
    float maxRange;
    time_t lastReadTime;

public:
    Sensor(string id, float min, float max);
    virtual ~Sensor() = default;

    // Pure virtual function for reading sensor value
    virtual float readValue() = 0;
    
    virtual string getSensorType() const = 0;
    
    string getSensorID() const { return sensorID; }
    float getCurrentValue() const { return currentValue; }
    time_t getLastReadTime() const { return lastReadTime; }
};

// Moisture Sensor - measures soil moisture percentage
class MoistureSensor : public Sensor {
public:
    MoistureSensor(string id = "MOIST_001");
    float readValue() override;
    string getSensorType() const override { return "MoistureSensor"; }
};

// Temperature Sensor - measures temperature in Celsius
class TemperatureSensor : public Sensor {
public:
    TemperatureSensor(string id = "TEMP_001");
    float readValue() override;
    string getSensorType() const override { return "TemperatureSensor"; }
};

// Humidity Sensor - measures relative humidity percentage
class HumiditySensor : public Sensor {
public:
    HumiditySensor(string id = "HUMID_001");
    float readValue() override;
    string getSensorType() const override { return "HumiditySensor"; }
};

// Light Intensity Sensor - measures light level in lux
class LightSensor : public Sensor {
public:
    LightSensor(string id = "LIGHT_001");
    float readValue() override;
    string getSensorType() const override { return "LightSensor"; }
};

// Water Tank Level Sensor
class WaterLevelSensor : public Sensor {
public:
    WaterLevelSensor(string id = "WATER_001");
    float readValue() override;
    string getSensorType() const override { return "WaterLevelSensor"; }
};

#endif // SENSOR_H

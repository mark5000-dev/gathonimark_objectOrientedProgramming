#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <string>
#include <iostream>

using namespace std;

// Abstract Actuator Base Class
class Actuator {
protected:
    string actuatorID;
    bool isActive;
    int powerLevel; // 0-100

public:
    Actuator(string id);
    virtual ~Actuator() = default;

    // Pure virtual functions
    virtual void activate(int level = 100) = 0;
    virtual void deactivate() = 0;
    virtual string getActuatorType() const = 0;

    string getActuatorID() const { return actuatorID; }
    bool getIsActive() const { return isActive; }
    int getPowerLevel() const { return powerLevel; }
};

// Water Pump - controls irrigation
class WaterPump : public Actuator {
private:
    float waterDispensed; // liters

public:
    WaterPump(string id = "PUMP_001");
    void activate(int level = 100) override;
    void deactivate() override;
    string getActuatorType() const override { return "WaterPump"; }
    float getWaterDispensed() const { return waterDispensed; }
    void resetWaterCounter() { waterDispensed = 0.0f; }
};

// Cooling Fan - maintains temperature
class CoolingFan : public Actuator {
public:
    CoolingFan(string id = "FAN_001");
    void activate(int level = 100) override;
    void deactivate() override;
    string getActuatorType() const override { return "CoolingFan"; }
};

// Heater - maintains minimum temperature
class Heater : public Actuator {
public:
    Heater(string id = "HEAT_001");
    void activate(int level = 100) override;
    void deactivate() override;
    string getActuatorType() const override { return "Heater"; }
};

// Grow Light - provides artificial light
class GrowLight : public Actuator {
public:
    GrowLight(string id = "LIGHT_001");
    void activate(int level = 100) override;
    void deactivate() override;
    string getActuatorType() const override { return "GrowLight"; }
};

// Sprinkler System - provides localized watering
class Sprinkler : public Actuator {
private:
    float waterDispensed;

public:
    Sprinkler(string id = "SPRINK_001");
    void activate(int level = 100) override;
    void deactivate() override;
    string getActuatorType() const override { return "Sprinkler"; }
    float getWaterDispensed() const { return waterDispensed; }
    void resetWaterCounter() { waterDispensed = 0.0f; }
};

#endif // ACTUATOR_H

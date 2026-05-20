#include "actuator.h"

// Base Actuator Constructor
Actuator::Actuator(string id)
    : actuatorID(id), isActive(false), powerLevel(0) {
}

// Water Pump Implementation
WaterPump::WaterPump(string id)
    : Actuator(id), waterDispensed(0.0f) {
}

void WaterPump::activate(int level) {
    isActive = true;
    powerLevel = (level < 0) ? 0 : (level > 100) ? 100 : level;
    waterDispensed += (powerLevel / 100.0f) * 0.5f; // Dispense 0-0.5 liters per cycle
    cout << "[PUMP] " << actuatorID << " activated at " << powerLevel << "% - Dispensed " 
         << (powerLevel / 100.0f) * 0.5f << "L" << endl;
}

void WaterPump::deactivate() {
    isActive = false;
    powerLevel = 0;
    cout << "[PUMP] " << actuatorID << " deactivated" << endl;
}

// Cooling Fan Implementation
CoolingFan::CoolingFan(string id)
    : Actuator(id) {
}

void CoolingFan::activate(int level) {
    isActive = true;
    powerLevel = (level < 0) ? 0 : (level > 100) ? 100 : level;
    cout << "[FAN] " << actuatorID << " activated at " << powerLevel << "% power" << endl;
}

void CoolingFan::deactivate() {
    isActive = false;
    powerLevel = 0;
    cout << "[FAN] " << actuatorID << " deactivated" << endl;
}

// Heater Implementation
Heater::Heater(string id)
    : Actuator(id) {
}

void Heater::activate(int level) {
    isActive = true;
    powerLevel = (level < 0) ? 0 : (level > 100) ? 100 : level;
    cout << "[HEATER] " << actuatorID << " activated at " << powerLevel << "% power" << endl;
}

void Heater::deactivate() {
    isActive = false;
    powerLevel = 0;
    cout << "[HEATER] " << actuatorID << " deactivated" << endl;
}

// Grow Light Implementation
GrowLight::GrowLight(string id)
    : Actuator(id) {
}

void GrowLight::activate(int level) {
    isActive = true;
    powerLevel = (level < 0) ? 0 : (level > 100) ? 100 : level;
    cout << "[LIGHT] " << actuatorID << " activated at " << powerLevel << "% intensity" << endl;
}

void GrowLight::deactivate() {
    isActive = false;
    powerLevel = 0;
    cout << "[LIGHT] " << actuatorID << " deactivated" << endl;
}

// Sprinkler Implementation
Sprinkler::Sprinkler(string id)
    : Actuator(id), waterDispensed(0.0f) {
}

void Sprinkler::activate(int level) {
    isActive = true;
    powerLevel = (level < 0) ? 0 : (level > 100) ? 100 : level;
    waterDispensed += (powerLevel / 100.0f) * 0.3f; // Dispense 0-0.3 liters per cycle
    cout << "[SPRINKLER] " << actuatorID << " activated at " << powerLevel << "% - Dispensed " 
         << (powerLevel / 100.0f) * 0.3f << "L" << endl;
}

void Sprinkler::deactivate() {
    isActive = false;
    powerLevel = 0;
    cout << "[SPRINKLER] " << actuatorID << " deactivated" << endl;
}

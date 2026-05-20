#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include <vector>
#include <map>
#include <queue>
#include <iostream>
#include "sensor.h"
#include "actuator.h"
#include "plant.h"
#include "logger.h"

using namespace std;

// Alert structure for alert queue
struct Alert {
    string message;
    string severity; // INFO, WARNING, CRITICAL
    time_t timestamp;

    Alert(string msg, string sev)
        : message(msg), severity(sev), timestamp(time(0)) {}
};

// Main Greenhouse Controller - demonstrates composition
class Greenhouse {
private:
    string greenhouseID;
    vector<Sensor*> sensors;
    vector<Actuator*> actuators;
    vector<Plant*> plants;
    queue<Alert> alertQueue;
    Logger logger;
    int simulationCycle;
    float totalWaterUsed;

    // Control thresholds
    float moistureThreshold;
    float temperatureMaxThreshold;
    float temperatureMinThreshold;
    float lightThreshold;

public:
    Greenhouse(string id = "GH_001");
    ~Greenhouse();

    // Add components
    void addSensor(Sensor* sensor);
    void addActuator(Actuator* actuator);
    void addPlant(Plant* plant);

    // Core simulation functions
    void readAllSensors();
    void updatePlantConditions();
    void makeAutomatedDecisions();
    void logCycleData();

    // Automation control
    void controlWaterPump(WaterPump* pump, float moisture, float waterLevel);
    void controlTemperature(float currentTemp);
    void controlLighting(float lightIntensity);

    // Alert management
    void generateAlert(const string& message, const string& severity = "INFO");
    void processAlertQueue();

    // Status reporting
    void printGreenhouseStatus();
    void printSensorReadings();
    void printPlantStatus();
    void printActuatorStatus();

    // Simulation control
    void runSimulationCycle();
    int getSimulationCycle() const { return simulationCycle; }

    // Getters
    string getGreenhouseID() const { return greenhouseID; }
    float getTotalWaterUsed() const { return totalWaterUsed; }
};

#endif // GREENHOUSE_H

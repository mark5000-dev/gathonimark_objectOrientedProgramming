#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include "sensor.h"
#include "actuator.h"
#include "plant.h"

using namespace std;

// CSV Logger for data persistence
class Logger {
private:
    string sensorLogFile;
    string actuatorLogFile;
    string plantLogFile;
    string waterUsageFile;
    string alertLogFile;

    string getTimestamp();
    string doubleToString(float value);

public:
    Logger();

    // Log sensor readings
    void logSensorReading(const Sensor* sensor);
    void logAllSensorReadings(const vector<Sensor* >& sensors);

    // Log actuator events
    void logActuatorEvent(const Actuator* actuator, const string& event);
    void logAllActuatorEvents(const vector<Actuator*>& actuators, const string& event);

    // Log plant status
    void logPlantStatus(const Plant* plant);
    void logAllPlantsStatus(const vector<Plant*>& plants);

    // Log water usage
    void logWaterUsage(float totalDispensed, const string& source);

    // Log alerts
    void logAlert(const string& alertMessage, const string& severity = "INFO");

    // Initialize CSV files with headers
    void initializeFiles();

    // Get file paths
    string getSensorLogFile() const { return sensorLogFile; }
    string getActuatorLogFile() const { return actuatorLogFile; }
    string getPlantLogFile() const { return plantLogFile; }
    string getWaterUsageFile() const { return waterUsageFile; }
    string getAlertLogFile() const { return alertLogFile; }
};

#endif // LOGGER_H

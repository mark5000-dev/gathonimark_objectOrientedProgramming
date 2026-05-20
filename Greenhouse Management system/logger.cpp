#include "logger.h"
#include <iostream>
#include <iomanip>
#include "plant.h"
#include "actuator.h"

Logger::Logger()
    : sensorLogFile("sensor_readings.csv"),
      actuatorLogFile("actuator_events.csv"),
      plantLogFile("plant_status.csv"),
      waterUsageFile("water_usage.csv"),
      alertLogFile("alerts.csv") {
    initializeFiles();
}

string Logger::getTimestamp() {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    stringstream ss;
    ss << put_time(timeinfo, "%Y-%m-%d|%H:%M:%S");
    return ss.str();
}

string Logger::doubleToString(float value) {
    stringstream ss;
    ss << fixed << setprecision(2) << value;
    return ss.str();
}

void Logger::initializeFiles() {
    // Initialize sensor log
    ofstream sensorFile(sensorLogFile);
    sensorFile << "Timestamp|SensorID|SensorType|Value|Unit|MinRange|MaxRange" << endl;
    sensorFile.close();

    // Initialize actuator log
    ofstream actuatorFile(actuatorLogFile);
    actuatorFile << "Timestamp|ActuatorID|ActuatorType|Event|PowerLevel" << endl;
    actuatorFile.close();

    // Initialize plant log
    ofstream plantFile(plantLogFile);
    plantFile << "Timestamp|PlantID|PlantName|PlantType|Moisture|Temperature|DaysGrowing|IsHealthy" << endl;
    plantFile.close();

    // Initialize water usage log
    ofstream waterFile(waterUsageFile);
    waterFile << "Timestamp|TotalLitersDispensed|Source" << endl;
    waterFile.close();

    // Initialize alert log
    ofstream alertFile(alertLogFile);
    alertFile << "Timestamp|Severity|Message" << endl;
    alertFile.close();
}

void Logger::logSensorReading(const Sensor* sensor) {
    ofstream file(sensorLogFile, ios::app);
    if (file.is_open()) {
        file << getTimestamp() << "|"
             << sensor->getSensorID() << "|"
             << sensor->getSensorType() << "|"
             << doubleToString(sensor->getCurrentValue()) << "|";

        // Add unit based on sensor type
        string type = sensor->getSensorType();
        if (type == "TemperatureSensor") file << "Celsius";
        else if (type == "MoistureSensor") file << "%";
        else if (type == "HumiditySensor") file << "%";
        else if (type == "LightSensor") file << "Lux";
        else if (type == "WaterLevelSensor") file << "%";
        else file << "Unit";

        file << "|" << "0" << "|" << "100" << endl;
        file.close();
    }
}

void Logger::logAllSensorReadings(const vector<Sensor*>& sensors) {
    for (const auto sensor : sensors) {
        logSensorReading(sensor);
    }
}

void Logger::logActuatorEvent(const Actuator* actuator, const string& event) {
    ofstream file(actuatorLogFile, ios::app);
    if (file.is_open()) {
        file << getTimestamp() << "|"
             << actuator->getActuatorID() << "|"
             << actuator->getActuatorType() << "|"
             << event << "|"
             << actuator->getPowerLevel() << endl;
        file.close();
    }
}

void Logger::logAllActuatorEvents(const vector<Actuator*>& actuators, const string& event) {
    for (const auto actuator : actuators) {
        logActuatorEvent(actuator, event);
    }
}

void Logger::logPlantStatus(const Plant* plant) {
    ofstream file(plantLogFile, ios::app);
    if (file.is_open()) {
        file << getTimestamp() << "|"
             << plant->getPlantID() << "|"
             << plant->getPlantName() << "|"
             << plant->getPlantType() << "|"
             << doubleToString(plant->getCurrentMoisture()) << "|"
             << doubleToString(plant->getCurrentTemperature()) << "|"
             << plant->getDaysGrowing() << "|"
             << (plant->getIsHealthy() ? "Healthy" : "Unhealthy") << endl;
        file.close();
    }
}

void Logger::logAllPlantsStatus(const vector<Plant*>& plants) {
    for (const auto plant : plants) {
        logPlantStatus(plant);
    }
}

void Logger::logWaterUsage(float totalDispensed, const string& source) {
    ofstream file(waterUsageFile, ios::app);
    if (file.is_open()) {
        file << getTimestamp() << "|"
             << doubleToString(totalDispensed) << "|"
             << source << endl;
        file.close();
    }
}

void Logger::logAlert(const string& alertMessage, const string& severity) {
    ofstream file(alertLogFile, ios::app);
    if (file.is_open()) {
        file << getTimestamp() << "|"
             << severity << "|"
             << alertMessage << endl;
        file.close();
    }
    cout << "[ALERT|" << severity << "] " << alertMessage << endl;
}

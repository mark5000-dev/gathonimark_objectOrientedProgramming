#include "greenhouse.h"
#include <iostream>
#include <iomanip>

Greenhouse::Greenhouse(string id)
    : greenhouseID(id), simulationCycle(0), totalWaterUsed(0),
      moistureThreshold(40.0f), temperatureMaxThreshold(28.0f),
      temperatureMinThreshold(15.0f), lightThreshold(3000.0f) {
}

Greenhouse::~Greenhouse() {
    // Clean up allocated memory
    for (auto sensor : sensors) delete sensor;
    for (auto actuator : actuators) delete actuator;
    for (auto plant : plants) delete plant;
}

void Greenhouse::addSensor(Sensor* sensor) {
    if (sensor) {
        sensors.push_back(sensor);
    }
}

void Greenhouse::addActuator(Actuator* actuator) {
    if (actuator) {
        actuators.push_back(actuator);
    }
}

void Greenhouse::addPlant(Plant* plant) {
    if (plant) {
        plants.push_back(plant);
    }
}

void Greenhouse::readAllSensors() {
    cout << "\n=== Reading All Sensors ===" << endl;
    for (auto sensor : sensors) {
        float value = sensor->readValue();
        cout << sensor->getSensorType() << " (" << sensor->getSensorID()
             << "): " << fixed << setprecision(2) << value << endl;
        logger.logSensorReading(sensor);
    }
}

void Greenhouse::updatePlantConditions() {
    if (sensors.size() >= 3) {
        float moisture = 0, temperature = 0, light = 0;

        // Extract sensor values
        for (auto sensor : sensors) {
            string type = sensor->getSensorType();
            if (type == "MoistureSensor") moisture = sensor->getCurrentValue();
            else if (type == "TemperatureSensor") temperature = sensor->getCurrentValue();
            else if (type == "LightSensor") light = sensor->getCurrentValue();
        }

        // Update all plants
        for (auto plant : plants) {
            plant->updateConditions(moisture, temperature, light);
            plant->incrementDaysGrowing();
        }
    }
}

void Greenhouse::controlWaterPump(WaterPump* pump, float moisture, float waterLevel) {
    if (moisture < moistureThreshold && waterLevel > 20.0f) {
        pump->activate(80);
        generateAlert("Water pump activated - low soil moisture", "INFO");
    } else if (pump->getIsActive()) {
        pump->deactivate();
        generateAlert("Water pump deactivated", "INFO");
    }
}

void Greenhouse::controlTemperature(float currentTemp) {
    CoolingFan* fan = nullptr;
    Heater* heater = nullptr;

    // Find temperature control actuators
    for (auto actuator : actuators) {
        if (actuator->getActuatorType() == "CoolingFan") fan = dynamic_cast<CoolingFan*>(actuator);
        if (actuator->getActuatorType() == "Heater") heater = dynamic_cast<Heater*>(actuator);
    }

    if (currentTemp > temperatureMaxThreshold && fan) {
        fan->activate(100);
        generateAlert("Cooling fan activated - temperature too high", "WARNING");
    } else if (fan && fan->getIsActive()) {
        fan->deactivate();
    }

    if (currentTemp < temperatureMinThreshold && heater) {
        heater->activate(100);
        generateAlert("Heater activated - temperature too low", "WARNING");
    } else if (heater && heater->getIsActive()) {
        heater->deactivate();
    }
}

void Greenhouse::controlLighting(float lightIntensity) {
    GrowLight* light = nullptr;

    for (auto actuator : actuators) {
        if (actuator->getActuatorType() == "GrowLight") {
            light = dynamic_cast<GrowLight*>(actuator);
        }
    }

    if (lightIntensity < lightThreshold && light) {
        light->activate(100);
        generateAlert("Grow lights activated - insufficient natural light", "INFO");
    } else if (light && light->getIsActive()) {
        light->deactivate();
    }
}

void Greenhouse::makeAutomatedDecisions() {
    cout << "\n=== Making Automated Decisions ===" << endl;

    if (sensors.empty()) {
        generateAlert("No sensors available for decision making", "CRITICAL");
        return;
    }

    float moisture = 0, temperature = 0, light = 0, waterLevel = 0;

    // Extract current sensor values
    for (auto sensor : sensors) {
        string type = sensor->getSensorType();
        if (type == "MoistureSensor") moisture = sensor->getCurrentValue();
        else if (type == "TemperatureSensor") temperature = sensor->getCurrentValue();
        else if (type == "LightSensor") light = sensor->getCurrentValue();
        else if (type == "WaterLevelSensor") waterLevel = sensor->getCurrentValue();
    }

    // Water control
    WaterPump* pump = nullptr;
    for (auto actuator : actuators) {
        if (actuator->getActuatorType() == "WaterPump") {
            pump = dynamic_cast<WaterPump*>(actuator);
            break;
        }
    }

    if (pump) {
        controlWaterPump(pump, moisture, waterLevel);
        totalWaterUsed += pump->getWaterDispensed();
    }

    // Temperature control
    controlTemperature(temperature);

    // Lighting control
    controlLighting(light);

    // Check plant health
    for (auto plant : plants) {
        if (!plant->getIsHealthy()) {
            generateAlert("Plant " + plant->getPlantID() + " is unhealthy - check conditions",
                         "WARNING");
        }
    }
}

void Greenhouse::generateAlert(const string& message, const string& severity) {
    Alert alert(message, severity);
    alertQueue.push(alert);
    logger.logAlert(message, severity);
}

void Greenhouse::processAlertQueue() {
    while (!alertQueue.empty()) {
        Alert alert = alertQueue.front();
        alertQueue.pop();
        // Alerts are logged in generateAlert
    }
}

void Greenhouse::printGreenhouseStatus() {
    cout << "\n" << string(60, '=') << endl;
    cout << "GREENHOUSE SIMULATION STATUS" << endl;
    cout << "ID: " << greenhouseID << " | Cycle: " << simulationCycle << endl;
    cout << "Total Water Used: " << fixed << setprecision(2) << totalWaterUsed << " Liters" << endl;
    cout << string(60, '=') << endl;
}

void Greenhouse::printSensorReadings() {
    cout << "\n--- SENSOR READINGS ---" << endl;
    cout << left << setw(20) << "Sensor" << setw(15) << "Value" << "Status" << endl;
    cout << string(50, '-') << endl;

    for (auto sensor : sensors) {
        cout << left << setw(20) << sensor->getSensorType()
             << setw(15) << fixed << setprecision(2) << sensor->getCurrentValue()
             << "OK" << endl;
    }
}

void Greenhouse::printPlantStatus() {
    cout << "\n--- PLANT STATUS ---" << endl;
    cout << left << setw(15) << "Plant" << setw(15) << "Type"
         << setw(12) << "Moisture" << setw(12) << "Temp" << "Health" << endl;
    cout << string(70, '-') << endl;

    for (auto plant : plants) {
        cout << left << setw(15) << plant->getPlantID()
             << setw(15) << plant->getPlantType()
             << setw(12) << fixed << setprecision(1) << plant->getCurrentMoisture()
             << setw(12) << plant->getCurrentTemperature()
             << (plant->getIsHealthy() ? "Healthy" : "Unhealthy") << endl;
    }
}

void Greenhouse::printActuatorStatus() {
    cout << "\n--- ACTUATOR STATUS ---" << endl;
    cout << left << setw(20) << "Actuator" << setw(15) << "Status" << "Power Level" << endl;
    cout << string(50, '-') << endl;

    for (auto actuator : actuators) {
        string status = actuator->getIsActive() ? "ACTIVE" : "INACTIVE";
        cout << left << setw(20) << actuator->getActuatorType()
             << setw(15) << status
             << actuator->getPowerLevel() << "%" << endl;
    }
}

void Greenhouse::runSimulationCycle() {
    simulationCycle++;
    cout << "\n" << endl;
    cout << "################################" << endl;
    cout << "### SIMULATION CYCLE #" << setfill('0') << setw(3) << simulationCycle << setfill(' ') << " ###" << endl;
    cout << "################################" << endl;

    // Execute cycle steps
    readAllSensors();
    updatePlantConditions();
    makeAutomatedDecisions();
    processAlertQueue();
    logCycleData();

    // Print status
    printGreenhouseStatus();
    printSensorReadings();
    printPlantStatus();
    printActuatorStatus();
}

void Greenhouse::logCycleData() {
    logger.logAllPlantsStatus(plants);
    logger.logWaterUsage(totalWaterUsed, "WaterPump");
}

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "greenhouse.h"
#include "sensor.h"
#include "actuator.h"
#include "plant.h"

using namespace std;
// trigger
int main() {
    srand(static_cast<unsigned int>(time(0)));

    cout << "\n" << string(70, '=') << endl;
    cout << "  SMART GREENHOUSE & AUTOMATED IRRIGATION MANAGEMENT SYSTEM" << endl;
    cout << "              Object-Oriented Design Project - C++" << endl;
    cout << string(70, '=') << "\n" << endl;

    // Create main greenhouse
    Greenhouse gh("GREENHOUSE_001");

    // Add Sensors
    cout << "[SETUP] Adding sensors to greenhouse..." << endl;
    gh.addSensor(new MoistureSensor("MOIST_001"));
    gh.addSensor(new TemperatureSensor("TEMP_001"));
    gh.addSensor(new HumiditySensor("HUMID_001"));
    gh.addSensor(new LightSensor("LIGHT_001"));
    gh.addSensor(new WaterLevelSensor("WATER_001"));

    // Add Actuators
    cout << "[SETUP] Adding actuators to greenhouse..." << endl;
    gh.addActuator(new WaterPump("PUMP_001"));
    gh.addActuator(new CoolingFan("FAN_001"));
    gh.addActuator(new Heater("HEAT_001"));
    gh.addActuator(new GrowLight("LIGHT_001"));
    gh.addActuator(new Sprinkler("SPRINK_001"));

    // Add Plants
    cout << "[SETUP] Adding plants to greenhouse..." << endl;
    gh.addPlant(new TomatoPlant("TOMATO_001"));
    gh.addPlant(new LettucePlant("LETTUCE_001"));
    gh.addPlant(new MaizePlant("MAIZE_001"));

    cout << "\n[SETUP] Initialization complete! Starting simulation...\n" << endl;

    // Run simulation for multiple cycles
    int numCycles = 10;

    cout << "Press Enter to start the " << numCycles << "-cycle simulation..." << endl;
    cin.get();

    for (int i = 0; i < numCycles; ++i) {
        gh.runSimulationCycle();

        if (i < numCycles - 1) {
            cout << "\nPress Enter to continue to next cycle..." << endl;
            cin.get();
        }
    }

    // Final Report
    cout << "\n" << endl;
    cout << string(70, '=') << endl;
    cout << "                    SIMULATION COMPLETE" << endl;
    cout << string(70, '=') << endl;

    cout << "\nData has been logged to the following files:" << endl;
    cout << "  . sensor_readings.csv - All sensor measurements" << endl;
    cout << "  . actuator_events.csv - All actuator activations" << endl;
    cout << "  . plant_status.csv - Plant condition logs" << endl;
    cout << "  . water_usage.csv - Water consumption tracking" << endl;
    cout << "  . alerts.csv - System alerts and warnings" << endl;

    cout << "\nTotal simulation cycles completed: " << gh.getSimulationCycle() << endl;
    cout << "Total water consumed: " << gh.getTotalWaterUsed() << " liters" << endl;

    cout << "\n[END] Greenhouse simulation terminated." << endl;

    return 0;
}

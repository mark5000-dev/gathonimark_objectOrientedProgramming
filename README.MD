# Smart Greenhouse & Automated Irrigation Management System

## Project Overview

A console-based simulation system demonstrating **Object-Oriented Design (OOP) principles** in C++. The system simulates an automated greenhouse that reads environmental inputs, makes intelligent decisions, and controls actuators to maintain optimal conditions for various plant types.

### Key Features

- **Sensor System**: Temperature, moisture, humidity, light intensity, and water level sensors
- **Actuator Control**: Water pumps, sprinklers, cooling fans, heaters, and grow lights
- **Plant Management**: Support for multiple plant types (Tomato, Lettuce, Maize) with different requirements
- **Automation Logic**: Intelligent decision-making based on sensor readings
- **Data Persistence**: CSV-based logging for all sensor readings, plant status, and system events
- **Alert System**: Queue-based alert management for critical system events

---

## OOP Design Principles Demonstrated

### 1. **Abstraction**
- Base classes define interfaces; derived classes implement specific behaviors
- `Sensor`, `Actuator`, and `Plant` abstract base classes
- Pure virtual functions enforce contract in derived classes

### 2. **Polymorphism**
- Virtual methods allow different sensor types to implement `readValue()` differently
- Actuators respond uniquely to control signals based on their type
- Plants have different environmental requirements (TomatoPlant vs MaizePlant)

### 3. **Inheritance**
- Clear class hierarchy:
  ```
  Sensor
    ├── MoistureSensor
    ├── TemperatureSensor
    ├── HumiditySensor
    ├── LightSensor
    └── WaterLevelSensor

  Actuator
    ├── WaterPump
    ├── CoolingFan
    ├── Heater
    ├── GrowLight
    └── Sprinkler

  Plant
    ├── TomatoPlant
    ├── LettucePlant
    └── MaizePlant
  ```

### 4. **Encapsulation**
- Private member variables with public getters/setters
- Each class manages its own internal state
- Sensors simulate readings; Actuators manage power levels

### 5. **Composition**
- `Greenhouse` class demonstrates HAS-A relationships:
  - Has multiple Sensors (vector composition)
  - Has multiple Actuators (vector composition)
  - Has multiple Plants (vector composition)
  - Has a Logger instance
  - Has Alert queue

---

## Data Structures Used

| Data Structure | Purpose | Location |
|---|---|---|
| **vector** | Store collections of sensors, actuators, plants | Greenhouse class |
| **queue** | Alert queue for managing system alerts | Greenhouse class |
| **map** | Can be extended for plant database lookup | Future enhancement |
| **priority_queue** | Can be used for scheduled watering tasks | Future enhancement |
| **set** | Can track active devices | Future enhancement |

---

## File Structure

```
Greenhouse Management system/
├── main.cpp                 # Simulation entry point
├── greenhouse.h/.cpp        # Main controller class
├── sensor.h/.cpp            # Sensor hierarchy
├── actuator.h/.cpp          # Actuator hierarchy
├── plant.h/.cpp             # Plant hierarchy
├── logger.h/.cpp            # CSV logging system
├── plant_profiles.csv       # Plant configuration database
├── system_config.csv        # System parameters
├── sensor_readings.csv      # Runtime log (generated)
├── actuator_events.csv      # Runtime log (generated)
├── plant_status.csv         # Runtime log (generated)
├── water_usage.csv          # Runtime log (generated)
└── alerts.csv               # Runtime log (generated)
```

---

## Class Descriptions

### Sensor Classes
- **MoistureSensor**: Reads soil moisture (0-100%)
- **TemperatureSensor**: Reads temperature (-10°C to 50°C)
- **HumiditySensor**: Reads relative humidity (0-100%)
- **LightSensor**: Reads light intensity (0-10000 lux)
- **WaterLevelSensor**: Reads water tank level (0-100%)

### Actuator Classes
- **WaterPump**: Main irrigation system (0-100% flow rate)
- **Sprinkler**: Localized watering (0-100% flow rate)
- **CoolingFan**: Temperature reduction (0-100% power)
- **Heater**: Temperature maintenance (0-100% power)
- **GrowLight**: Artificial lighting (0-100% intensity)

### Plant Classes
- **TomatoPlant**: High water demand (50-70% soil moisture)
  - Optimal temp: 25°C | Light: 6000 lux
- **LettucePlant**: Medium water demand (40-60% soil moisture)
  - Optimal temp: 20°C | Light: 4000 lux
- **MaizePlant**: Low water demand (30-50% soil moisture)
  - Optimal temp: 28°C | Light: 8000 lux

### Logger Class
Handles all data persistence with pipe-separated CSV files:
- Logs sensor readings with timestamps
- Records actuator activation events
- Tracks plant health status
- Monitors water consumption
- Captures system alerts by severity level

---

## Automation Rules

### Irrigation Control
```
IF soil_moisture < MOISTURE_THRESHOLD AND water_level > MIN_LEVEL
  THEN activate water pump
ELSE
  deactivate water pump
```

### Temperature Control
```
IF temperature > TEMP_MAX_THRESHOLD
  THEN activate cooling fan at full power
IF temperature < TEMP_MIN_THRESHOLD
  THEN activate heater at full power
```

### Lighting Control
```
IF light_intensity < LIGHT_THRESHOLD
  THEN activate grow lights at full intensity
ELSE
  deactivate grow lights
```

### Plant Health Monitoring
```
FOR each plant:
  IF moisture_critical OR temperature_out_of_range OR light_insufficient
    THEN mark plant as unhealthy
    AND generate WARNING alert
```

---

## CSV File Format (Pipe-Separated)

### sensor_readings.csv
```
Timestamp|SensorID|SensorType|Value|Unit|MinRange|MaxRange
2024-12-13|12:30:45|MOIST_001|MoistureSensor|45.23|%|0|100
```

### plant_status.csv
```
Timestamp|PlantID|PlantName|PlantType|Moisture|Temperature|DaysGrowing|IsHealthy
2024-12-13|12:30:45|TOMATO_001|Tomato|TomatoPlant|45.23|25.15|10|Healthy
```

### water_usage.csv
```
Timestamp|TotalLitersDispensed|Source
2024-12-13|12:30:45|3.75|WaterPump
```

### alerts.csv
```
Timestamp|Severity|Message
2024-12-13|12:30:45|INFO|Water pump activated - low soil moisture
2024-12-13|12:31:15|WARNING|Plant TOMATO_001 is unhealthy - check conditions
```

---

## Compilation & Execution

### Using CodeBlocks (Recommended)
1. Open `Greenhouse Management system.cbp` in CodeBlocks
2. Select Build → Build & Run (F9)
3. The program will compile and start the simulation

### Using Command Line (MinGW/GCC)
```bash
cd "Greenhouse Management system"
g++ -std=c++11 -Wall -o greenhouse main.cpp sensor.cpp actuator.cpp plant.cpp greenhouse.cpp logger.cpp
./greenhouse
```

### Using Visual Studio
1. Create new C++ Console Project
2. Add all `.h` and `.cpp` files to the project
3. Build Solution (Ctrl+Shift+B)
4. Debug → Start (F5)

---

## Program Flow

1. **Initialization Phase**
   - Create Greenhouse instance
   - Instantiate and register all sensors
   - Instantiate and register all actuators
   - Instantiate and register all plants
   - Initialize CSV log files

2. **Simulation Loop** (repeats for 10 cycles)
   - **Read Sensors**: All sensors take readings with realistic variations
   - **Update Plants**: Current environmental conditions update plant status
   - **Make Decisions**: Automation logic evaluates conditions and controls actuators
   - **Process Alerts**: Handle any generated alerts
   - **Log Data**: Write all data to CSV files
   - **Print Status**: Display current system state to console

3. **Termination**
   - Display simulation statistics
   - Generate final report
   - Clean up allocated memory
   - Inform user of generated log files

---

## Sample Output

```
################################
### SIMULATION CYCLE #001 ###
################################

=== Reading All Sensors ===
TemperatureSensor (TEMP_001): 25.43
MoistureSensor (MOIST_001): 41.23
HumiditySensor (HUMID_001): 59.87
LightSensor (LIGHT_001): 5123.45
WaterLevelSensor (WATER_001): 78.92

=== Making Automated Decisions ===
[PUMP] PUMP_001 activated at 80% - Dispensed 0.40L
[ALERT|INFO] Water pump activated - low soil moisture

--- SENSOR READINGS ---
TemperatureSensor          25.43           OK
MoistureSensor             41.23           OK
HumiditySensor             59.87           OK
LightSensor              5123.45           OK
WaterLevelSensor           78.92           OK

--- PLANT STATUS ---
Plant           Type           Moisture     Temp         Health
TOMATO_001      TomatoPlant    41.23        25.43        Healthy
LETTUCE_001     LettucePlant   41.23        25.43        Healthy
MAIZE_001       MaizePlant     41.23        25.43        Healthy
```

---

## Key Learning Outcomes

This project demonstrates:

✓ **OOP Fundamentals**: Inheritance, polymorphism, encapsulation, abstraction
✓ **Design Patterns**: Observer (alerts), Factory (sensor creation), Composite (greenhouse structure)
✓ **STL Usage**: Vectors, queues, dynamic memory management
✓ **File I/O**: CSV data persistence with special character separation
✓ **Simulation Logic**: State management, decision trees, automation rules
✓ **Code Organization**: Clear separation of concerns, modular architecture
✓ **Real-world Application**: IoT simulation relevant to industry

---

## Future Enhancements

- Load plant profiles from CSV at runtime
- Weather simulation (sunny, rainy, cloudy, cold night)
- Multi-greenhouse support with inter-greenhouse resource sharing
- Scheduling system for planned maintenance
- Statistics dashboard with water usage analysis
- Plant growth prediction models
- Database integration (SQLite) instead of CSV
- Web interface for remote monitoring
- IoT integration with actual sensors (Arduino)

---

## Project Statistics

| Metric | Value |
|--------|-------|
| Total Classes | 15+ |
| Lines of Code | 800+ |
| Header Files | 6 |
| Implementation Files | 6 |
| CSV Data Files | 7 |
| OOP Concepts Demonstrated | 8+ |
| STL Data Structures | 2+ |

---

## Notes for Presentation

When defending this project, emphasize:

1. **Strong OOP Design**: Clear class hierarchy with proper use of abstract classes
2. **Realistic Simulation**: Sensor readings vary naturally; actuators consume resources
3. **Intelligent Automation**: Not hardcoded; uses conditional logic based on sensor data
4. **Data Persistence**: All system events logged for analysis
5. **Scalability**: Easy to add new plant types, sensors, or actuators
6. **Real-world Relevance**: Demonstrates practical IoT/embedded systems concepts

---

## Author Notes

This project was designed to demonstrate **professional C++ development practices** suitable for an end-of-semester OOP course. The architecture emphasizes:

- Clean code with meaningful variable names
- Proper memory management (constructors/destructors)
- Const correctness and appropriate access levels
- Comprehensive error handling
- Extensibility for future features

The greenhouse simulation provides an engaging context for OOP concepts while remaining feasible within a 5-day development window.

---

**Created**: December 2024  
**Language**: C++ (C++11 standard)  
**IDE**: Code::Blocks / Visual Studio / GCC  
**Status**: Complete & Ready for Presentation

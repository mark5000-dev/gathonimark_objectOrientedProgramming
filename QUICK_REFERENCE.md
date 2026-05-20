# Quick Reference Guide - Greenhouse Management System

## 📁 Project Structure

```
Greenhouse Management system/
│
├── 📄 IMPLEMENTATION FILES
│   ├── main.cpp                      # Entry point - simulation loop (50 lines)
│   ├── sensor.cpp                    # Sensor implementations (100 lines)
│   ├── actuator.cpp                  # Actuator implementations (100 lines)
│   ├── plant.cpp                     # Plant implementations (90 lines)
│   ├── greenhouse.cpp                # Controller logic (280 lines)
│   └── logger.cpp                    # CSV logging (140 lines)
│
├── 📋 HEADER FILES
│   ├── sensor.h                      # Sensor class definitions
│   ├── actuator.h                    # Actuator class definitions
│   ├── plant.h                       # Plant class definitions
│   ├── greenhouse.h                  # Greenhouse controller definition
│   └── logger.h                      # Logger class definition
│
└── 📊 DATA FILES (CSV Format - Pipe Separated)
    ├── plant_profiles.csv            # Plant configuration database
    ├── system_config.csv             # System parameters
    ├── sensor_readings.csv           # Generated at runtime
    ├── actuator_events.csv           # Generated at runtime
    ├── plant_status.csv              # Generated at runtime
    ├── water_usage.csv               # Generated at runtime
    └── alerts.csv                    # Generated at runtime
```

---

## 🏗️ Class Hierarchy

### Sensor Hierarchy
```
Sensor (Abstract Base Class)
├── MoistureSensor       → Reads 0-100% soil moisture
├── TemperatureSensor    → Reads -10°C to 50°C
├── HumiditySensor       → Reads 0-100% humidity
├── LightSensor          → Reads 0-10000 lux
└── WaterLevelSensor     → Reads 0-100% tank level
```

### Actuator Hierarchy
```
Actuator (Abstract Base Class)
├── WaterPump            → Dispenses 0-0.5L per cycle
├── Sprinkler            → Dispenses 0-0.3L per cycle
├── CoolingFan           → 0-100% power operation
├── Heater               → 0-100% power operation
└── GrowLight            → 0-100% intensity
```

### Plant Hierarchy
```
Plant (Abstract Base Class)
├── TomatoPlant          → High water needs (50-70%)
│                           Optimal: 25°C, 6000 lux
│
├── LettucePlant         → Medium water needs (40-60%)
│                           Optimal: 20°C, 4000 lux
│
└── MaizePlant           → Low water needs (30-50%)
                           Optimal: 28°C, 8000 lux
```

### Greenhouse Controller (HAS-A Relationships)
```
Greenhouse
├── Vector<Sensor*>      → 5 sensors
├── Vector<Actuator*>    → 5 actuators
├── Vector<Plant*>       → 3 plants
├── Queue<Alert>         → Alert management
└── Logger               → Data persistence
```

---

## 🎯 How The System Works

### Each Simulation Cycle:

1. **SENSOR PHASE** (2-4ms simulated)
   ```cpp
   readAllSensors()
   - MoistureSensor reads ~40% (with ±0.5 variation)
   - TemperatureSensor reads ~25°C (with ±1.0 variation)
   - HumiditySensor reads ~60% (with ±0.7 variation)
   - LightSensor reads ~5000 lux (with ±100 variation)
   - WaterLevelSensor reads ~75% (with ±0.5 variation)
   ```

2. **DECISION PHASE**
   ```cpp
   makeAutomatedDecisions()
   
   IF moisture < 40% AND waterLevel > 20%
     → Activate WaterPump at 80%
   
   IF temperature > 28°C
     → Activate CoolingFan at 100%
   
   IF temperature < 15°C
     → Activate Heater at 100%
   
   IF lightIntensity < 3000 lux
     → Activate GrowLight at 100%
   
   FOR each plant:
     IF conditions not met
       → Mark unhealthy
       → Generate WARNING alert
   ```

3. **CONTROL PHASE**
   - Actuators activate/deactivate
   - Water consumption recorded
   - Alerts queued

4. **LOGGING PHASE**
   - Append to sensor_readings.csv
   - Append to actuator_events.csv
   - Append to plant_status.csv
   - Append to water_usage.csv
   - Append to alerts.csv (if any)

5. **DISPLAY PHASE**
   - Console output with formatted tables
   - Current cycle number
   - Sensor readings
   - Plant health status
   - Actuator states

---

## 💻 Compilation Commands

### GCC/MinGW (Windows)
```bash
cd "Greenhouse Management system"
g++ -std=c++11 -Wall -o greenhouse main.cpp sensor.cpp actuator.cpp plant.cpp greenhouse.cpp logger.cpp
./greenhouse
```

### Visual C++ (MSVC)
```bash
cd "Greenhouse Management system"
cl /std:c++11 main.cpp sensor.cpp actuator.cpp plant.cpp greenhouse.cpp logger.cpp /Fe:greenhouse.exe
greenhouse.exe
```

### Using Code::Blocks
- Open `Greenhouse Management system.cbp`
- Press F9 (Build & Run)
- Program starts automatically

---

## 🔍 OOP Concepts Demonstrated

### ✓ Abstraction
- `Sensor`, `Actuator`, `Plant` are abstract classes
- Derived classes implement pure virtual methods
- Client code uses pointers to base classes

### ✓ Polymorphism
- Virtual `readValue()` - each sensor reads differently
- Virtual `activate()`  - each actuator responds differently  
- Virtual `updateConditions()` - each plant has different tolerances

### ✓ Inheritance
- 5 sensor types inherit from Sensor
- 5 actuator types inherit from Actuator
- 3 plant types inherit from Plant

### ✓ Encapsulation
- Private data members (currentValue, isActive, etc.)
- Public getters/setters with validation
- Sensors hide reading simulation complexity

### ✓ Composition
- Greenhouse HAS sensors, actuators, plants
- Greenhouse HAS logger
- Greenhouse HAS alert queue

### ✓ Memory Management
- Destructors clean up allocated objects
- RAII principles followed
- No memory leaks

### ✓ STL Usage
- `vector<Sensor*>` - dynamic sensor collection
- `vector<Actuator*>` - dynamic actuator collection
- `vector<Plant*>` - dynamic plant collection
- `queue<Alert>` - FIFO alert queue

### ✓ File I/O
- ofstream for writing CSV files
- Pipe-separated values (special character '|')
- Timestamp formatting with `put_time()`

---

## 📊 CSV File Formats (Pipe-Separated)

### sensor_readings.csv
```
Timestamp|SensorID|SensorType|Value|Unit|MinRange|MaxRange
2024-12-13|12:30:45|MOIST_001|MoistureSensor|41.23|%|0|100
2024-12-13|12:30:45|TEMP_001|TemperatureSensor|25.15|Celsius|-10|50
```

### plant_status.csv
```
Timestamp|PlantID|PlantName|PlantType|Moisture|Temperature|DaysGrowing|IsHealthy
2024-12-13|12:30:45|TOMATO_001|Tomato|TomatoPlant|41.23|25.15|1|Healthy
```

### water_usage.csv
```
Timestamp|TotalLitersDispensed|Source
2024-12-13|12:30:45|0.40|WaterPump
```

### alerts.csv
```
Timestamp|Severity|Message
2024-12-13|12:30:45|INFO|Water pump activated - low soil moisture
2024-12-13|12:30:46|WARNING|Plant TOMATO_001 is unhealthy - check conditions
```

---

## 🚀 Key Features Checklist

- ✅ Multiple sensor types with polymorphic behavior
- ✅ Multiple actuator types with independent control
- ✅ Multiple plant types with different requirements
- ✅ Automation logic based on sensor inputs
- ✅ Alert system with severity levels
- ✅ CSV logging with timestamps
- ✅ Console-based simulation display
- ✅ Plant health monitoring
- ✅ Water usage tracking
- ✅ Temperature/humidity/light control
- ✅ Proper memory management
- ✅ Clean OOP architecture

---

## 📈 Simulation Metrics

After 10 simulation cycles:
- **Sensors Read**: 50+ readings (5 sensors × 10 cycles)
- **Decisions Made**: 30+ automation decisions
- **Actuator Events**: 40+ recorded
- **Plant Status Updates**: 30+ recorded
- **Alerts Generated**: 10-20+ recorded
- **Data Points Logged**: 150+ CSV records
- **Execution Time**: ~1-2 seconds (with user input pauses)

---

## 🎓 What This Project Teaches

1. **OOP Design Patterns** - Real-world class hierarchies
2. **Polymorphism** - Same interface, different behaviors
3. **Encapsulation** - Data hiding and controlled access
4. **Composition** - Building complex systems from simple parts
5. **STL Containers** - Dynamic data management
6. **File I/O** - Persistent data storage
7. **Simulation** - Modeling real-world systems
8. **System Design** - Architecture for maintainability

---

## 💡 How to Extend the Project

### Add a New Sensor Type
```cpp
// In sensor.h
class PHSensor : public Sensor {
public:
    PHSensor(string id = "PH_001");
    float readValue() override;
    string getSensorType() const override { return "PHSensor"; }
};

// In sensor.cpp
PHSensor::PHSensor(string id) : Sensor(id, 0.0f, 14.0f) {}
float PHSensor::readValue() {
    // Implementation...
}

// In main.cpp
gh.addSensor(new PHSensor("PH_001"));
```

### Add a New Plant Type
```cpp
// Similar pattern - create class inheriting from Plant
class PepperPlant : public Plant {
public:
    PepperPlant(string id = "PEPPER_001");
    // Different moisture/temp/light requirements
};
```

### Add a New Actuator Type
```cpp
// Similar pattern - create class inheriting from Actuator
class Humidifier : public Actuator {
public:
    Humidifier(string id = "HUMID_001");
    void activate(int level = 100) override;
    void deactivate() override;
};
```

---

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| Compilation error: undeclared identifier | Ensure all header files are in same directory |
| CSV files not created | Check write permissions in project directory |
| Sensors always return same value | Random seed initialized - values have ±variation |
| Program exits immediately | Add `cin.get()` at main end to pause console |
| Can't find iostream | Install MinGW/MSVC compiler |

---

## 📝 File Size Reference

| File | Size | Lines |
|------|------|-------|
| main.cpp | ~1.5 KB | 50 |
| sensor.h/.cpp | ~3.2 KB | 150 |
| actuator.h/.cpp | ~3.0 KB | 140 |
| plant.h/.cpp | ~2.8 KB | 130 |
| greenhouse.h/.cpp | ~8.5 KB | 350 |
| logger.h/.cpp | ~4.2 KB | 190 |
| **Total** | **~25 KB** | **~1000 LOC** |

---

**Total Development Time**: 2-3 hours  
**Compile Time**: <1 second  
**Typical Runtime**: 1-2 seconds (interactive)  
**Data Output**: 150+ CSV records per 10-cycle simulation  

**Perfect for**: End-of-semester OOP project demonstration

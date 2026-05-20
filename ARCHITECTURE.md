# System Architecture & Design Document

## 1. System Overview

The Smart Greenhouse Management System is a **console-based IoT simulation** that demonstrates professional C++ development practices through a real-world greenhouse automation scenario.

### Core Responsibilities

```
┌─────────────────────────────────────────────────────────────┐
│                    GREENHOUSE SYSTEM                         │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐             │
│  │  SENSORS   │  │ CONTROLLER │  │ ACTUATORS  │             │
│  │            │  │            │  │            │             │
│  │ • Read     │  │ • Decide   │  │ • Execute  │             │
│  │ • Monitor  │  │ • Control  │  │ • Log      │             │
│  │ • Log      │  │ • Alert    │  │ • Monitor  │             │
│  └────────────┘  └────────────┘  └────────────┘             │
│                                                               │
│  ┌────────────────────────────────────────────────────┐     │
│  │              PLANTS (Monitored)                     │     │
│  │  Sensors → Controller → Actuators → Plants Update  │     │
│  └────────────────────────────────────────────────────┘     │
│                                                               │
│  ┌────────────────────────────────────────────────────┐     │
│  │         PERSISTENCE (CSV Logging)                   │     │
│  │  All events → Logger → CSV files for analysis      │     │
│  └────────────────────────────────────────────────────┘     │
└─────────────────────────────────────────────────────────────┘
```

---

## 2. Class Hierarchy & Relationships

### Sensor Class Hierarchy
```
        ┌─────────────────┐
        │     Sensor      │  (Abstract Base)
        │   (Pure Virtual)│
        └────────┬────────┘
                 │
       ┌─────────┼─────────┬─────────┬──────────┐
       │         │         │         │          │
    ┌──┴──┐  ┌──┴───┐  ┌──┴──┐  ┌──┴───┐  ┌──┴───────┐
    │ Mst │  │ Temp │  │Humid│  │Light │  │ WaterLvl │
    │ Sens│  │ Sens │  │Sens │  │Sens  │  │  Sensor  │
    └─────┘  └──────┘  └─────┘  └──────┘  └──────────┘

    Key Methods:
    - readValue() : float          [Pure Virtual]
    - getSensorType() : string     [Virtual]
    - getCurrentValue() : float
    - getLastReadTime() : time_t
```

### Actuator Class Hierarchy
```
        ┌──────────────────┐
        │    Actuator      │  (Abstract Base)
        │  (Pure Virtual)  │
        └────────┬─────────┘
                 │
    ┌────────────┼────────────┬─────────┬──────────┐
    │            │            │         │          │
 ┌──┴──┐  ┌────┴─┐  ┌───────┴┐  ┌────┴─┐  ┌────┴────┐
 │Pump │  │ Fan  │  │Heater  │  │Light │  │Sprinkler│
 └─────┘  └──────┘  └────────┘  └──────┘  └─────────┘

    Key Methods:
    - activate(int level) : void   [Pure Virtual]
    - deactivate() : void          [Pure Virtual]
    - getActuatorType() : string   [Virtual]
    - getIsActive() : bool
    - getPowerLevel() : int
```

### Plant Class Hierarchy
```
        ┌─────────────────┐
        │     Plant       │  (Abstract Base)
        │  (Pure Virtual) │
        └────────┬────────┘
                 │
           ┌─────┼──────┐
           │     │      │
        ┌──┴──┐ ┌┴────┐ ┌┴────┐
        │Toma │ │Lett │ │Maize│
        │to   │ │uce  │ │     │
        └─────┘ └─────┘ └─────┘

    Key Methods:
    - updateConditions(float, float, float) [Pure Virtual]
    - needsWater() : bool
    - needsCooling() : bool
    - needsHeating() : bool
    - getIsHealthy() : bool
```

---

## 3. Composition Structure (HAS-A Relationships)

```
            ┌──────────────────────────┐
            │    Greenhouse (Main)     │
            └────────────┬─────────────┘
                         │
        ┌────────────────┼────────────────┐
        │                │                │
    ┌───▼────┐      ┌────▼────┐      ┌───▼────┐
    │ Sensors │      │Actuators │     │ Plants  │
    │ (vector)│      │ (vector) │     │(vector) │
    └─────────┘      └──────────┘     └─────────┘
        │                 │                 │
    [5 objects]       [5 objects]       [3 objects]
    
    ┌─────────────┐              ┌──────────────┐
    │   Logger    │              │Alert Queue   │
    │(1 instance) │              │ (FIFO)       │
    └─────────────┘              └──────────────┘
        │                            │
    [CSV files]                [System alerts]

    Total Objects in Memory:
    - 5 Sensors + 5 Actuators + 3 Plants = 13 objects
    - 1 Greenhouse + 1 Logger = 2 container objects
    = 16 dynamic objects per simulation
```

---

## 4. Control Flow Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                    START SIMULATION                              │
└────────────────────────┬────────────────────────────────────────┘
                         │
                         ▼
        ┌────────────────────────────────┐
        │  Initialize Greenhouse         │
        │  - Create 5 sensors            │
        │  - Create 5 actuators          │
        │  - Create 3 plants             │
        │  - Initialize CSV files        │
        └────────────┬───────────────────┘
                     │
                     ▼
    ┌───────────────────────────────────────┐
    │     FOR each Simulation Cycle         │  (10 cycles)
    │                                       │
    │  ┌──────────────────────────────────┐ │
    │  │ PHASE 1: READ SENSORS            │ │
    │  │ └─ For each sensor               │ │
    │  │    └─ Call readValue()           │ │
    │  │    └─ Log to sensor_readings.csv │ │
    │  └──────────────────────────────────┘ │
    │                                       │
    │  ┌──────────────────────────────────┐ │
    │  │ PHASE 2: UPDATE PLANTS           │ │
    │  │ └─ For each plant                │ │
    │  │    └─ updateConditions()         │ │
    │  │    └─ Check if healthy           │ │
    │  │    └─ Increment daysGrowing      │ │
    │  └──────────────────────────────────┘ │
    │                                       │
    │  ┌──────────────────────────────────┐ │
    │  │ PHASE 3: MAKE DECISIONS          │ │
    │  │ ├─ Check Moisture → Control Pump │ │
    │  │ │  IF moist < 40 AND water > 20 │ │
    │  │ │  THEN pump.activate(80%)       │ │
    │  │ │                                 │ │
    │  │ ├─ Check Temperature             │ │
    │  │ │  IF temp > 28 THEN fan.act()  │ │
    │  │ │  IF temp < 15 THEN heat.act() │ │
    │  │ │                                 │ │
    │  │ ├─ Check Light                   │ │
    │  │ │  IF light < 3000 THEN light()  │ │
    │  │ │                                 │ │
    │  │ └─ Check Plant Health            │ │
    │  │    IF unhealthy THEN alert()     │ │
    │  └──────────────────────────────────┘ │
    │                                       │
    │  ┌──────────────────────────────────┐ │
    │  │ PHASE 4: LOG DATA                │ │
    │  │ ├─ Log to plant_status.csv       │ │
    │  │ ├─ Log to actuator_events.csv    │ │
    │  │ ├─ Log to water_usage.csv        │ │
    │  │ └─ Log to alerts.csv (if any)    │ │
    │  └──────────────────────────────────┘ │
    │                                       │
    │  ┌──────────────────────────────────┐ │
    │  │ PHASE 5: DISPLAY STATUS          │ │
    │  │ ├─ Print sensor readings         │ │
    │  │ ├─ Print plant status            │ │
    │  │ ├─ Print actuator status         │ │
    │  │ └─ Print statistics              │ │
    │  └──────────────────────────────────┘ │
    │                                       │
    │  ┌──────────────────────────────────┐ │
    │  │ Wait for user to continue        │ │
    │  └──────────────────────────────────┘ │
    └───────────────────────────────────────┘
                     │
                     ▼ (10 times)
        ┌────────────────────────────┐
        │ END SIMULATION              │
        │ - Cleanup memory            │
        │ - Display final statistics  │
        │ - Show generated CSV files  │
        └────────────────────────────┘
                     │
                     ▼
            ┌────────────────┐
            │  PROGRAM EXIT  │
            └────────────────┘
```

---

## 5. Decision Tree (Automation Logic)

```
EVERY SIMULATION CYCLE:

┌─ Check Irrigation ──┐
│                     │
├─ moisture < 40%?   ─┤─ NO  ──→ Keep pump OFF
│  AND               │
│  waterLevel > 20%? ─┤─ YES ──→ Activate pump at 80%
│                     │
│                     └─ Log: "Pump activated"
│
├─ Check Temperature ┐
│                    │
├─ temp > 28°C?    ─┤─ YES ──→ Activate fan at 100%
│                    │         Log: "Cooling activated"
│                    │
│                    ├─ NO ───→ Check if fan is on
│                    │          └─ YES: Deactivate fan
│                    │
├─ temp < 15°C?    ─┤─ YES ──→ Activate heater at 100%
│                    │         Log: "Heating activated"
│                    │
│                    └─ NO ───→ Check if heater is on
│                               └─ YES: Deactivate heater
│
├─ Check Lighting   ┐
│                   │
├─ light < 3000?  ─┤─ YES ──→ Activate grow lights 100%
│                   │         Log: "Lights activated"
│                   │
│                   └─ NO ───→ Check if lights are on
│                              └─ YES: Deactivate lights
│
└─ Check Plant Health ┐
                      │
  FOR each plant      │
  ├─ moisture OK?   ─┤─ NO ──→ Mark unhealthy
  ├─ temp OK?       ─┤─ NO ──→ Mark unhealthy
  ├─ light OK?      ─┤─ NO ──→ Mark unhealthy
  │                  │
  └─ If unhealthy  ─┤────────→ Generate WARNING alert
                     │         Log alert to alerts.csv
                     │
                     └─────────→ Update plant status
```

---

## 6. Data Flow Diagram

```
┌──────────────────────────────────────────────────────────────────┐
│                     ENVIRONMENTAL DATA                            │
│                                                                   │
│   Temperature  Moisture  Humidity  Light Intensity  Water Level  │
│         │          │        │           │               │        │
│         └──────────┴────────┴───────────┴───────────────┘        │
│                           │                                       │
│                           ▼                                       │
│                   ┌──────────────┐                               │
│                   │ SENSORS READ │                               │
│                   │ & SIMULATE   │  ← Realistic variations      │
│                   └──────┬───────┘                               │
│                          │                                       │
│                ┌─────────┴────────┐                              │
│                │                  │                              │
│                ▼                  ▼                              │
│         ┌────────────┐      ┌─────────────┐                     │
│         │ CONTROLLER │      │ LOGGER      │                     │
│         │ LOGIC      │      │ (CSV Write) │                     │
│         └─────┬──────┘      └─────────────┘                     │
│               │                                                  │
│               ▼                                                  │
│         ┌─────────────┐                                          │
│         │ DECISION    │      IF moisture < threshold             │
│         │ ENGINE      │  ──→ THEN activate pump                 │
│         │             │      IF temperature > max               │
│         │             │  ──→ THEN activate fan                  │
│         └─────┬───────┘      etc...                             │
│               │                                                  │
│               ▼                                                  │
│         ┌──────────────┐                                         │
│         │ ACTUATORS    │                                         │
│         │ CONTROL      │                                         │
│         └──────┬───────┘                                         │
│                │                                                 │
│  ┌─────────────┼─────────────┬──────────────┬─────────────┐    │
│  │             │             │              │             │    │
│  ▼             ▼             ▼              ▼             ▼    │
│┌──────┐    ┌──────┐    ┌──────┐        ┌──────┐    ┌──────┐   │
││Pump  │    │Fan   │    │Heater│        │Lights│    │ Sprk │   │
│└──┬───┘    └──┬───┘    └──┬───┘        └──┬───┘    └──┬───┘   │
│   │           │           │               │           │        │
│   │      ┌────┴────────────┴───────────────┴───────────┘        │
│   │      │                                                       │
│   ▼      ▼                                                       │
│ PLANTS AFFECTED                                                  │
│ (Moisture ↑, Temp ↓, Light ↑)                                   │
│   │      │      │                                               │
│   └──────┴──────┘                                               │
│          │                                                       │
│          ▼                                                       │
│  ┌───────────────┐                                              │
│  │ PLANT HEALTH  │                                              │
│  │ UPDATED       │                                              │
│  └───────┬───────┘                                              │
│          │                                                       │
│          ▼                                                       │
│  ┌──────────────────────────────────┐                           │
│  │ CSV FILES UPDATED                │                           │
│  │ • sensor_readings.csv            │                           │
│  │ • actuator_events.csv            │                           │
│  │ • plant_status.csv               │                           │
│  │ • water_usage.csv                │                           │
│  │ • alerts.csv (if alerts)         │                           │
│  └──────────────────────────────────┘                           │
│                                                                   │
└──────────────────────────────────────────────────────────────────┘
```

---

## 7. Memory Layout (Stack & Heap)

```
┌─────────────────────────────────────────────┐
│              STACK (Automatic)              │
├─────────────────────────────────────────────┤
│ main() Function                             │
│ ├─ Greenhouse gh ("GREENHOUSE_001")        │
│ │  ├─ string greenhouseID                 │
│ │  ├─ vector<Sensor*> sensors             │
│ │  ├─ vector<Actuator*> actuators         │
│ │  ├─ vector<Plant*> plants               │
│ │  ├─ queue<Alert> alertQueue             │
│ │  ├─ Logger logger                       │
│ │  │  ├─ string sensorLogFile             │
│ │  │  ├─ string actuatorLogFile           │
│ │  │  └─ ...other string members          │
│ │  └─ ...other members                    │
│ └─ Local variables (numCycles, etc.)      │
│                                             │
└─────────────────────────────────────────────┘
                    │
                    │ References (pointers)
                    │
                    ▼
┌─────────────────────────────────────────────┐
│           HEAP (Dynamic Memory)             │
├─────────────────────────────────────────────┤
│                                             │
│  Sensor Objects:                           │
│  ├─ MoistureSensor      [~200 bytes]      │
│  ├─ TemperatureSensor   [~200 bytes]      │
│  ├─ HumiditySensor      [~200 bytes]      │
│  ├─ LightSensor         [~200 bytes]      │
│  └─ WaterLevelSensor    [~200 bytes]      │
│                                             │
│  Actuator Objects:                         │
│  ├─ WaterPump           [~200 bytes]      │
│  ├─ CoolingFan          [~200 bytes]      │
│  ├─ Heater              [~200 bytes]      │
│  ├─ GrowLight           [~200 bytes]      │
│  └─ Sprinkler           [~200 bytes]      │
│                                             │
│  Plant Objects:                            │
│  ├─ TomatoPlant         [~300 bytes]      │
│  ├─ LettucePlant        [~300 bytes]      │
│  └─ MaizePlant          [~300 bytes]      │
│                                             │
│  TOTAL HEAP: ~4.0 KB per simulation       │
│                                             │
└─────────────────────────────────────────────┘

Cleanup in ~Greenhouse():
for (auto sensor : sensors) delete sensor;
for (auto actuator : actuators) delete actuator;
for (auto plant : plants) delete plant;
→ All heap memory deallocated
```

---

## 8. Polymorphic Method Resolution

### Example: Reading Sensors (Virtual Methods)
```cpp
vector<Sensor*> sensors;  // Base class pointers
sensors.push_back(new MoistureSensor());
sensors.push_back(new TemperatureSensor());
sensors.push_back(new LightSensor());

for (auto sensor : sensors) {
    float value = sensor->readValue();  // Virtual call
}

// Runtime dispatch:
sensors[0]->readValue()    →  MoistureSensor::readValue()
sensors[1]->readValue()    →  TemperatureSensor::readValue()
sensors[2]->readValue()    →  LightSensor::readValue()

// Each returns different value based on different logic
// Client code doesn't need to know the specific type
```

### Example: Controlling Actuators (Virtual Methods)
```cpp
vector<Actuator*> actuators;
actuators.push_back(new WaterPump());
actuators.push_back(new CoolingFan());
actuators.push_back(new Heater());

// Same method call, different behaviors:
actuators[0]->activate(80);  → Pump outputs console: "Dispensed 0.40L"
actuators[1]->activate(100); → Fan outputs console: "FAN activated at 100%"
actuators[2]->activate(100); → Heater outputs console: "HEATER activated at 100%"
```

---

## 9. Configuration Parameters (system_config.csv)

```
GREENHOUSE SETTINGS:
├─ MOISTURE_THRESHOLD = 40.0%
│  (Irrigation starts below this)
│
├─ TEMP_MAX_THRESHOLD = 28.0°C
│  (Cooling activates above this)
│
├─ TEMP_MIN_THRESHOLD = 15.0°C
│  (Heating activates below this)
│
├─ LIGHT_THRESHOLD = 3000.0 lux
│  (Grow lights activate below this)
│
├─ WATER_TANK_CAPACITY = 1000.0 L
│  (Total tank size)
│
└─ WATER_TANK_MIN_LEVEL = 50.0 L
   (Pump disabled if below this)
```

---

## 10. Extension Points (Future Features)

### 1. Add New Sensor Type
- Inherit from Sensor
- Implement readValue()
- Add to greenhouse in main()

### 2. Add New Actuator Type
- Inherit from Actuator
- Implement activate() and deactivate()
- Add to greenhouse in main()

### 3. Add New Plant Type
- Inherit from Plant
- Set thresholds in constructor
- Implement updateConditions()
- Add to greenhouse in main()

### 4. Implement Weather Simulation
- Add RandomWeather class
- Modify sensor readings based on weather
- Add weather events queue

### 5. Add Multi-Greenhouse Support
- Create GreenhouseCluster class
- Manage multiple greenhouses
- Share resource pools

### 6. Implement Scheduling
- Add priority_queue<Task>
- Schedule watering times
- Schedule maintenance tasks

---

## Summary Table: Design Patterns Used

| Pattern | Where | Purpose |
|---------|-------|---------|
| **Factory** | Sensor/Actuator creation | Create appropriate types |
| **Strategy** | Virtual methods | Different behaviors same interface |
| **Observer** | Alert system | Notify on events |
| **Composite** | Greenhouse/Plants | Tree structure of components |
| **Singleton** | Logger instance | Single point of file access |
| **Template Method** | Simulation cycle | Standard steps, varies by type |

---

**Total System Complexity**: Manageable yet comprehensive  
**Educational Value**: High (demonstrates 8+ OOP concepts)  
**Real-world Applicability**: Yes (IoT/embedded systems)  
**Extensibility**: Excellent (easy to add new types)  

Perfect for demonstrating professional C++ architecture in an academic project!

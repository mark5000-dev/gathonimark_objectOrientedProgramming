#include "plant.h"

// Base Plant Constructor
Plant::Plant(string id, string name, float minMoist, float optMoist,
             float minTemp, float optTemp, float minL, float optL)
    : plantID(id), plantName(name), currentMoisture(0), currentTemperature(0),
      minMoisture(minMoist), optimalMoisture(optMoist),
      minTemperature(minTemp), optimalTemperature(optTemp),
      minLight(minL), optimalLight(optL),
      daysGrowing(0), isHealthy(true) {
}

bool Plant::needsWater() const {
    return currentMoisture < minMoisture;
}

bool Plant::needsCooling() const {
    return currentTemperature > optimalTemperature + 5.0f;
}

bool Plant::needsHeating() const {
    return currentTemperature < minTemperature;
}

bool Plant::needsLight() const {
    return false; // To be implemented with actual light sensor value
}

// Tomato Plant Implementation
TomatoPlant::TomatoPlant(string id)
    : Plant(id, "Tomato", 50.0f, 70.0f, 15.0f, 25.0f, 3000.0f, 6000.0f) {
}

void TomatoPlant::updateConditions(float moisture, float temp, float light) {
    currentMoisture = moisture;
    currentTemperature = temp;

    // Tomatoes are sensitive to conditions
    bool tempOK = (temp >= minTemperature && temp <= optimalTemperature + 10.0f);
    bool moistOK = (moisture >= minMoisture * 0.8f);
    bool lightOK = (light >= minLight * 0.7f);

    isHealthy = tempOK && moistOK && lightOK;
}

// Lettuce Plant Implementation
LettucePlant::LettucePlant(string id)
    : Plant(id, "Lettuce", 40.0f, 60.0f, 10.0f, 20.0f, 2000.0f, 4000.0f) {
}

void LettucePlant::updateConditions(float moisture, float temp, float light) {
    currentMoisture = moisture;
    currentTemperature = temp;

    // Lettuce prefers cooler conditions
    bool tempOK = (temp >= minTemperature && temp <= optimalTemperature + 8.0f);
    bool moistOK = (moisture >= minMoisture * 0.75f);
    bool lightOK = (light >= minLight * 0.6f);

    isHealthy = tempOK && moistOK && lightOK;
}

// Maize Plant Implementation
MaizePlant::MaizePlant(string id)
    : Plant(id, "Maize", 30.0f, 50.0f, 10.0f, 28.0f, 5000.0f, 8000.0f) {
}

void MaizePlant::updateConditions(float moisture, float temp, float light) {
    currentMoisture = moisture;
    currentTemperature = temp;

    // Maize is drought tolerant
    bool tempOK = (temp >= minTemperature && temp <= optimalTemperature + 15.0f);
    bool moistOK = (moisture >= minMoisture * 0.7f);
    bool lightOK = (light >= minLight * 0.5f);

    isHealthy = tempOK && moistOK && lightOK;
}

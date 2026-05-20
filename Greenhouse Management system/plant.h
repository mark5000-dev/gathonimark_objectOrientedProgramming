#ifndef PLANT_H
#define PLANT_H

#include <string>

using namespace std;

// Abstract Plant Base Class
class Plant {
protected:
    string plantID;
    string plantName;
    float currentMoisture;
    float currentTemperature;
    float minMoisture;
    float optimalMoisture;
    float minTemperature;
    float optimalTemperature;
    float minLight;
    float optimalLight;
    int daysGrowing;
    bool isHealthy;

public:
    Plant(string id, string name, float minMoist, float optMoist, 
          float minTemp, float optTemp, float minL, float optL);
    
    virtual ~Plant() = default;

    // Pure virtual functions
    virtual string getPlantType() const = 0;
    virtual void updateConditions(float moisture, float temp, float light) = 0;

    // Getters
    string getPlantID() const { return plantID; }
    string getPlantName() const { return plantName; }
    float getCurrentMoisture() const { return currentMoisture; }
    float getCurrentTemperature() const { return currentTemperature; }
    float getMinMoisture() const { return minMoisture; }
    float getOptimalMoisture() const { return optimalMoisture; }
    float getMinTemperature() const { return minTemperature; }
    float getOptimalTemperature() const { return optimalTemperature; }
    float getMinLight() const { return minLight; }
    float getOptimalLight() const { return optimalLight; }
    int getDaysGrowing() const { return daysGrowing; }
    bool getIsHealthy() const { return isHealthy; }

    // Setters
    void setCurrentMoisture(float moisture) { currentMoisture = moisture; }
    void setCurrentTemperature(float temp) { currentTemperature = temp; }
    void incrementDaysGrowing() { daysGrowing++; }
    void setIsHealthy(bool health) { isHealthy = health; }

    // Utility functions
    bool needsWater() const;
    bool needsCooling() const;
    bool needsHeating() const;
    bool needsLight() const;
};

// Tomato Plant - high water demand
class TomatoPlant : public Plant {
public:
    TomatoPlant(string id = "TOMATO_001");
    string getPlantType() const override { return "TomatoPlant"; }
    void updateConditions(float moisture, float temp, float light) override;
};

// Lettuce Plant - moderate water demand
class LettucePlant : public Plant {
public:
    LettucePlant(string id = "LETTUCE_001");
    string getPlantType() const override { return "LettucePlant"; }
    void updateConditions(float moisture, float temp, float light) override;
};

// Maize Plant - drought tolerant
class MaizePlant : public Plant {
public:
    MaizePlant(string id = "MAIZE_001");
    string getPlantType() const override { return "MaizePlant"; }
    void updateConditions(float moisture, float temp, float light) override;
};

#endif // PLANT_H

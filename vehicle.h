#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>

enum class VehicleType { MOTORCYCLE, CAR, EV };

class Vehicle {
protected:
    std::string plate;
    std::string owner;
    time_t arrivalTime;
    VehicleType type;
public:
    Vehicle(const std::string& p, const std::string& o, VehicleType t);
    virtual ~Vehicle();
    
    std::string getPlate() const;
    std::string getOwner() const;
    VehicleType getType() const;
    time_t getArrivalTime() const;
    
    void setArrivalTime(time_t t);
    
    virtual double calculateRate(int durationHours) const = 0;
    virtual double calculateRate(time_t startTime, time_t endTime) const;
    virtual std::string getTypeName() const;
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(const std::string& p, const std::string& o);
    double calculateRate(int durationHours) const override;
};

class Car : public Vehicle {
public:
    Car(const std::string& p, const std::string& o);
    double calculateRate(int durationHours) const override;
};

class EV : public Vehicle {
private:
    bool chargingRequired;
public:
    EV(const std::string& p, const std::string& o, bool charging);
    bool needsCharging() const;
    double calculateRate(int durationHours) const override;
};

#endif

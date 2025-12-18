#include "Vehicle.h"
#include "Exceptions.h"
#include <iostream>

Vehicle::Vehicle(const std::string& p, const std::string& o, VehicleType t) 
    : plate(p), owner(o), type(t), arrivalTime(0) {}

Vehicle::~Vehicle() {}

std::string Vehicle::getPlate() const { return plate; }
std::string Vehicle::getOwner() const { return owner; }
VehicleType Vehicle::getType() const { return type; }
time_t Vehicle::getArrivalTime() const { return arrivalTime; }

void Vehicle::setArrivalTime(time_t t) { arrivalTime = t; }

double Vehicle::calculateRate(time_t startTime, time_t endTime) const {
    int duration = (endTime - startTime) / 3600;
    if (duration < 1) duration = 1;
    return calculateRate(duration);
}

std::string Vehicle::getTypeName() const {
    switch(type) {
        case VehicleType::MOTORCYCLE: return "Motorcycle";
        case VehicleType::CAR: return "Car";
        case VehicleType::EV: return "EV";
        default: return "Unknown";
    }
}

// Motorcycle implementation
Motorcycle::Motorcycle(const std::string& p, const std::string& o) 
    : Vehicle(p, o, VehicleType::MOTORCYCLE) {}

double Motorcycle::calculateRate(int durationHours) const {
    double rate = durationHours * 2000;
    if (durationHours > 24) {
        rate *= 0.9; // 10% discount
    }
    return rate;
}

// Car implementation
Car::Car(const std::string& p, const std::string& o) 
    : Vehicle(p, o, VehicleType::CAR) {}

double Car::calculateRate(int durationHours) const {
    double rate = durationHours * 5000;
    if (durationHours > 24) {
        rate *= 0.9; // 10% discount
    }
    return rate;
}

// EV implementation
EV::EV(const std::string& p, const std::string& o, bool charging) 
    : Vehicle(p, o, VehicleType::EV), chargingRequired(charging) {}

bool EV::needsCharging() const { return chargingRequired; }

double EV::calculateRate(int durationHours) const {
    double rate = durationHours * 4000;
    if (chargingRequired) {
        rate += 3000; // charging fee
    }
    if (durationHours > 24) {
        rate *= 0.9; // 10% discount
    }
    return rate;
}

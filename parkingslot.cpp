#include "ParkingSlot.h"
#include <ctime>
#include <iostream>

ParkingSlot::ParkingSlot(const std::string& slotId, int lvl) 
    : id(slotId), level(lvl), occupied(false), vehicle(nullptr) {}

ParkingSlot::~ParkingSlot() {}

std::string ParkingSlot::getId() const { return id; }
int ParkingSlot::getLevel() const { return level; }
bool ParkingSlot::isOccupied() const { return occupied; }
std::shared_ptr<Vehicle> ParkingSlot::getVehicle() const { return vehicle; }

void ParkingSlot::parkVehicle(std::shared_ptr<Vehicle> v) {
    if (occupied) {
        throw ParkingException("Slot already occupied");
    }
    vehicle = v;
    occupied = true;
    v->setArrivalTime(time(0));
}

void ParkingSlot::unparkVehicle() {
    vehicle.reset();
    occupied = false;
}

// RegularSlot implementation
RegularSlot::RegularSlot(const std::string& id, int lvl) : ParkingSlot(id, lvl) {}

bool RegularSlot::canPark(const Vehicle& v) const {
    return true; // Regular slot accepts all vehicles
}

std::string RegularSlot::getSlotType() const {
    return "Regular";
}

// EVSlot implementation
EVSlot::EVSlot(const std::string& id, int lvl) : ParkingSlot(id, lvl) {}

bool EVSlot::canPark(const Vehicle& v) const {
    return v.getType() == VehicleType::EV;
}

std::string EVSlot::getSlotType() const {
    return "EV";
}

// VIPSlot implementation
VIPSlot::VIPSlot(const std::string& id, int lvl) 
    : ParkingSlot(id, lvl), reserved(false), reservationEnd(0) {}

bool VIPSlot::canPark(const Vehicle& v) const {
    if (reserved) {
        return v.getPlate() == reservedPlate && time(0) < reservationEnd;
    }
    return true;
}

std::string VIPSlot::getSlotType() const {
    return "VIP";
}

bool VIPSlot::isReserved() const { return reserved; }

void VIPSlot::reserve(const std::string& plate, int durationHours) {
    if (occupied) {
        throw ReservationException();
    }
    reserved = true;
    reservedPlate = plate;
    reservationEnd = time(0) + (durationHours * 3600);
}

void VIPSlot::clearReservation() {
    reserved = false;
    reservedPlate = "";
    reservationEnd = 0;
}

double VIPSlot::calculateRate(int durationHours) const {
    double baseRate = durationHours * 5000; // Car rate as base
    double vipRate = baseRate * 1.5; // 50% surcharge
    if (durationHours > 24) {
        vipRate *= 0.9; // 10% discount
    }
    return vipRate;
}

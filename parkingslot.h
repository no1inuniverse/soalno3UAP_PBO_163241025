#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include "Vehicle.h"
#include "Exceptions.h"
#include <memory>

class ParkingSlot {
protected:
    std::string id;
    int level;
    bool occupied;
    std::shared_ptr<Vehicle> vehicle;
public:
    ParkingSlot(const std::string& slotId, int lvl);
    virtual ~ParkingSlot();
    
    std::string getId() const;
    int getLevel() const;
    bool isOccupied() const;
    std::shared_ptr<Vehicle> getVehicle() const;
    
    virtual bool canPark(const Vehicle& v) const = 0;
    virtual std::string getSlotType() const = 0;
    
    void parkVehicle(std::shared_ptr<Vehicle> v);
    void unparkVehicle();
};

class RegularSlot : public ParkingSlot {
public:
    RegularSlot(const std::string& id, int lvl);
    bool canPark(const Vehicle& v) const override;
    std::string getSlotType() const override;
};

class EVSlot : public ParkingSlot {
public:
    EVSlot(const std::string& id, int lvl);
    bool canPark(const Vehicle& v) const override;
    std::string getSlotType() const override;
};

class VIPSlot : public ParkingSlot {
private:
    bool reserved;
    std::string reservedPlate;
    time_t reservationEnd;
public:
    VIPSlot(const std::string& id, int lvl);
    bool canPark(const Vehicle& v) const override;
    std::string getSlotType() const override;
    
    bool isReserved() const;
    void reserve(const std::string& plate, int durationHours);
    void clearReservation();
    double calculateRate(int durationHours) const;
};

#endif

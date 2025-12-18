#ifndef TICKET_H
#define TICKET_H

#include "Vehicle.h"
#include "ParkingSlot.h"
#include <memory>
#include <ctime>

class Ticket {
private:
    static int nextId;
    int id;
    std::string slotId;
    std::shared_ptr<Vehicle> vehicle;
    time_t entryTime;
    bool paid;
public:
    Ticket(const std::string& slotId, std::shared_ptr<Vehicle> v);
    
    int getId() const;
    std::string getSlotId() const;
    std::shared_ptr<Vehicle> getVehicle() const;
    time_t getEntryTime() const;
    bool isPaid() const;
    void markPaid();
    
    double calculateFee(time_t exitTime) const;
};

#endif

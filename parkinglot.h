#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include "ParkingSlot.h"
#include "Ticket.h"
#include "Exceptions.h"
#include <vector>
#include <memory>
#include <utility>

class ParkingLot {
private:
    std::vector<std::shared_ptr<ParkingSlot>> slots;
    std::vector<std::shared_ptr<Ticket>> tickets;
    std::vector<std::pair<time_t, double>> payments;
    
    ParkingSlot* findAvailableSlot(const Vehicle& v);
    
public:
    ParkingLot();
    
    std::shared_ptr<Ticket> parkVehicle(std::shared_ptr<Vehicle> v);
    double unparkVehicle(int ticketId);
    void reserveVIP(const std::string& plate, int durationHours);
    void showOccupation() const;
    void generateDailyReport() const;
    void saveTestData();
};

#endif

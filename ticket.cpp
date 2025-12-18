#include "Ticket.h"
#include <ctime>

int Ticket::nextId = 1000;

Ticket::Ticket(const std::string& slotId, std::shared_ptr<Vehicle> v) 
    : id(nextId++), slotId(slotId), vehicle(v), entryTime(time(0)), paid(false) {}

int Ticket::getId() const { return id; }
std::string Ticket::getSlotId() const { return slotId; }
std::shared_ptr<Vehicle> Ticket::getVehicle() const { return vehicle; }
time_t Ticket::getEntryTime() const { return entryTime; }
bool Ticket::isPaid() const { return paid; }
void Ticket::markPaid() { paid = true; }

double Ticket::calculateFee(time_t exitTime) const {
    int duration = (exitTime - entryTime) / 3600;
    if (duration < 1) duration = 1;
    
    // Check if it's in VIP slot
    if (slotId.find("V") != std::string::npos) { // VIP slot starts with V
        VIPSlot vip("", 0);
        return vip.calculateRate(duration);
    }
    
    return vehicle->calculateRate(duration);
}

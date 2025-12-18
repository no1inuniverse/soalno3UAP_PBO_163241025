#include "ParkingLot.h"
#include <iostream>
#include <iomanip>
#include <ctime>

ParkingLot::ParkingLot() {
    // Initialize some slots
    slots.push_back(std::make_shared<RegularSlot>("R1", 1));
    slots.push_back(std::make_shared<RegularSlot>("R2", 1));
    slots.push_back(std::make_shared<EVSlot>("E1", 1));
    slots.push_back(std::make_shared<EVSlot>("E2", 1));
    slots.push_back(std::make_shared<VIPSlot>("V1", 2));
    slots.push_back(std::make_shared<VIPSlot>("V2", 2));
}

ParkingSlot* ParkingLot::findAvailableSlot(const Vehicle& v) {
    for (auto& slot : slots) {
        if (!slot->isOccupied() && slot->canPark(v)) {
            return slot.get();
        }
    }
    return nullptr;
}

std::shared_ptr<Ticket> ParkingLot::parkVehicle(std::shared_ptr<Vehicle> v) {
    ParkingSlot* slot = findAvailableSlot(*v);
    if (!slot) {
        throw NoAvailableSlotException();
    }
    
    try {
        slot->parkVehicle(v);
        auto ticket = std::make_shared<Ticket>(slot->getId(), v);
        tickets.push_back(ticket);
        return ticket;
    } catch (const ParkingException& e) {
        // Demonstrating rethrow
        std::cout << "Error in parkVehicle, rethrowing..." << std::endl;
        throw; // rethrow the same exception
    }
}

double ParkingLot::unparkVehicle(int ticketId) {
    for (size_t i = 0; i < tickets.size(); i++) {
        if (tickets[i]->getId() == ticketId && !tickets[i]->isPaid()) {
            time_t exitTime = time(0);
            double fee = tickets[i]->calculateFee(exitTime);
            
            // Find the slot and unpark
            for (auto& slot : slots) {
                if (slot->getId() == tickets[i]->getSlotId()) {
                    slot->unparkVehicle();
                    
                    // Clear VIP reservation if applicable
                    auto vipSlot = std::dynamic_pointer_cast<VIPSlot>(slot);
                    if (vipSlot && vipSlot->isReserved()) {
                        vipSlot->clearReservation();
                    }
                    break;
                }
            }
            
            tickets[i]->markPaid();
            payments.push_back({exitTime, fee});
            return fee;
        }
    }
    throw InvalidTicketException();
}

void ParkingLot::reserveVIP(const std::string& plate, int durationHours) {
    for (auto& slot : slots) {
        auto vipSlot = std::dynamic_pointer_cast<VIPSlot>(slot);
        if (vipSlot && !vipSlot->isOccupied() && !vipSlot->isReserved()) {
            vipSlot->reserve(plate, durationHours);
            std::cout << "VIP slot " << vipSlot->getId() << " reserved for " << plate << std::endl;
            return;
        }
    }
    throw ReservationException();
}

void ParkingLot::showOccupation() const {
    std::cout << "\n=== Current Occupation ===\n";
    for (const auto& slot : slots) {
        std::cout << "Slot " << slot->getId() 
                  << " (" << slot->getSlotType() 
                  << ", Level " << slot->getLevel() << "): ";
        
        if (slot->isOccupied()) {
            auto vehicle = slot->getVehicle();
            std::cout << "Occupied by " << vehicle->getTypeName()
                      << " (" << vehicle->getPlate() << ")";
        } else if (auto vipSlot = std::dynamic_pointer_cast<VIPSlot>(slot)) {
            if (vipSlot->isReserved()) {
                std::cout << "Reserved for " << vipSlot->getId();
            } else {
                std::cout << "Available";
            }
        } else {
            std::cout << "Available";
        }
        std::cout << std::endl;
    }
}

void ParkingLot::generateDailyReport() const {
    std::cout << "\n=== Daily Report ===\n";
    
    int totalVehicles = 0;
    int motorcycleCount = 0;
    int carCount = 0;
    int evCount = 0;
    double totalRevenue = 0;
    
    for (const auto& payment : payments) {
        totalRevenue += payment.second;
    }
    
    for (const auto& ticket : tickets) {
        if (ticket->isPaid()) {
            totalVehicles++;
            auto vehicle = ticket->getVehicle();
            switch(vehicle->getType()) {
                case VehicleType::MOTORCYCLE: motorcycleCount++; break;
                case VehicleType::CAR: carCount++; break;
                case VehicleType::EV: evCount++; break;
            }
        }
    }
    
    std::cout << "Total Vehicles: " << totalVehicles << std::endl;
    std::cout << "Motorcycles: " << motorcycleCount << std::endl;
    std::cout << "Cars: " << carCount << std::endl;
    std::cout << "EVs: " << evCount << std::endl;
    std::cout << "Total Revenue: Rp " << std::fixed << std::setprecision(2) << totalRevenue << std::endl;
    
    // Utilization per floor
    std::cout << "\nUtilization per Floor:\n";
    for (int floor = 1; floor <= 2; floor++) {
        int occupied = 0;
        int total = 0;
        for (const auto& slot : slots) {
            if (slot->getLevel() == floor) {
                total++;
                if (slot->isOccupied()) occupied++;
            }
        }
        double utilization = (total > 0) ? (occupied * 100.0 / total) : 0;
        std::cout << "Floor " << floor << ": " << std::setprecision(1) << utilization << "% (" 
                  << occupied << "/" << total << " slots)" << std::endl;
    }
}

void ParkingLot::saveTestData() {
    std::cout << "Test data saved (simulated)" << std::endl;
}

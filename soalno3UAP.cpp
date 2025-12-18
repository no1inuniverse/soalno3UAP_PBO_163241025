#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>

// ============================
// EXCEPTION HIERARCHY
// ============================
class ParkingException : public std::exception {
protected:
    std::string message;
public:
    ParkingException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
    virtual ~ParkingException() {}
};

class NoAvailableSlotException : public ParkingException {
public:
    NoAvailableSlotException() : ParkingException("No available slot for this vehicle type") {}
};

class InvalidTicketException : public ParkingException {
public:
    InvalidTicketException() : ParkingException("Invalid ticket ID") {}
};

class ReservationException : public ParkingException {
public:
    ReservationException() : ParkingException("VIP slot reservation failed") {}
};

class PaymentException : public ParkingException {
public:
    PaymentException() : ParkingException("Payment processing error") {}
};

// ============================
// VEHICLE HIERARCHY
// ============================
enum class VehicleType { MOTORCYCLE, CAR, EV };

class Vehicle {
protected:
    std::string plate;
    std::string owner;
    time_t arrivalTime;
    VehicleType type;
public:
    Vehicle(const std::string& p, const std::string& o, VehicleType t) 
        : plate(p), owner(o), type(t), arrivalTime(0) {}
    
    virtual ~Vehicle() {}
    
    std::string getPlate() const { return plate; }
    std::string getOwner() const { return owner; }
    VehicleType getType() const { return type; }
    time_t getArrivalTime() const { return arrivalTime; }
    
    void setArrivalTime(time_t t) { arrivalTime = t; }
    
    virtual double calculateRate(int durationHours) const = 0;
    
    virtual double calculateRate(time_t startTime, time_t endTime) const {
        int duration = (endTime - startTime) / 3600;
        if (duration < 1) duration = 1;
        return calculateRate(duration);
    }
    
    virtual std::string getTypeName() const {
        switch(type) {
            case VehicleType::MOTORCYCLE: return "Motorcycle";
            case VehicleType::CAR: return "Car";
            case VehicleType::EV: return "EV";
            default: return "Unknown";
        }
    }
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(const std::string& p, const std::string& o) 
        : Vehicle(p, o, VehicleType::MOTORCYCLE) {}
    
    double calculateRate(int durationHours) const override {
        double rate = durationHours * 2000;
        if (durationHours > 24) {
            rate *= 0.9; // 10% discount
        }
        return rate;
    }
};

class Car : public Vehicle {
public:
    Car(const std::string& p, const std::string& o) 
        : Vehicle(p, o, VehicleType::CAR) {}
    
    double calculateRate(int durationHours) const override {
        double rate = durationHours * 5000;
        if (durationHours > 24) {
            rate *= 0.9; // 10% discount
        }
        return rate;
    }
};

class EV : public Vehicle {
private:
    bool chargingRequired;
public:
    EV(const std::string& p, const std::string& o, bool charging) 
        : Vehicle(p, o, VehicleType::EV), chargingRequired(charging) {}
    
    bool needsCharging() const { return chargingRequired; }
    
    double calculateRate(int durationHours) const override {
        double rate = durationHours * 4000;
        if (chargingRequired) {
            rate += 3000; // charging fee
        }
        if (durationHours > 24) {
            rate *= 0.9; // 10% discount
        }
        return rate;
    }
};

// ============================
// PARKING SLOT HIERARCHY
// ============================
class ParkingSlot {
protected:
    std::string id;
    int level;
    bool occupied;
    std::shared_ptr<Vehicle> vehicle;
public:
    ParkingSlot(const std::string& slotId, int lvl) 
        : id(slotId), level(lvl), occupied(false), vehicle(nullptr) {}
    
    virtual ~ParkingSlot() {}
    
    std::string getId() const { return id; }
    int getLevel() const { return level; }
    bool isOccupied() const { return occupied; }
    std::shared_ptr<Vehicle> getVehicle() const { return vehicle; }
    
    virtual bool canPark(const Vehicle& v) const = 0;
    virtual std::string getSlotType() const = 0;
    
    void parkVehicle(std::shared_ptr<Vehicle> v) {
        if (occupied) {
            throw ParkingException("Slot already occupied");
        }
        vehicle = v;
        occupied = true;
        v->setArrivalTime(time(0));
    }
    
    void unparkVehicle() {
        vehicle.reset();
        occupied = false;
    }
};

class RegularSlot : public ParkingSlot {
public:
    RegularSlot(const std::string& id, int lvl) : ParkingSlot(id, lvl) {}
    
    bool canPark(const Vehicle& v) const override {
        return true; // Regular slot accepts all vehicles
    }
    
    std::string getSlotType() const override {
        return "Regular";
    }
};

class EVSlot : public ParkingSlot {
public:
    EVSlot(const std::string& id, int lvl) : ParkingSlot(id, lvl) {}
    
    bool canPark(const Vehicle& v) const override {
        return v.getType() == VehicleType::EV;
    }
    
    std::string getSlotType() const override {
        return "EV";
    }
};

class VIPSlot : public ParkingSlot {
private:
    bool reserved;
    std::string reservedPlate;
    time_t reservationEnd;
public:
    VIPSlot(const std::string& id, int lvl) 
        : ParkingSlot(id, lvl), reserved(false), reservationEnd(0) {}
    
    bool canPark(const Vehicle& v) const override {
        if (reserved) {
            return v.getPlate() == reservedPlate && time(0) < reservationEnd;
        }
        return true;
    }
    
    std::string getSlotType() const override {
        return "VIP";
    }
    
    bool isReserved() const { return reserved; }
    
    void reserve(const std::string& plate, int durationHours) {
        if (occupied) {
            throw ReservationException();
        }
        reserved = true;
        reservedPlate = plate;
        reservationEnd = time(0) + (durationHours * 3600);
    }
    
    void clearReservation() {
        reserved = false;
        reservedPlate = "";
        reservationEnd = 0;
    }
    
    double calculateRate(int durationHours) const {
        double baseRate = durationHours * 5000; // Car rate as base
        double vipRate = baseRate * 1.5; // 50% surcharge
        if (durationHours > 24) {
            vipRate *= 0.9; // 10% discount
        }
        return vipRate;
    }
};

// ============================
// TICKET/RECEIPT
// ============================
class Ticket {
private:
    static int nextId;
    int id;
    std::string slotId;
    std::shared_ptr<Vehicle> vehicle;
    time_t entryTime;
    bool paid;
public:
    Ticket(const std::string& slotId, std::shared_ptr<Vehicle> v) 
        : id(nextId++), slotId(slotId), vehicle(v), entryTime(time(0)), paid(false) {}
    
    int getId() const { return id; }
    std::string getSlotId() const { return slotId; }
    std::shared_ptr<Vehicle> getVehicle() const { return vehicle; }
    time_t getEntryTime() const { return entryTime; }
    bool isPaid() const { return paid; }
    void markPaid() { paid = true; }
    
    double calculateFee(time_t exitTime) const {
        int duration = (exitTime - entryTime) / 3600;
        if (duration < 1) duration = 1;
        
        // Check if it's in VIP slot
        if (slotId.find("V") != std::string::npos) { // VIP slot starts with V
            VIPSlot vip("", 0);
            return vip.calculateRate(duration);
        }
        
        return vehicle->calculateRate(duration);
    }
};

int Ticket::nextId = 1000;

// ============================
// PARKING LOT
// ============================
class ParkingLot {
private:
    std::vector<std::shared_ptr<ParkingSlot>> slots;
    std::vector<std::shared_ptr<Ticket>> tickets;
    std::vector<std::pair<time_t, double>> payments; // timestamp, amount
    
    ParkingSlot* findAvailableSlot(const Vehicle& v) {
        for (auto& slot : slots) {
            if (!slot->isOccupied() && slot->canPark(v)) {
                return slot.get();
            }
        }
        return nullptr;
    }
    
public:
    ParkingLot() {
        // Initialize some slots
        slots.push_back(std::make_shared<RegularSlot>("R1", 1));
        slots.push_back(std::make_shared<RegularSlot>("R2", 1));
        slots.push_back(std::make_shared<EVSlot>("E1", 1));
        slots.push_back(std::make_shared<EVSlot>("E2", 1));
        slots.push_back(std::make_shared<VIPSlot>("V1", 2));
        slots.push_back(std::make_shared<VIPSlot>("V2", 2));
    }
    
    std::shared_ptr<Ticket> parkVehicle(std::shared_ptr<Vehicle> v) {
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
    
    double unparkVehicle(int ticketId) {
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
    
    void reserveVIP(const std::string& plate, int durationHours) {
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
    
    void showOccupation() const {
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
    
    void generateDailyReport() const {
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
    
    void saveTestData() {
        // This would normally save to file, but per requirements we keep it simple
        std::cout << "Test data saved (simulated)" << std::endl;
    }
};

// ============================
// USER INTERFACE
// ============================
class ParkingSystem {
private:
    ParkingLot lot;
    
    void registerAndPark() {
        try {
            std::string plate, owner;
            int typeChoice;
            bool charging = false;
            
            std::cout << "\nEnter vehicle plate: ";
            std::cin.ignore(); // Clear buffer
            std::getline(std::cin, plate);
            std::cout << "Enter owner name: ";
            std::getline(std::cin, owner);
            
            std::cout << "Vehicle type:\n1. Motorcycle\n2. Car\n3. EV\nChoice: ";
            std::cin >> typeChoice;
            
            std::shared_ptr<Vehicle> vehicle;
            
            switch(typeChoice) {
                case 1:
                    vehicle = std::make_shared<Motorcycle>(plate, owner);
                    break;
                case 2:
                    vehicle = std::make_shared<Car>(plate, owner);
                    break;
                case 3:
                    std::cout << "Need charging? (1=Yes, 0=No): ";
                    int chargeChoice;
                    std::cin >> chargeChoice;
                    charging = (chargeChoice == 1);
                    vehicle = std::make_shared<EV>(plate, owner, charging);
                    break;
                default:
                    std::cout << "Invalid choice!" << std::endl;
                    return;
            }
            
            auto ticket = lot.parkVehicle(vehicle);
            std::cout << "Vehicle parked successfully!\n";
            std::cout << "Ticket ID: " << ticket->getId() << std::endl;
            
        } catch (const ParkingException& e) {
            std::cout << "Error: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Unknown error occurred!" << std::endl;
        }
    }
    
    void unparkVehicle() {
        try {
            int ticketId;
            std::cout << "\nEnter ticket ID: ";
            std::cin >> ticketId;
            
            double fee = lot.unparkVehicle(ticketId);
            std::cout << "Vehicle unparked successfully!\n";
            std::cout << "Total fee: Rp " << std::fixed << std::setprecision(2) << fee << std::endl;
            
        } catch (const ParkingException& e) {
            std::cout << "Error: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Unknown error occurred!" << std::endl;
        }
    }
    
    void reserveVIP() {
        try {
            std::string plate;
            int duration;
            
            std::cout << "\nEnter vehicle plate for reservation: ";
            std::cin.ignore();
            std::getline(std::cin, plate);
            std::cout << "Enter reservation duration (hours): ";
            std::cin >> duration;
            
            lot.reserveVIP(plate, duration);
            
        } catch (const ParkingException& e) {
            std::cout << "Error: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Unknown error occurred!" << std::endl;
        }
    }
    
public:
    void run() {
        int choice;
        
        do {
            std::cout << "\n=== Parking Management System ===" << std::endl;
            std::cout << "1. Register vehicle and park" << std::endl;
            std::cout << "2. Unpark vehicle" << std::endl;
            std::cout << "3. Reserve VIP slot" << std::endl;
            std::cout << "4. Show current occupation" << std::endl;
            std::cout << "5. Generate daily report" << std::endl;
            std::cout << "6. Save test data" << std::endl;
            std::cout << "0. Exit" << std::endl;
            std::cout << "Choice: ";
            std::cin >> choice;
            
            switch(choice) {
                case 1:
                    registerAndPark();
                    break;
                case 2:
                    unparkVehicle();
                    break;
                case 3:
                    reserveVIP();
                    break;
                case 4:
                    lot.showOccupation();
                    break;
                case 5:
                    lot.generateDailyReport();
                    break;
                case 6:
                    lot.saveTestData();
                    break;
                case 0:
                    std::cout << "Goodbye!" << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice!" << std::endl;
            }
            
        } while (choice != 0);
    }
};

// ============================
// MAIN FUNCTION
// ============================
int main() {
    ParkingSystem system;
    system.run();
    return 0;
}

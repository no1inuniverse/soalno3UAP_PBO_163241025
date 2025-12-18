#include "ParkingSystemUI.h"
#include <iostream>
#include <iomanop>
#include <string>

void ParkingSystemUI::registerAndPark() {
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

void ParkingSystemUI::unparkVehicle() {
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

void ParkingSystemUI::reserveVIP() {
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

void ParkingSystemUI::run() {
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

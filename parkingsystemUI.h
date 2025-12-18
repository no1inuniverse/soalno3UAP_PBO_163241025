#ifndef PARKINGSYSTEMUI_H
#define PARKINGSYSTEMUI_H

#include "ParkingLot.h"

class ParkingSystemUI {
private:
    ParkingLot lot;
    
    void registerAndPark();
    void unparkVehicle();
    void reserveVIP();
    
public:
    void run();
};

#endif

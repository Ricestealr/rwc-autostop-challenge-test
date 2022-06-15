#include <mbed.h>
#include "motor.h"
#include "definitions.h"
class Brakes{
    public:
    Brakes();
    
    void ParkMode();
    void BrakesOn();
    void FrontBrakeOn();
    void RearBrakeOn();
    void MechanicalBraking(int brakeRate, Motor motor);
    void RegenControl(int ratecontrol, Motor motor);
    };
    
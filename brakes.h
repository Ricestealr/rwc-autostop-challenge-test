#include <mbed.h>
#include "motor.h"
#include "rtc.h"
#include "definitions.h"
class Brakes{ // Separate brakes class to implement friction and regen braking
    public:
    Brakes(); //default constructor
    
    void ParkMode(Motor motor); //Park Mode Definition
    void BrakesOn(); //Turns on Both Mechanical BRakes
    void FrontBrakeOn(); //Only turns on Front Mechanical Brake
    void RearBrakeOn(); // Only turns on Rear Mechanical Brake
    void MechanicalBraking(int brakeRate, Motor motor); //Mechanical Braking Function when train in operation
    void RegenControl(int ratecontrol, Motor motor); //Regen Function
    void EmergencyStop(Motor motor, RoundTrainCircuit rtc, bool emergencyStopActive);
    
    };
    
    
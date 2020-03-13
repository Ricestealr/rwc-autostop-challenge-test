#include <mbed.h>
#include "definitions.h"
#include "motor.h"

Motor::Motor   (AnalogOut& motorAccelerator,
                AnalogOut& motorBrake,
                DigitalOut& keySwitch,
                DigitalOut& directionFwd,
                DigitalOut& directionRev,
                DigitalOut& footSwitch,
                DigitalOut& seatSwitch,
                DigitalOut& inchFwd,
                DigitalOut& speedLimit2,
                DigitalOut& speedLimit3) : 
                
                _motorAccelerator(motorAccelerator),
                _motorBrake(motorBrake),
                _keySwitch(keySwitch),
                _directionFwd(directionFwd),
                _directionRev(directionRev),
                _footSwitch(footSwitch),
                _seatSwitch(seatSwitch),
                _inchFwd(inchFwd),
                _speedLimit2(speedLimit2),
                _speedLimit3(speedLimit3)
                {
                    _motorAccelerator = 0;
                    _motorBrake = 0;
//                    _contactMtr = 0;
                    _keySwitch = 0;
                    _directionFwd = 0;
                    _directionRev = 0;
                    _footSwitch = 0;
                    _seatSwitch = 0;
                    _inchFwd = 1;
                    _speedLimit2 = 1;
                    _speedLimit3 = 0;   // SET MAX SPEED MODE BY DEFAULT
                }


void Motor::turnOn() {
    _keySwitch = 1;
//    pc.printf("Motor Switched On\r\n");  
}

void Motor::turnOff() {
    _keySwitch = 0;
//    pc.printf("Motor Switched Off\r\n");  
}

void Motor::closeDeadman() {
    _footSwitch = 1;
    _seatSwitch = 1;
}

void Motor::releaseDeadman() {
    _footSwitch = 0;
    _seatSwitch = 0;
}

void Motor::setForward() {
    _directionFwd = 1;
    _directionRev = 0;
//    pc.printf("Motor Set to Forward\r\n");  
}

void Motor::setPark() {
    _directionFwd = 0;
    _directionRev = 0;
    
//    pc.printf("Motor Set to Park\r\n"); 
}

void Motor::setReverse() {
    _directionFwd = 0;
    _directionRev = 1;
//    pc.printf("Motor Set to Reverse\r\n"); 
}

void Motor::engage() {
//    _contactMtr = 1;    
//    pc.printf("Motor Contactor Engaged\r\n");   
}

void Motor::disengage() {
//    _contactMtr = 0;
    
    setPark();
    
//    pc.printf("Motor Disengaged\r\n");  
}

void Motor::setSpeedMode(int speed) {
    switch (speed) {
        case 0:     // Clear limits
        
            _inchFwd = 0;
            _speedLimit2 = 0;
            _speedLimit3 = 0;
            
//                    pc.printf("Motor Speed Cleared\r\n");
            break;
        
        case 1:     // Inch Forward
        
            _inchFwd = 1;
            _speedLimit2 = 0;
            _speedLimit3 = 0;
            
//                    pc.printf("Motor Set to Inch Forward\r\n");
            break;
            
        case 2:     // Speed 2
            
            _inchFwd = 0;
            _speedLimit2 = 1;
            _speedLimit3 = 0;
            
//                    pc.printf("Motor Set to Speed Limit 2\r\n");
            break;
            
        case 3:     // Speed 3
            
            _inchFwd = 0;
            _speedLimit2 = 0;
            _speedLimit3 = 1;
            
//            pc.printf("Motor Set to Speed Limit 3\r\n");
            break;   
    }
}

void Motor::throttle(float throttleRate) {
    // set the output voltage of the analog output pin specified as a percentage of Vcc (3.3V)
    _motorAccelerator = throttleRate;   // Analog value between 0.0f and 1.0f
//    pc.printf("Throttling\r\n");
    
}

void Motor::brake(float brakeRate) {
    _motorBrake = brakeRate;            // Analog value between 0.0f and 1.0f
//    pc.printf("Motor Braking\r\n");
}
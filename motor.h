#ifndef MOTOR_H
#define MOTOR_H

#include <mbed.h>

class Motor {
    public:
        Motor  (AnalogOut& motorAccelerator,
                AnalogOut& motorBrake,
                DigitalOut& keySwitch,
                DigitalOut& directionFwd,
                DigitalOut& directionRev,
                DigitalOut& footSwitch,
                DigitalOut& seatSwitch,
                DigitalOut& inchFwd,
                DigitalOut& speedLimit2,
                DigitalOut& speedLimit3);
                
        void turnOn();
        void turnOff();
        void closeDeadman();
        void releaseDeadman();
        void setForward();
        void setPark();
        void setReverse();
        void engage();
        void disengage();
        void setSpeedMode(int speed);
        void throttle(float throttleRate);
        void brake(float brakeRate);
        
    private:
        AnalogOut& _motorAccelerator;
        AnalogOut& _motorBrake;
        DigitalOut& _keySwitch;
        DigitalOut& _directionFwd;
        DigitalOut& _directionRev;
        DigitalOut& _footSwitch;
        DigitalOut& _seatSwitch;
        DigitalOut& _inchFwd;
        DigitalOut& _speedLimit2;
        DigitalOut& _speedLimit3;
};

#endif

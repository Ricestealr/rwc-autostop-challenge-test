#ifndef _CHALLENGE_H_
#define _CHALLENGE_H_

#include <mbed.h>
#include "dashboard.h"
#include "remoteControl.h"
#include "motor.h"

class ChallengeMode {
    public:
        // CONSTURCTOR
        ChallengeMode(InterruptIn& autoStopTrigger, Dashboard& dashboard, Remote& remote, Motor& motor1); 
        
        // FUNCTIONS
        // Regen Mode
        void regenThrottleOn();
        void regenThrottleOff();
        bool regenBrakingOn();
        void regenBrakingOff();
        
        // Auto-Stop Mode
        void autoStopOn();
        void autoStopOff();
        void autoStopTriggered();
        void autoStopControl();
        
        // Innovation Mode
        void innovationOn();
        void innovationOff();
        int innovationControl(int requestedThrottle);
        
        // VARIABLES
        bool regenThrottleActive;
        bool regenBrakingActive;
        bool autoStopActive;
        bool innovationActive;
        
        int autoStopCruiseSpeed;
        int autoStopThrottle;
        bool autoStopInProgress;
    
    private:
        InterruptIn& _autoStopTrigger;
        Dashboard& _dashboard;
        Remote& _remote;
        Motor& _motor1;
//        Motor& _motor2;
        
        // AutoStop
        float targetDistance;
        float remainingDistance;

//        float timeToReachTarget;
        float decelerationGradient;
        float requiredSpeed;
        
        // Innovation
        int innovationDistanceLimit;    // Distance(mm) from IR sensors to apply brakes
        bool stopLoco;
        
        // IR Output voltages at various distances (for calibration purposes)
        float voltageAt5500;
        float voltageAt5000;
        float voltageAt4500;
        float voltageAt4000;
        float voltageAt3500;
        float voltageAt3000;
        float voltageAt2500;
        float voltageAt2000;
        float voltageAt1500;
        float voltageAt1000;
        float voltageAt500;
};

#endif
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
        
        
        // VARIABLES
        bool regenThrottleActive;
        bool regenBrakingActive;
        bool autoStopActive;
        

        bool autoStopInProgress;
    
    private:
        InterruptIn& _autoStopTrigger;
        Dashboard& _dashboard;
        Remote& _remote;
        Motor& _motor1;
        
        // AutoStop
        float targetDistance;
        float remainingDistance;

//        float timeToReachTarget;
        float decelerationGradient;
        float requiredSpeed;
        
       
};

#endif

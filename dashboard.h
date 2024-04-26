#ifndef _DASHBOARD_H_
#define _DASHBOARD_H_

#include <mbed.h>

class Dashboard {
    public:
        Dashboard(InterruptIn& hallSensor);    // CONSTRUCTOR
        
        void tachoInterrupt();
        void getCurrentSpeed();
        
        int currentSpeed;
        float currentDistance;
        float passedTime;
        
    private:
        InterruptIn& _hallSensor;
        Timer tachoTimer;
        int passedTime_ms;
        //float passedTime;
        float lastTime;
        float wheelFreq;
        float wheelCircumference;
        int numberOfMagnets;
        int tachoCounter;
        int currentTime;
};

#endif

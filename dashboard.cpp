#include <mbed.h>
#include "definitions.h"
#include "dashboard.h"

Dashboard::Dashboard(InterruptIn& hallSensor) : _hallSensor(hallSensor) {    // CONSTRUCTOR
    
    _hallSensor.rise(this, &Dashboard::tachoInterrupt);           // Register everytime hall sensor is detected
    tachoTimer.start();                         // Timer for counting time between hall sensor triggers
    
    numberOfMagnets = 8;    // How many magnets on the wheels
    
    currentSpeed = 0.00f;
    passedTime_ms = 0.00f;
    passedTime = 0.00f;
    lastTime = 0.00f;
    wheelCircumference = 0.73513268f; // in meters. Radius = 0.117m
    
    currentDistance = 0.00f;
    tachoCounter = 0;
    currentTime = 0;
}

void Dashboard::tachoInterrupt() 
{
    
    currentTime = tachoTimer.read_ms();
    passedTime = (currentTime - lastTime) / 1000;
    pc.printf("Passed Time: %.2f\r\n", passedTime);
    currentDistance += (wheelCircumference / numberOfMagnets);
    pc.printf("Current Distance = %.2f\r\n", currentDistance);
    wheelFreq = 1 / (passedTime * numberOfMagnets);
    lastTime = currentTime;
    if (passedTime > 0.00f) 
    {    // Stops dividing by 0
        // 1 Hz FREQ = 1 RPS -> x60 for minutes
        float rpm = wheelFreq * 60.00f;       /// MAX RPM AT 15KPH = 340RPM
        float kph = (wheelCircumference * rpm * (60.00f / 1000.00f));
        printf("Speed = %.2f\r\n", kph);
        currentSpeed = int(kph);

    }

}

void Dashboard::getCurrentSpeed() 
{
    
    // USE THESE
//    tachoCounter
//    currentTime
//    lastTime

    currentTime = tachoTimer.read_ms();
    if ((currentTime - lastTime) > 5000) 
    {
        currentSpeed = 0;
    }

                

}
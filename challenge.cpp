#include <mbed.h>
#include "definitions.h"
#include "challenge.h"
#include "remoteControl.h"
#include "dashboard.h"
#include "motor.h"

ChallengeMode::ChallengeMode(InterruptIn& autoStopTrigger, Dashboard& dashboard, Remote& remote, Motor& motor1) : 
                            _autoStopTrigger(autoStopTrigger), _dashboard(dashboard), _remote(remote), _motor1(motor1) {
    // CONSTRUCTOR
                                
    // ATTACH AUTOSTOP INTERRUPT
    _autoStopTrigger.rise(this, &ChallengeMode::autoStopTriggered);
    
    // FUNCTIONS
    regenThrottleOff(); // Make sure regen throttle is off
    autoStopOff();      // Make sure auto-stop is off
    
    // VARIABLES
    
    contactBatt = 1;
    
    // Auto-Stop Challenge
    autoStopActive = false;             // Flag is auto-stop mode is active
    autoStopInProgress = false;         // Flag if auto-stop track-side sensor has been detected and auto-stop is in progress
    targetDistance = 25.00f;            // How far in meters to bring the loco to a stop
    remainingDistance = targetDistance; // How far the loco has left to go before reaching target distance
    decelerationGradient = 0;           // Gradient of y=mx+c linear speed-distance curve that is used in this auto-stop version
    requiredSpeed = 0;                  // How fast the loco should be going according to y=mx+c line
    
    
}    // CONSTRUCTOR

void ChallengeMode::regenThrottleOn() 
{
    // OPEN THE BATTERY CONTACTOR SO POWER IS DELIVERED BY SUPERCAPS
    
    contactCapCharge = 0;               // Open supercap charging contactor to prevent charging
    
    contactBatt = 0;                    // Open battery contactor so all power comes from supercaps
    regenThrottleActive = true;         // Flag to indicate that regen throttling is on
    pc.printf("Regen Throttle On\r\n");
}

void ChallengeMode::regenThrottleOff() 
{
    // CLOSE THE BATTERY CONTACTOR
    if (regenBrakingActive == false) 
    {  // If regen braking is not active and using the supercaps, allow capacitor to pre-charge from batteries
        contactCapCharge = 1;           // Close the supercap charging contactor
    }
    
    contactBatt = 1;                    // Close the battery contactor so power comes from batteries
    regenThrottleActive = false;        // Flag to indicate that regen throttling is off
    pc.printf("Regen Throttle Off\r\n");
}

bool ChallengeMode::regenBrakingOn() 
{
    // TURN ON REGEN BRAKING
    //if (superCapVoltage == 0) 
    //{             // If super caps are not full
        contactCapCharge = 0;               // Open the super cap pre-cahrging contactor
        contactBatt = 1;                    // Open battery contactor so all power comes from supercaps
        regenBrakingActive = true;          // Flag to indicate that regen throttling is on
        pc.printf("Regen Braking On\r\n");
        return 1;                           // Return 1 if regen braking switched on
    //}
    /*else 
    {
        return 0;                           // Return 0 is regen braking didnt turn on due to full supercaps
    }*/
}

void ChallengeMode::regenBrakingOff() 
{
    // TURN OFF REGEN BRAKING
    if (regenThrottleActive == false) 
    {     // If regen throttle is not active and using the supercaps, allow capacitor to pre-charge from batteries
        contactCapCharge = 1;               // Close the supercap pre-charge contactor
    }
    regenBrakingActive = false;             // Flag that regen braking is off
    contactBatt = 1;                    // Close battery contactor so all power comes from supercaps
    pc.printf("Regen Braking Off\r\n");
}

void ChallengeMode::autoStopOn() 
{
    // TURN ON AUTOSTOP MODE
    autoStopActive = true;                  // Flag that auto-stop mode is on
    //pc.printf("Auto-Stop On\r\n");
}

void ChallengeMode::autoStopTriggered()
{
    // INTERRUPT FUNCTION CALLED WHEN TRACK-SIDE TRIGGER HAS BEEN DETECTED
    if (autoStopActive == true && autoStopInProgress == false) 
    {    // If auto-stop mode is on and signal has not yet been detected
        autoStopInProgress = true;                                  // Flag that auto-stop is in progress and fully autonomous
        whistle = 1;
        _dashboard.currentDistance = 0.00f;                         // Reset the distance-travelled counter to 0
        //pc.printf("Auto-Stop Triggered\r\n");
    }
}

void ChallengeMode::autoStopControl() 
{
    // FUNCTION TO MANAGE THE LOCO THROTTLE AND BRAKING WHEN AUTO-STOPPING
    remainingDistance = targetDistance - _dashboard.currentDistance;        // Calculate remaining distance from target distance
    
    // FOLLOWING DECELERATION GRADIENT
    
    if (remainingDistance > 2.0f) 
    {    // IF OVER 3M FROM TARGET, CONTROL SPEED (1m + 2m sensor to nose of train)
    
        _motor1.throttle(0.3f);     // Apply 30% throttle

    }// if 
    else 
    {  // IF REACHED STOPPING TARGET AREA
        rtc_Trigger = 0;        // APPLY EMERGENCY BRAKES

    }

    /*switch((int)remainingDistance)
    {
        case 15 ... 25 : 
            _motor1.throttle(0.7f);
            break;
        case 10 ... 14:
            _motor1.throttle(0.3f);
            break;
        case 4 ... 9:
            _motor1.throttle(0.1f);
            break;
        case 0 ... 3:
            rtc_Trigger = 0;
            break;
    }*/
    
    //pc.printf("remainingDistance  %f\r\n", remainingDistance);
}

void ChallengeMode::autoStopOff() 
{
    // TURN OFF AUTOSTOP MODE
    autoStopActive = false;         // CLEAR AUTOSTOP MODE FLAG
    autoStopInProgress = false;     // CLEAR AUTOSTOPPING
    rtc_Trigger = 1;
    //pc.printf("Auto-Stop Off\r\n");
}
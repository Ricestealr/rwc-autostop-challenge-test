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
    innovationOff();    // make sure innovation (collision detection mode) is off
    
    // VARIABLES
    
    contactBatt = 1;
    
    // Auto-Stop Challenge
    autoStopActive = false;             // Flag is auto-stop mode is active
    autoStopInProgress = false;         // Flag if auto-stop track-side sensor has been detected and auto-stop is in progress
    autoStopCruiseSpeed = 0;            // Speed of loco at time of auto-stop trackside sensor triggering so loco can maintain this speed without operator input
    autoStopThrottle = 0;               // Throttle rate applied at the time of triggering autostop
    targetDistance = 25.00f;            // How far in meters to bring the loco to a stop
    remainingDistance = targetDistance; // How far the loco has left to go before reaching target distance
    decelerationGradient = 0;           // Gradient of y=mx+c linear speed-distance curve that is used in this auto-stop version
    requiredSpeed = 0;                  // How fast the loco should be going according to y=mx+c line
    
    // Innovation Challenge
    innovationDistanceLimit = 1500;     // Stopping distance in mm from IR sensors
    stopLoco = false;                   // Flag to stop the loco when obstacle has been detected
    
    // IR Output voltages at various distances (for calibration purposes)
    voltageAt5500 = 0;
    voltageAt5000 = 0;
    voltageAt4500 = 0;
    voltageAt4000 = 0;
    voltageAt3500 = 0;
    voltageAt3000 = 0;
    voltageAt2500 = 0;
    voltageAt2000 = 0;
    voltageAt1500 = 0;
    voltageAt1000 = 2.2f;
    voltageAt500  = 3.0f;
    
}    // CONSTRUCTOR

void ChallengeMode::regenThrottleOn() {
    // OPEN THE BATTERY CONTACTOR SO POWER IS DELIVERED BY SUPERCAPS
    
    contactCapCharge = 0;               // Open supercap charging contactor to prevent charging
    
    contactBatt = 0;                    // Open battery contactor so all power comes from supercaps
    regenThrottleActive = true;         // Flag to indicate that regen throttling is on
    pc.printf("Regen Throttle On\r\n");
}

void ChallengeMode::regenThrottleOff() {
    // CLOSE THE BATTERY CONTACTOR
    if (regenBrakingActive == false) {  // If regen braking is not active and using the supercaps, allow capacitor to pre-charge from batteries
        contactCapCharge = 1;           // Close the supercap charging contactor
    }
    
    contactBatt = 1;                    // Close the battery contactor so power comes from batteries
    regenThrottleActive = false;        // Flag to indicate that regen throttling is off
    pc.printf("Regen Throttle Off\r\n");
}

bool ChallengeMode::regenBrakingOn() {
    // TURN ON REGEN BRAKING
//     if (superCapVoltage == 0) {             // If super caps are not full
//      contactCapCharge = 0;               // Open the super cap pre-cahrging contactor
//        contactBatt = 0;                    // Open battery contactor so all power comes from supercaps
        regenBrakingActive = true;          // Flag to indicate that regen throttling is on
        pc.printf("Regen Braking On\r\n");
        return 1;                           // Return 1 if regen braking switched on
//    }
//    else {
//        return 0;                           // Return 0 is regen braking didnt turn on due to full supercaps
//    }
}

void ChallengeMode::regenBrakingOff() {
    // TURN OFF REGEN BRAKING
    if (regenThrottleActive == false) {     // If regen throttle is not active and using the supercaps, allow capacitor to pre-charge from batteries
        contactCapCharge = 1;               // Close the supercap pre-charge contactor
    }
    regenBrakingActive = false;             // Flag that regen braking is off
    contactBatt = 1;                    // Close battery contactor so all power comes from supercaps
    pc.printf("Regen Braking Off\r\n");
}

void ChallengeMode::autoStopOn() {
    // TURN ON AUTOSTOP MODE
    autoStopActive = true;                  // Flag that auto-stop mode is on
    pc.printf("Auto-Stop On\r\n");
}

void ChallengeMode::autoStopTriggered() {
    // INTERRUPT FUNCTION CALLED WHEN TRACK-SIDE TRIGGER HAS BEEN DETECTED
    if (autoStopActive == true && autoStopInProgress == false) {    // If auto-stop mode is on and signal has not yet been detected
        autoStopInProgress = true;                                  // Flag that auto-stop is in progress and fully autonomous
        autoStopCruiseSpeed = _dashboard.currentSpeed;              // Set the speed the loco was going at the point of triggering
        autoStopThrottle = _remote.throttle;
        
        _dashboard.currentDistance = 0.00f;                         // Reset the distance-travelled counter to 0
//        timeToReachTarget = targetDistance / (autoStopCruiseSpeed * 1000 / 3600);   // Time(s) = Distance(m) / Speed(converted to m/s)
        decelerationGradient = (autoStopCruiseSpeed - 1) / (targetDistance - 1);    // m = y / x → to get to 1kph at 24m, leaving ~ 4 seconds to get to target of 25m
        pc.printf("Auto-Stop Triggered\r\n");
    }
}

void ChallengeMode::autoStopControl() {
    // FUNCTION TO MANAGE THE LOCO THROTTLE AND BRAKING WHEN AUTO-STOPPING
    remainingDistance = targetDistance - _dashboard.currentDistance;        // Calculate remaining distance from target distance
//    timeToReachTarget = int(_dashboard.currentSpeed) > 0 ? remainingDistance / _dashboard.currentSpeed : 999;
    
    // FOLLOWING DECELERATION GRADIENT
    
    if (remainingDistance > 2.0f) {    // IF OVER 3M FROM TARGET, CONTROL SPEED (1m + 2m sensor to nose of train)
    
        _motor1.throttle(0.3f);     // Apply 30% throttle
        //requiredSpeed = (decelerationGradient * _dashboard.currentDistance) + autoStopCruiseSpeed;  // (y = mx + c)
//        
//        if (_dashboard.currentSpeed < requiredSpeed) {  // If train is below the speed it should be, apply 30% throttle and no braking
////            _motor1.throttle(0.3f);
////            _motor2.throttle(0.1f);
////            _motor1.brake(0.0f);
////            _motor2.brake(0.0f);
//
//            brakeValve32 = 1;
//        }
//        else {                                                              // If loco is going faster than is should be
//            int speedDifference = _dashboard.currentSpeed - requiredSpeed;  // Work out how much faster it is going and apply a proportional amount of motor braking
//            
//            _motor1.throttle(0.0f);         // Turn off throttle
////            _motor2.throttle(0.0f);
//
//            brakeValve32 = 0;
//            if (pressureSwitch1.read() == 0) {
//                brakeValve22 = 0;
//                pc.printf("Pressure 1 Reached");
//            }
//            
//            switch (speedDifference) {  // MOTOR BRAKING
//                case 1:
//                    _motor1.brake(0.1f);
////                    _motor2.brake(0.1f);
//                    break;
//                    
//                case 2:
//                    _motor1.brake(0.2f);
////                    _motor2.brake(0.2f);
//                    break;
//                    
//                case 3:
//                    _motor1.brake(0.3f);
////                    _motor2.brake(0.3f);
//                    break;
//                    
//                case 4:
//                    _motor1.brake(0.4f);
////                    _motor2.brake(0.4f);
//                    break;
//                    
//                case 5:
//                    _motor1.brake(0.5f);
////                    _motor2.brake(0.5f);
//                    break;
//                    
//                case 6:
//                    _motor1.brake(0.6f);
////                    _motor2.brake(0.6f);
//                    break;
//                    
//                case 7:
//                    _motor1.brake(0.7f);
////                    _motor2.brake(0.7f);
//                    break;
//                    
//                case 8:
//                    _motor1.brake(0.8f);
////                    _motor2.brake(0.8f);
//                    break;
//                    
//                case 9:
//                    _motor1.brake(0.9f);
////                    _motor2.brake(0.9f);
//                    break;
//                    
//                default:
//                    _motor1.brake(1.0f);
////                    _motor2.brake(1.0f);
//                    break;
//            }   // switch
//        }// else
    }// if 
    else {  // IF REACHED STOPPING TARGET AREA
        rtc_Trigger = 0;        // APPLY EMERGENCY BRAKES
//        if (remainingDistance > 0.2f) {     // If more than 20cm to go, stay at 1kph
//        
//        }
//        else {                              // If less than 20cm from target apply full mechanical brakes and turn off throttle
//            _motor1.throttle(0.0f);
////            _motor2.throttle(0.0f);
//            
//            // MECHANICAL BRAKES
//            brakeValve32 = 1;
//            if (pressureSwitch3 == 0) {
//                brakeValve22 = 0;
//            }
//            else {
//                brakeValve22 = 1;   
//            }
//        }
    }
    
    pc.printf("remainingDistance  %f\r\n", remainingDistance);
//    pc.printf("timeToReachTarget %f\r\n", timeToReachTarget);
}

void ChallengeMode::autoStopOff() {
    // TURN OFF AUTOSTOP MODE
    autoStopActive = false;         // CLEAR AUTOSTOP MODE FLAG
    autoStopInProgress = false;     // CLEAR AUTOSTOPPING
    rtc_Trigger = 1;
    pc.printf("Auto-Stop Off\r\n");
}

void ChallengeMode::innovationOn() {
    // TURN ON INNOVATION MODE
    innovationActive = true;
    stopLoco = false;
    rtc_Trigger = 1;
    _motor1.setSpeedMode(1);     // SET MOTORS TO INCH FORWARD MODE
//    _motor2.setSpeedMode(1);
    pc.printf("Innovation On\r\n");
}

void ChallengeMode::innovationOff() {
    // TURN OFF INNOVATION MODE
    innovationActive = false;
    stopLoco = false;
    rtc_Trigger = 1;
    _motor1.setSpeedMode(3);     // SET MOTOR SPEED LIMIT TO MAX
    pc.printf("Innovation Off\r\n");
}

int ChallengeMode::innovationControl(int requestedThrottle) {
    // CONTROL THE TRAIN THROTTLING AND BRAKING
    
    int count = 0;
    
    if (irSensor_1 > voltageAt1000 / 3.3f) { count++; }
    if (irSensor_2 > voltageAt1000 / 3.3f) { count++; }
    if (irSensor_3 > voltageAt1000 / 3.3f ) { count++; }
    
    if (count >= 2) {
        stopLoco = true;
    }

    
    if (stopLoco == true) { // IF SENSORS INDICATE TRAIN SHOULD STOP
    
//        // APPLY MECHANICAL BRAKES
//        brakeValve32 = 0;
//        brakeValve22 = 1;

        // APPLY E-BRAKE
        rtc_Trigger = 0;
        
        pc.printf("Obstacle Detected\r\n");
        
        return 0;       // RETURN THROTTLE = 0 TO INDICATE TRAIN SHOULD STOP
    }
    else {
        return requestedThrottle;   // OTHERWISE THROTTLE = USER REQUEST
    }
}
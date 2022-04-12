#include <mbed.h>
#include "millis.h"

#include "definitions.h"
#include "remoteControl.h"
#include "dashboard.h"
#include "rtc.h"
#include "motor.h"
#include "challenge.h"

//  SET UP REMOTE CONTROL COMMS
SPI remoteControl(PE_14, PE_13, PE_12);   // (SPI_MOSI, SPI_MISO, SPI_SCK)
DigitalOut remoteControlCS(PE_11);    // (SPI_SS)

//  CREATE OBJECTS
Remote remote(remoteControl, remoteControlCS);
Dashboard dashboard(hallSensor);
RoundTrainCircuit rtc(rtc_1, rtc_2, rtc_3, rtc_4, rtc_5, rtc_6, rtc_7, rtc_override);
Motor motor1(motorAccelerator, motorBrake, keySwitchM1, directionFwd, directionRev, footswitchM1, seatM1, inchFwdM1, speedLimit2M1, speedLimit3M1);
ChallengeMode challenge(autoStopTrigger, dashboard, remote, motor1);

int driveMode = 2;      // Drive mode - fwd(0), rev(1), park(2)
bool emergencyStopActive = false;

// FUNCTIONS

void startupHealthCheck() {
       
   while(1) {

        if (remote.commsGood == 1) {
            if (rtc_output.read() == 0) {
//                if (batteryVoltage == true) {
                    if (superCapVoltage == true) {
//                        
//
                        return;     // Exit the function if all checks are passed
//
                    }
                    else {
                        pc.printf("System Start-Up Health Check: SuperCap Voltage Check Failed\r\n");
                    }
//                }
//                else {
//                    pc.printf("System Start-Up Health Check: Battery Voltage Check Failed\r\n");
//                }
            }
            else {
                pc.printf("System Start-Up Health Check: RTC Check Failed\r\n");
            }           
        }
        else {
            pc.printf("System Start-Up Health Check: CommsCheck Failed\r\n");
        }
        remote.sendError('H');     // Send error to remote
        wait(100); // Wait a while until trying again
    }
}

void emergencyStop() {
//    pc.printf("Emergency Stop Active\r\n");
    if (emergencyStopActive == false) {
        
        emergencyStopActive = true;
                
        motor1.disengage();     // Disengage both motors
//        motor2.disengage();
        
        motor1.setPark();       // Clear Motor Directions
//        motor2.setPark();
        
        if (rtc_output.read() == 1) {
            rtc.getTriggerCause();        // Get RTC input status
        }
    }
}

// Prototype"!!!!
void speedControl(int);

void brakeControl(int brakeRate) {
    if (driveMode == 2) {   // PARK MODE
        //  BREAK RATE LEVEL 1
        speedControl(0);
        brakeValve32 = 0;
        brakeValve22 = 1;
        //if (pressureSwitch1 == 0) {
//            brakeValve22 = 0;
//        }
//        else {
//            brakeValve22 = 1;   
//        }
    }
    else {
        
        if (challenge.regenBrakingActive == true) { // REGEN BRAKING WITH OVERVOLTAGE SAFETY CHECK
            if (brakeRate > 0) {
               motor1.setPark();
            }
            else {
                motor1.setForward();
            }
           // switch (brakeRate) {
//                case 0:     // NO BRAKING
//                    motor1.brake(0.00f);
////                    motor2.brake(0.00f);
////                    contactBatt = 0;                    // Close battery contactor so all power comes from supercaps
//                    pc.printf("Regen Braking set to 0%\r\n");
//                    break;
//                    
//                case 1:
//                    motor1.throttle(0.0f);
//                    motor1.brake(0.33f);
////                    motor2.brake(0.33f);
////                    contactBatt = 0;                    // Open battery contactor so all power comes from supercaps
//                    pc.printf("Regen Braking set to 33%\r\n");
//                    break;
//                
//                case 2:
//                    motor1.brake(0.66f);
//                    motor1.throttle(0.0f);
////                    motor2.brake(0.66f);
////                    contactBatt = 0;                    // Open battery contactor so all power comes from supercaps
//                    pc.printf("Regen Braking set to 66%\r\n");
//                    break;
//                
//                case 3:
//                    motor1.brake(1.0f);
//                    motor1.throttle(0.0f);
////                    motor2.brake(1.0f);
//                    pc.printf("Regen Braking set to 100%\r\n");
//                    break;
//                                    
//                default:
//                    break;
//            }
        }
        else {  // MECHANICAL BRAKING 
            switch (brakeRate) {
                case 0:     // NO BRAKING
                    brakeValve32 = 1;//(PF_2)
                    brakeValve22 = 1;//(PG_1)
                    break;
                    
                case 1:           //One brake high 
                    motor1.throttle(0.0f);
                    brakeValve32 = 0;//(PF_2)
                    brakeValve22 = 1;//(PG_1)
                    break;
                case 2 ... 4 :    //Two brake high
                    motor1.throttle(0.0f);
                    brakeValve32 = 0;//(PF_2)
                    brakeValve22 = 0;//(PG_1)
                    break;
                    

//                case 1:
//                    motor1.throttle(0.0f);
//                    brakeValve32 = 0;
//                    if (pressureSwitch1.read() == 0) {
//                        brakeValve22 = 0;
//                        pc.printf("Pressure 1 Reached");
//                    }
//                    else {
//                        brakeValve22 = 1;   
//                        pc.printf("Braking Level 1\r\n");
//                    }
//                    break;
                    
//                case 2:
//                    motor1.throttle(0.0f);
//                    brakeValve32 = 0;
//                    if (pressureSwitch2.read() == 0) {
//                        brakeValve22 = 0;
//                        pc.printf("Pressure 2 Reached");
//                    }
//                    else {
//                        brakeValve22 = 1;   
//                        pc.printf("Braking Level 2\r\n");
//                    }
//                    
//                    break;
//                    
//                case 2 ... 4:
//                    motor1.throttle(0.0f);
//                    brakeValve32 = 0;
//                    brakeValve22 = 1;
                    
//                    if (pressureSwitch3.read() == 0) {
//                        brakeValve22 = 0;
//                        pc.printf("Pressure 3 Reached");
//                    }
//                    else {
//                        brakeValve22 = 1;  
//                        pc.printf("Braking Level 3\r\n");
//                    }
//                    break;
                    
                    
                default:    // NO BRAKING
                    brakeValve32 = 1;
                    brakeValve22 = 1;
                    break;
            }
        }
    }   
    return;
}

void speedControl(int commandedSpeed) {
    if (dashboard.currentSpeed < 16.00) {  // IF SPEED LESS THAN LIMIT
        switch (commandedSpeed) {
            
            default:
//                motor1.throttle(0.0f);
                break;
                
            case 0:
                motor1.throttle(0.0f);
                break;
                
            case 1 ... 2:
                motor1.throttle(0.1f);
                break;
                
            case 3 ... 4:
                motor1.throttle(0.2f);
                break;
                
            case 5 ... 6:
                motor1.throttle(0.3f);
                break;
                
            case 7 ... 8:
                motor1.throttle(0.4f);
                break;
                
            case 9 ... 10:
                motor1.throttle(0.5f);
                break;
                
            case 11:
                motor1.throttle(0.6f);
                break;
                
            case 12:
                motor1.throttle(0.7f);
                break;
                
            case 13:
                motor1.throttle(0.8f);
                break;
                
            case 14:
                motor1.throttle(0.9f);
                break;
                
            case 15:
                motor1.throttle(1.0f);
                break;
        }
    }
    else {  // IF OVER 15KPH
        if (dashboard.currentSpeed < 20.00) {   // If speed less than 20 (we cant physically go this fast so any faster is probably compressor noise), cut throttle, otherwise ignore
            motor1.throttle(0.0f);    // COMMENTED AS ALREADY SET 0 IN BRAKECONTROL
//          brakeControl(1);
        }
    }
}

/*void speedControl(int commandedSpeed) {
    if (commandedSpeed > dashboard.currentSpeed) {  // IF THROTTLE REQUESTED
        // Max possible difference is 15
        // Motor Analog Voltage between 0 and 5
        // 5 / 15 = 0.33333 = 0.4v / kph difference
        
        int difference = commandedSpeed - dashboard.currentSpeed;
        
        switch (difference) {
            case 1:
                motor1.throttle(0.1f);
//              motor2.throttle(0.1f);
                pc.printf("Throttle set to 10%\r\n");
                break;
                
            case 2 ... 3:
                motor1.throttle(0.2f);
//                motor2.throttle(0.2f);
                pc.printf("Throttle set to 20%\r\n");
                break;
            
            case 4 ... 6:
                motor1.throttle(0.4f);
//                motor2.throttle(0.4f);
                pc.printf("Throttle set to 40%\r\n");
                break;
            
            case 7 ... 9:
                motor1.throttle(0.6f);
//                motor2.throttle(0.6f);
                pc.printf("Throttle set to 60%\r\n");
                break;
                
            case 10 ... 12:
                motor1.throttle(0.8f);
//                motor2.throttle(0.8f);
                pc.printf("Throttle set to 80%\r\n");
                break;
                
            case 13 ... 15:
                motor1.throttle(1.0f);
//                motor2.throttle(1.0f);
                pc.printf("Throttle set to 100%\r\n");
                break;
                
            default:
                motor1.throttle(0.0f);
                break;
        }
    }
    else {  // COAST
        motor1.throttle(0.0f);
//        motor2.throttle(0.0f);
    }
}*/

int main() {
    pc.baud(115200);
    
    // CONFIGURE INTERRUPTS
    rtc_output.rise(&emergencyStop);

    millisStart();
    
    rtc_Trigger = 1;

    // LOCAL VARIABLES
    bool systemOn = false;   // On/Off status of loco
    int lastKnownDirection = 2;
    bool inParkMode = false;
    
    // Record last time error was sent
    unsigned long lastErrorMillis = 0;

    while(1) {
     
        while(remote.commsGood == true) {
            
            // PING
            remote.commsCheck();
            // GET SWITCH STATES
            remote.getSwitchStates();
            
            // ALLOW BRAKES TO BE OPERATED
            brakeControl(remote.braking);
            
            // Print Pressure Switch States (Debugging)
//            pc.printf("Pressure Switch 1: %d\r\n", pressureSwitch1.read());
//            pc.printf("Pressure Switch 2: %d\r\n", pressureSwitch2.read());
//            pc.printf("Pressure Switch 3: %d\r\n", pressureSwitch3.read());
            
            // SOUND WHISTLE IF WHISTLE BUTTON PRESSED 
            if (remote.whistle == 0) {
                 whistleValve32 = 1;
//                 wait(0.5);
//                 whistleValve32 = 1;
             }
             else {
                 whistleValve32 = 0;
             }   


            //  GET AND DISPLAY SPEED
            dashboard.getCurrentSpeed();
            remote.sendData(2, dashboard.currentSpeed);       // Send speed to remote
                
            // TOGGLE COMPRESSOR
            remote.compressor == 0 ? contactCompressor = 1 : contactCompressor = 0;
            
            // TOGGLE MOTOR CONTROLLER DEADMAN (SEAT SWITCH AND FOOTBRAKE)
            if (rtc.deadman == 0) { // IF DEADMAN PRESSED
                motor1.closeDeadman();
            }
            else {
                motor1.releaseDeadman();
            }
            
            // TOGGLE REGEN THROTTLING
            if (challenge.regenThrottleActive == false) {
                if (remote.regenThrottle == 0 && remote.start == 0) {    // TURN OFF IF ON
                    challenge.regenThrottleOn();
                }
            }
            else {
                remote.sendError('B');     // Send error to remote
                if (remote.regenThrottle == 1) {    // TURN ON IF OFF
                    challenge.regenThrottleOff();
                }
            }
            
            //  TOGGLE REGEN BRAKING
            if (challenge.regenBrakingActive == false) {
                if (remote.regenBrake == 0 && remote.start == 0) {    // TURN OFF IF ON
                    if (challenge.regenBrakingOn() == 0) {
                        remote.sendError('I');     // Send error to remote
                        pc.printf("Regen Braking Off - SuperCaps are full\r\n");
                    }
                }
            }
            else {
                remote.sendError('C');     // Send error to remote
                if (remote.regenBrake == 1) {     // TURN OFF
                    challenge.regenBrakingOff();
                    if (superCapVoltage == 1) {
                        pc.printf("Regen Braking Off - SuperCaps are full\r\n");
                        remote.sendError('I');     // Send error to remote
                    }
                }
            }

            //  TOGGLE AUTOSTOP
            if (challenge.autoStopActive == 0) {
                if (remote.autoStop == 0 && remote.start == 0) {    // TURN OFF IF ON
                    challenge.autoStopOn();
                }
            }
            else {
                remote.sendError('D');     // Send error to remote
                if (remote.autoStop == 1) {    // TURN ON IF OFF
                    challenge.autoStopOff();
                }
            }

            //  TOGGLE INNOVATION
            if (challenge.innovationActive == 0) {
                if (remote.innovation == 0 && remote.start == 0) {    // TURN OFF IF ON
                    if (driveMode == 0) {
                        challenge.innovationOn();
                    }
                    else {
                        remote.sendError('J');     // Send error to remote
                        pc.printf("Can only active innovation mode in forward direction\r\n");
                    }
                }
            }
            else {
                remote.sendError('E');     // Send error to remote
                
                if (remote.innovation == 1) {    // TURN ON IF OFF
                    challenge.innovationOff();
                }
            }
            
            
            // CONTROL
            
           if (systemOn == false) {
            
                if (remote.start == 1) {
                    
                    if (millis() - lastErrorMillis > 500) {
                        remote.sendError('A');   // SEND ERROR MESSAGE 'A' TO REMOTE
                        lastErrorMillis = millis();
                    }
                
                    motor1.turnOff();
//                    motor2.turnOff();
                }
                else {
                    systemOn = true;
                    pc.printf("Start Switch is On\r\n");
//                    startupHealthCheck();   // Run System Startup Health Check - Will stay in here until it passes
                    
                    motor1.turnOn();    // Turn on motors
//                    motor2.turnOn();
                    
                }
            }   // END IF SYSTEMON = FALSE
            else {  // IF SYSTEMON == TRUE
                if (remote.start == 1) {
                    systemOn = false;       // WILL STOP ABOVE HERE NEXT LOOP
                    pc.printf("Start Switch is Off\r\n");
                }
                
                if (driveMode != 0 && remote.forward == 0) {
                    driveMode = 0;
                    motor1.setForward();
//                    motor2.setForward();
                }
                if (driveMode != 1 && remote.reverse == 0) {
                    driveMode = 1;
                    motor1.setReverse();
//                    motor2.setReverse();
                }
                if (driveMode != 2 && remote.park == 0) {
                    driveMode = 2;
                    motor1.setPark();
                    motor1.throttle(0);
//                    motor2.setPark();
                }

                if (driveMode == 2) {                             //place in park mode if selected by driver
                
//                    pc.printf("RTC Output is %d\r\n", rtc_output.read());
//                    pc.printf("EM State Output is %d\r\n", emergencyStopActive);
//                    pc.printf("ParkMode = %d", inParkMode);
                    
                    if (inParkMode == false) {
                        pc.printf("Train in park mode.\r\n");
                    }
                    
                    if (emergencyStopActive == true && rtc_output.read() == 0) {   // Clear emergency stop flag
                        emergencyStopActive = false;
                    }
                    
                    led_parkMode = 1;
                    inParkMode = true;      // Stop above debug print from displaying more than once
                    
                    motor1.setPark();
//                    motor2.setPark();
                    
                }
                else{                                                   //else i.e if selected drive mode is forward or reverse
//                    pc.printf("RTC Output is %d\r\n", rtc_output.read());
//                    pc.printf("EM State Output is %d\r\n", emergencyStopActive);
                    if (emergencyStopActive == false && rtc_output.read() == 0) {                                  // IF RTC FLAGGED AS SAFE
                    
//                        if (dashboard.currentSpeed < 1 || driveMode == lastKnownDirection) {      //do not allow motors to reverse if significant forward speed exists
                            
                            led_parkMode = 0;
                            inParkMode = false;
                            
                            if (driveMode != lastKnownDirection) {
                                pc.printf("Train enabled for direction %d\r\n", driveMode);
                                
                                lastKnownDirection = driveMode;
                            }
                           

                            if (challenge.autoStopInProgress == true) { // IF AUTOSTOPPING, PASS THROTTLE CONTROL TO FUNCTION
                                challenge.autoStopControl();
                                pc.printf("Autostop in Control\r\n");
                            }
                            else {  // OTHERWISE INPUT THROTTLE FROM REMOTE
                                if (remote.throttle > 0) {   // If joystick pushed upwards = throttle
                                    
                                    if (challenge.innovationActive == true) {
                                        pc.printf("Collision Detection in Control\r\n");
                                        int innovationThrottle = challenge.innovationControl(remote.throttle);
                                        speedControl(innovationThrottle);
                                        
                                        if (innovationThrottle == 0) {
                                             emergencyStop();   // emergency Brake if obstacle detected
                                        }
                                    }
                                    else {
                                        speedControl(remote.throttle);
                                        pc.printf("Throttling: %d\r\n", remote.throttle);
                                    }
                                } // remote.throttle
                                else {
                                    speedControl(0);  
                                }
                            }

//                        }
//                        else {
//                            pc.printf("Cannot change direction until train has stopped moving\r\n");
//                            remote.sendError('F');     // Send error to remote
//                        }
                    }
                    else {
                        pc.printf("Cannot exit park mode until RTC is cleared\r\n");
                        inParkMode = false;
                        remote.sendError('G');     // Send error to remote
                    }
                }
            }   // END IF (SYSTEMON == TRUE)

            wait_ms(500);   // SLOW DOWN THE SYSTEM (REMOTE CANT KEEP UP)
        }   // END WHILE(COMMSGOOD)
        pc.printf("Main Loop Skipped Due To Emergency Status\r\n");
        emergencyStop();    // Emergency stop if comms lost with remote controller 

    }   //END WHILE(1)
}
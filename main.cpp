#include <mbed.h>
#include "millis.h"
#include "brakes.h"
#include "definitions.h"
#include "remoteControl.h"
#include "dashboard.h"
#include "rtc.h"
#include "motor.h"
#include "challenge.h"
#include <sstream>
using std::string;

//  SET UP REMOTE CONTROL COMMS
SPI remoteControl(PE_14, PE_13, PE_12);   // (SPI_MOSI, SPI_MISO, SPI_SCK)
DigitalOut remoteControlCS(PE_11);    // (SPI_SS)

//  CREATE OBJECTS
Remote remote(remoteControl, remoteControlCS);
Dashboard dashboard(hallSensor);
RoundTrainCircuit rtc(rtc_1, rtc_2, rtc_3, rtc_4, rtc_5, rtc_6, rtc_7, rtc_override);
Motor motor1(motorAccelerator, motorBrake, keySwitchM1, directionFwd, directionRev, footswitchM1, seatM1, inchFwdM1, speedLimit2M1, speedLimit3M1);
ChallengeMode challenge(autoStopTrigger, dashboard, remote, motor1);
Brakes brakes;

int driveMode = 2;      // Drive mode - fwd(0), rev(1), park(2)
bool emergencyStopActive = false;

// FUNCTIONS

//Display Function for data logger
void DisplaySerial(){ 
    std::stringstream displayline;
    displayline << "Blackbox# " << " Motor Accelerator: " << motorAccelerator << " Brake 3/2: " << brakeValve32 <<  " Brake 2/2: " << brakeValve22 <<  " Speed: " << dashboard.currentSpeed << " Distance: " << dashboard.currentDistance << " Drive Mode: " << driveMode << "\n";
    string disp = displayline.str();
    pc.printf("%s \n", disp.c_str());
    }

//Brake code 
void brakeControl(int brakeRate) { //set brake rate to float for regen
  if (driveMode == 2) {   // PARK MODE
    //  All Mechanical brakes applied
      brakes.ParkMode(motor1);
  }
  {//REGEN BRAKING
  
  if (challenge.regenBrakingActive == true) { // REGEN BRAKING WITH OVERVOLTAGE SAFETY CHECK
       brakes.RegenControl(brakeRate,motor1);
      }
    
    else {  // MECHANICAL BRAKING
     brakes.MechanicalBraking(brakeRate,motor1); // calls mechanical braking 
    }
  }
  return;
}
void emergencyStop()
{
    brakes.EmergencyStop(motor1,rtc,emergencyStopActive); //invokes emergency stop code from brakes class 
}
//Motor code
void speedControl(int commandedSpeed) {
  switch (commandedSpeed) {

    default:
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

  //MainLoop
  while (1) {

    while (remote.commsGood == true) {
      /////Start Up///////////////////////////////////////////////////////////////////////////////////////////////////////////

      /////Checking Modes from controller/////////////////////////////////////////////////////////////////////////////////
      // PING
      remote.commsCheck();

      // GET SWITCH STATES
      remote.getSwitchStates();

      // ALLOW BRAKES TO BE OPERATED
      brakeControl(remote.braking);

      // SOUND WHISTLE IF WHISTLE BUTTON PRESSED
      if (remote.whistle == 0) {
        whistleValve32 = 1;
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
      /////END OF TOGGLE CHECKS//////////////////////////////////////////////////////////////////////////////////////////////

      /////Control///////////////////////////////////////////////////////////////////////////////////////////////////////////
      //Is a Toggle check, but it is the train start swtich A
      if (systemOn == false) {
        if (remote.start == 1) {
          if (millis() - lastErrorMillis > 500) {
            remote.sendError('A');   // SEND ERROR MESSAGE 'A' TO REMOTE
            lastErrorMillis = millis();
          }
          motor1.turnOff();
        }
        else {
          systemOn = true;
          motor1.turnOn();    // Turn on motors
        }
      }   // END IF SYSTEMON = FALSE
      //If train is switched on and in start do this start///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      else {  // IF SYSTEMON == TRUE
        if (remote.start == 1) {
          systemOn = false;       // WILL STOP ABOVE HERE NEXT LOOP
          pc.printf("Start Switch is Off\r\n");
        }

        //Set foward
        if (driveMode != 0 && remote.forward == 0) {
          driveMode = 0;
          motor1.setForward();
        }
        //Set reverse
        if (driveMode != 1 && remote.reverse == 0) {
          driveMode = 1;
          motor1.setReverse();
        }
        //Set park
        if (driveMode != 2 && remote.park == 0) {
          driveMode = 2;
          motor1.setPark();
          motor1.throttle(0);
        }
        ////Park Mode
        if (driveMode == 2) {     
             brakes.ParkMode(motor1);                        //place in park mode if selected by driver
          if (inParkMode == false) {
            pc.printf("Train in park mode.\r\n"); //why?
          }

          if (emergencyStopActive == true && rtc_output.read() == 0) {   // Clear emergency stop flag
            emergencyStopActive = false;
          }

          led_parkMode = 1;
          inParkMode = true;      // Stop above debug print from displaying more than once
          motor1.setPark();
        }

        ////Drive
        else {                                                  //else i.e if selected drive mode is forward or reverse
          ////////////////// Start of check for error G RTC clear
          if (emergencyStopActive == false && rtc_output.read() == 0) {                                  // IF RTC FLAGGED AS SAFE

            led_parkMode = 0;
            inParkMode = false;

            if (driveMode != lastKnownDirection) {
              pc.printf("Train enabled for direction %d\r\n", driveMode);

              lastKnownDirection = driveMode;
            }

            ////Call autostop challenge
            if (challenge.autoStopInProgress == true) { // IF AUTOSTOPPING, PASS THROTTLE CONTROL TO FUNCTION
              challenge.autoStopControl();
              pc.printf("Autostop in Control\r\n");
            }
            //Use controls from remote
            else {  // OTHERWISE INPUT THROTTLE FROM REMOTE
              if (remote.throttle > 0) {   // If joystick pushed upwards = throttle
                /////////////////////////Innovation braking start
                if (challenge.innovationActive == true) {
                  pc.printf("Collision Detection in Control\r\n");
                  int innovationThrottle = challenge.innovationControl(remote.throttle);
                  speedControl(innovationThrottle);

                  if (innovationThrottle == 0) {
                      emergencyStop(); // emergency Brake if obstacle detected
                  }
                }
                /////////////////////////Innovation braking end
                //normal throttle control
                else {
                  speedControl(remote.throttle);
                  pc.printf("Throttling: %d\r\n", remote.throttle);
                }
              } // remote.throttle
              ///if nothing set to 0
              else {
                speedControl(0);
              }
            }

          }
          ////////////////// End of check for error G RTC clear
          else {
            pc.printf("Cannot exit park mode until RTC is cleared\r\n");
            inParkMode = false;
            remote.sendError('G');     // Send error to remote
          }
        }
        //Datalogger Chai Funciton
        DisplaySerial();
      }   // END IF (SYSTEMON == TRUE)
      //If train is switched on and in start do this end///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      wait_ms(25);   // SLOW DOWN THE SYSTEM (REMOTE CANT KEEP UP)
    }   // END WHILE(COMMSGOOD)
    pc.printf("Main Loop Skipped Due To Emergency Status\r\n");
    emergencyStop();    // Emergency stop if comms lost with remote controller


  }   //END WHILE(1)
}

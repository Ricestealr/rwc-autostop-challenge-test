#include "brakes.h"
#include <mbed.h>
#include "definitions.h"
Brakes::Brakes()
{

}
void Brakes::FrontBrakeOn() //turns on front mechanical brake
{
    frontBrake=0;
    backBrake=1;
}

void Brakes::RearBrakeOn() //turns on rear mechanical brake
{
    frontBrake=1;
    backBrake=0;
}
void Brakes::BrakesOn() //turns on both mechanical brakes
{
    frontBrake=0;
    backBrake=0;
}

void Brakes::ParkMode(Motor motor) //Disengage motor and apply friction brakes
{
     motor.setPark();
     BrakesOn();
}
void Brakes::MechanicalBraking(int brakeRate, Motor motor) //friction braking cases
{
    switch (brakeRate) 
    {
        case 0:     // NO BRAKING
          frontBrake = 1;//(PD_3)
          backBrake = 1;//(PC_0)
          break;

        case 1:           //HALF BRAKING
          motor.throttle(0.0f);
          RearBrakeOn();
          break;

        case 2 ... 4 :    //FULL BRAKING
          motor.throttle(0.0f);
          BrakesOn();
          break;

        default:    // NO BRAKING
          frontBrake = 1;//(PC_0)
          backBrake = 1;//(PD_3)
          break;
    }
}

void Brakes::RegenControl(int ratecontrol,Motor motor) //graduated braking control
{
    switch (ratecontrol)
    {
          default:
            break;
            
            case 0:
            motor.brake(0.0f);
            break;

            case 1:
            motor.brake(0.25f);
            break;
            
            case 2:
            motor.brake(0.5f);
            break;

            case 3:
            motor.brake(0.75f);
            break;
            
            case 4:
            motor.brake(1.0f);
            break;
      }
    }
void Brakes::EmergencyStop(Motor motor,RoundTrainCircuit rtc, bool emergencyStopActive)
{
    if (emergencyStopActive == false) 
    {

        emergencyStopActive = true;
    
        //Set brake throttle to zero before disconnected, think is why we had the runaway train imo
        motor.throttle(0.0f);

        //Disengage motor
        motor.disengage();

        //Setting brakes to high
        frontBrake=0;
        backBrake=0;
    
        if (rtc_output.read() == 1) 
        {  //Check RTC pin out
            rtc.getTriggerCause();        // Get RTC input status
        }
  }
}
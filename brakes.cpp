#include "brakes.h"
#include <mbed.h>
#include "definitions.h"
void Brakes::FrontBrakeOn()
{
    brakeValve22=0;
    brakeValve32=1;
}

void Brakes::RearBrakeOn()
{
    brakeValve22=1;
    brakeValve32=0;
}
void Brakes::BrakesOn()
{
    brakeValve22=0;
    brakeValve32=0;
}

void Brakes::ParkMode()
{
     BrakesOn();
}
void Brakes::MechanicalBraking(int brakeRate, Motor motor)
{
switch (brakeRate) {
        case 0:     // NO BRAKING
          brakeValve32 = 1;//(PD_3)
          brakeValve22 = 1;//(PC_0)
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
          brakeValve32 = 1;//(PD_3)
          brakeValve22 = 1;//(PC_0)
          break;
          }
}

void Brakes::RegenControl(int ratecontrol,Motor motor)
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

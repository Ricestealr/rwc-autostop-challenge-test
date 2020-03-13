#include <mbed.h>
#include "definitions.h"
//#include "remoteControl.h"    // Enable if you want error sent to remote
#include "rtc.h"
#include "motor.h"
//
RoundTrainCircuit::RoundTrainCircuit(
                            DigitalIn rtc_1, 
                            DigitalIn rtc_2,
                            DigitalIn rtc_3,
                            DigitalIn rtc_4,
                            DigitalIn rtc_5,
                            DigitalIn rtc_6,
                            DigitalIn rtc_7,
                            DigitalIn rtc_override) :
                            
                            deadman(rtc_1),
                            emergencyButtonLeft(rtc_2),
                            emergencyButtonRight(rtc_3),
                            heatDetector(rtc_4),
                            driverCord(rtc_5),
                            superCapOverVoltage(rtc_6),
                            spare(rtc_7),
                            override(rtc_override)
                            {

}

void RoundTrainCircuit::getTriggerCause() {

    if (deadman == 1) {
        pc.printf("Deadman Switch Triggered\r\n");
//        DigitalOut footswitchM1(PD_4);
//        DigitalOut seatM1(PD_5);
    }
    
    if (emergencyButtonLeft == 1) {
        pc.printf("Left Emergency Button Operated\r\n");
    }
    
    if (emergencyButtonRight == 1) {
        pc.printf("Right Emergency Button Operated\r\n");
    }
    
    
    if (heatDetector == 1) {
        pc.printf("Heat Detector Triggered\r\n");
    }
    
    if (driverCord == 1) {
        pc.printf("Driver Cord Released\r\n");
    }
    
    if (superCapOverVoltage == 1) {
        pc.printf("Super Cap Reached Dangerous Levels\r\n");
    }
    
    if (spare == 1) {
        pc.printf("Spare Input Triggered\r\n");
    }
}
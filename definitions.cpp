#include <mbed.h>
#include "definitions.h"

// COMMS DEFINITIONS
Serial pc(USBTX, USBRX);

// INTERRUPTS
InterruptIn rtc_output(PF_11, PullUp);
InterruptIn autoStopTrigger(PD_2, PullDown);        //interrupt on lineside equipment detection
InterruptIn hallSensor(PG_3);              // For Hall Sensor Tachometer

// PIN DEFINITIONS

// INNOVATION/ COLLISION DETECTION
AnalogIn irSensor_1(PF_6);
AnalogIn irSensor_2(PF_7);
AnalogIn irSensor_3(PC_2);

//RTC individual inputs//
DigitalIn rtc_1(PG_4, PullUp);
DigitalIn rtc_2(PG_6, PullUp);
DigitalIn rtc_3(PG_7, PullUp);
DigitalIn rtc_4(PG_5, PullUp);
DigitalIn rtc_5(PD_10, PullUp);
DigitalIn rtc_6(PG_8, PullUp);
DigitalIn rtc_7(PE_0, PullUp);
DigitalIn rtc_override(PG_14, PullUp);

// RTC Outputs
DigitalOut rtc_Trigger(PD_14);


// Output LEDs
//DigitalOut led_rtcOutput();    // No need as RTC has led output
//DigitalOut led_emergencyBrake(PE_11);
DigitalOut led_parkMode(PF_10);


//Motor outputs               // ENSURE YOU USE DAC PINS - BOTH MOTORS SHARE PINS
AnalogOut motorAccelerator(PA_5);
AnalogOut motorBrake(PA_4);

////motor 1 settings
DigitalOut keySwitchM1(PF_1);
DigitalOut directionFwd(PF_0);
DigitalOut directionRev(PD_1);
//DigitalOut directionM1(PA_3); // FORWARD AND REVERSE NEED TO BE SEPARATE
DigitalOut footswitchM1(PD_4);
DigitalOut seatM1(PD_5);
DigitalOut inchFwdM1(PD_6);
DigitalOut speedLimit2M1(PD_7);
DigitalOut speedLimit3M1(PE_3);

////motor 2 settings;
//DigitalOut keySwitchM2(PD_10);
//DigitalOut directionM2(PG_6);
//DigitalOut footswitchM2(PG_4);
//DigitalOut seatM2(PC_8);
//DigitalOut inchFwdM2(PC_6);
//DigitalOut speedLimit2M2(PA_12);
//DigitalOut speedLimit3M2(PA_11);

DigitalIn superCapPreCharge(PB_11);    // Supercaps are pre-charged
DigitalIn superCapVoltage(PB_2);  // Supercaps are Full

//Contactors as digital outputs
DigitalOut contactBatt(PE_8);        // C-BAT
DigitalOut contactCompressor(PG_10);   // C-COM
DigitalOut contactCapCharge(PG_11);   // C-CHA

//DigitalOut contactMtr1(PE_0); // DRIVEN BY MOTOR CONTROLLER
//DigitalOut contactMtr2(PG_8);

//mechanical braking and other air components.
DigitalOut brakeValve32(PD_3); //brake pins updated wire 90 back brake
DigitalOut brakeValve22(PC_0); // brake pins updated  wire 91 front brake
DigitalOut whistleValve32(PG_15);

DigitalIn pressureSwitch1(PC_13, PullUp);
DigitalIn pressureSwitch2(PC_1, PullUp);
DigitalIn pressureSwitch3(PC_15, PullUp);
//DigitalIn brakePressure(PH_0);      // NOT INSTALLED
DigitalIn mainlinePressure(PH_1, PullUp);

/*//Energy Storage;

AnalogIn vref_powercab(PA_0);
AnalogIn vref_supercap(PF_5);
AnalogIn vout_powercab(PB_1);
AnalogIn vout_supercap(PF_3);
*/

///Lights
DigitalOut FrontLight(PB_14);
DigitalOut BackLight(PB_15);
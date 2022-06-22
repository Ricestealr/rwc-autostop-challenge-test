#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <mbed.h>

extern Serial pc;

//// INTERRUPTS
extern InterruptIn rtc_output;
extern InterruptIn autoStopTrigger;                     //interrupt on lineside equipment detection
extern InterruptIn hallSensor;              // For Hall Sensor Tachometer
//


// PIN DEFINITIONS

// INNOVATION/ COLLISION DETECTION
extern AnalogIn irSensor_1;
extern AnalogIn irSensor_2;
extern AnalogIn irSensor_3;

//RTC individual inputs//
extern DigitalIn rtc_1;
extern DigitalIn rtc_2;
extern DigitalIn rtc_3;
extern DigitalIn rtc_4;
extern DigitalIn rtc_5;
extern DigitalIn rtc_6;
extern DigitalIn rtc_7;
extern DigitalIn rtc_override;

// RTC Outputs
extern DigitalOut rtc_Trigger;


// Output LEDs
//DigitalOut led_rtcOutput;    // No need as RTC has led output
//extern DigitalOut led_emergencyBrake;
extern DigitalOut led_parkMode;


//Motor outputs               // ENSURE YOU USE DAC PINS - BOTH MOTORS SHARE PINS
extern AnalogOut motorAccelerator;
extern AnalogOut motorBrake;

//motor 1 settings
extern DigitalOut keySwitchM1;
extern DigitalOut directionFwd;
extern DigitalOut directionRev;
extern DigitalOut footswitchM1;
extern DigitalOut seatM1;
extern DigitalOut inchFwdM1;
extern DigitalOut speedLimit2M1;
extern DigitalOut speedLimit3M1;

//motor 2 settings;
//extern DigitalOut keySwitchM2;
//extern DigitalOut directionM2;
//extern DigitalOut footswitchM2;
//extern DigitalOut seatM2;
//extern DigitalOut inchFwdM2;
//extern DigitalOut speedLimit2M2;
//extern DigitalOut speedLimit3M2;

// Power
extern DigitalIn superCapVoltage;
//extern DigitalIn batteryVoltage;

//Contactors as digital outputs
extern DigitalOut contactBatt;
extern DigitalOut contactCompressor;
extern DigitalOut contactCapCharge;

//mechanical braking and other air components.
extern DigitalOut brakeValve32;
extern DigitalOut brakeValve22;
extern DigitalOut whistleValve32;

extern DigitalIn pressureSwitch1;
extern DigitalIn pressureSwitch2;
extern DigitalIn pressureSwitch3;
//extern DigitalIn brakePressure;
extern DigitalIn mainlinePressure;
//


//Energy Storage Transducer Inputs
/*
extern AnalogIn vref_powercab;
extern AnalogIn vref_supercap;
extern AnalogIn vout_powercab;
extern AnalogIn vout_supercap;
*/
//Lights

extern DigitalOut FrontLight;
extern DigitalOut BackLight;
#endif
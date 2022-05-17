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

void Dashboard::tachoInterrupt() {
    
    currentTime = tachoTimer.read_ms();
    passedTime = (currentTime - lastTime) / 1000;
    pc.printf("Passed Time: %.2f\r\n", passedTime);
    
//    if (passedTime > 0.03) {
    
//        tachoCounter++;     // Increment tacho counter = number of strips detected
        //pc.printf("count = %d\r\n", tachoCounter);
        
    //    getCurrentSpeed();
    //    //   PASSED TIME IN ms
    //    passedTime_ms = tachoTimer.read_ms() - lastTime;// - 21;  // 20ms propogation delay
    //    
    //    if (passedTime_ms > 100) { // IGNORE SHORT PULSES
    //        lastTime = tachoTimer.read_ms();
            currentDistance += (wheelCircumference / numberOfMagnets);
            pc.printf("Current Distance = %.2f\r\n", currentDistance);
            
    //        passedTime = passedTime_ms / 1000.00f;
    ////        pc.printf("Passed TimeMS Int: %d\r\n", passedTime_ms);
    ////        pc.printf("Passed Time Int: %.2f\r\n", passedTime);
    ////        pc.printf("Tacho Interrupt\r\n");
    //    }
        
//        pc.printf("count = %d\r\n", tachoCounter);
        
        wheelFreq = 1 / (passedTime * numberOfMagnets);
//        pc.printf("wheelFreq = %.2f\r\n", wheelFreq);
        
//        tachoCounter = 0;   // RESET
        lastTime = currentTime;
        
        
        if (passedTime > 0.00f) {    // Stops dividing by 0
    
            // 1 Hz FREQ = 1 RPS -> x60 for minutes
            float rpm = wheelFreq * 60.00f;       /// MAX RPM AT 15KPH = 340RPM
//            pc.printf("RPM = %.2f\r\n",rpm);
            
    //        if (rpm < 15.0f) {
            
                float kph = (wheelCircumference * rpm * (60.00f / 1000.00f));
                printf("Speed = %.2f\r\n", kph);
        
//                if (kph > 17.0f) {
//                    currentSpeed = 0;
//                }
//                else {
                currentSpeed = int(kph);
//                }
    //        }
            
            
//        }
//        else {
//            currentSpeed = 0;
//             
//        }
    }

}

void Dashboard::getCurrentSpeed() {
    
    // USE THESE
//    tachoCounter
//    currentTime
//    lastTime

    currentTime = tachoTimer.read_ms();
    if ((currentTime - lastTime) > 5000) {
        currentSpeed = 0;
    }
//    passedTime = (currentTime - lastTime) / 1000;
//    pc.printf("Passed Time: %.2f\r\n", passedTime);
    
    //pc.printf("count = %d\r\n", tachoCounter);
//    
//    wheelFreq = tachoCounter / (passedTime * numberOfMagnets);
//    pc.printf("wheelFreq = %.2f\r\n", wheelFreq);
//    
//    tachoCounter = 0;   // RESET
//    lastTime = currentTime;
//    
//    
//    if (passedTime > 0.00f) {    // Stops dividing by 0
        // PASSED TIME IN SECONDS = 1000 / TIME IN ms
        // FREQ = 1/T = 1 * 1000 / T
        
//        pc.printf("Passed Time Calc: %.2f\r\n", passedTime);
        
//        if ((tachoTimer.read_ms() / 1000.00f) - lastTime < 10.00f) {
//             wheelFreq = 1 / (numberOfMagnets * passedTime);
////             pc.printf("wheelFreq set\r\n");
//        }
//        else {
//            wheelFreq = 0.00f;  // Set frequency to zero if not sensed for more than 10s
//            passedTime = 0.00f;
////            pc.printf("wheelFreq Zeroed\r\n"); 
//        }
       
                
//        pc.printf("wheelFreq = %.4f\r\n", wheelFreq);
        
        // 1 Hz FREQ = 1 RPS -> x60 for minutes
//        float rpm = wheelFreq * 60.00f;       /// MAX RPM AT 15KPH = 340RPM
//        pc.printf("RPM = %.2f\r\n",rpm);
//        
//        // rpm x 60 = rph
//        // speed = distance / time
//        // kph = rph / (1km x circumference [m])
//        
//        // kph = 2πr × RPM × (60/1000)
//        float kph = wheelCircumference * rpm * (60.00f / 1000.00f);
////        float kph = (rpm * 60.00f) / (1000.00f * wheelCircumference);
////        pc.printf("KM/H = %.2f\r\n",kph);
//        if (kph > 15) {
//            currentSpeed = 0;
//        }
//        else {
//            currentSpeed = int(kph);
//        }
//    }
//    else {
//        currentSpeed = 0;
//         
//    }
}
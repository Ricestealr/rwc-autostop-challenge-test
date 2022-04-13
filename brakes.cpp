void brakeControl(int brakeRate) {
    if (driveMode == 2) {   // PARK MODE
        //  BREAK RATE LEVEL 1
        speedControl(0);
        brakeValve32 = 0;
        brakeValve22 = 1;

    }
    else {
        
        if (challenge.regenBrakingActive == true) { // REGEN BRAKING WITH OVERVOLTAGE SAFETY CHECK
            if (brakeRate > 0) {
               motor1.setPark();
            }
            else {
                motor1.setForward();
            }
           
//        }
        }
        else {  // MECHANICAL BRAKING 
            switch (brakeRate) {
                //case 0:     // NO BRAKING
                  //  brakeValve32 = 1;//(PF_2)
                    //brakeValve22 = 1;//(PG_1)
                    //break;
                    
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
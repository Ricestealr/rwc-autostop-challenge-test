#ifndef RTC_H
#define RTC_H

class RoundTrainCircuit {
    public:
        RoundTrainCircuit  (DigitalIn rtc_1, 
                            DigitalIn rtc_2,
                            DigitalIn rtc_3,
                            DigitalIn rtc_4,
                            DigitalIn rtc_5,
                            DigitalIn rtc_6,
                            DigitalIn rtc_7,
                            DigitalIn rtc_override);
        
        DigitalIn deadman;
        DigitalIn emergencyButtonLeft;
        DigitalIn emergencyButtonRight;
        DigitalIn heatDetector;
        DigitalIn driverCord;
        DigitalIn superCapOverVoltage;
        DigitalIn spare;
        DigitalIn override;
                            
        void getTriggerCause();
};

#endif

#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <mbed.h>

class Remote{
    public:
        // CONSTRUCTOR
        Remote(SPI& remoteControl, DigitalOut& remoteControlCS, SPI& lcdScreen, DigitalOut& lcdScreenCS);
        
        bool commsGood;                 // Successful Comms Between Nucleo and Remote Control
        
        volatile int throttle;
        volatile int braking;
        volatile bool start;
        volatile bool forward;
        volatile bool park;
        volatile bool reverse;
        volatile bool compressor;
        volatile bool autoStop;
        volatile bool regenBrake;
        volatile bool regenThrottle;
        volatile bool whistle;
        volatile bool innovation;
        
        void initialiseRemoteComms();
        int sendData(int precursor, int data);
        void sendError(int error);
        void commsCheck();
        void getSwitchStates();
        void setTime(int hr, int min, int sec, int day, int mon, int yr);
        
    private:
        SPI& _remoteControl;
        DigitalOut& _remoteControlCS;

        SPI& _lcdScreen;
        DigitalOut& _lcdScreenCS;

        int spiDelay;
        int commsFailures;                      // Number of consecutive remote comms failures
        int errorIndex;
        int errorBuffer[27];
        
        void ByteToBits(unsigned char character, bool *boolArray);
        
        Ticker commsCheckTicker;                    //ticker for recurring comms check
        Ticker remoteSwitchStateTicker;             //ticker for recurring remote switch state update
};

#endif
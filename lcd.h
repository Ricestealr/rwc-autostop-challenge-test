#ifndef LCD_H
#define LCD_H

#include <mbed.h>

class Lcd{
    public:
        // CONSTRUCTOR
        Lcd(SPI& lcd, DigitalOut& lcdCS);
        
        int sendData(int precursor, int data) ;
        void sendError(int error);
        void setTime(int hr, int min, int sec, int day, int mon, int yr);
        
    private:
        SPI& _lcd;
        DigitalOut& _lcdCS;

        int spiDelay;
        int commsFailures;                      // Number of consecutive remote comms failures
        int errorIndex;
        int errorBuffer[27];
        
        
        //Ticker commsCheckTicker;                    //ticker for recurring comms check
        //Ticker remoteSwitchStateTicker;             //ticker for recurring remote switch state update
};

#endif
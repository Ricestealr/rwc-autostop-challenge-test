#include <mbed.h>
#include "lcd.h"
#include "definitions.h"

Lcd::Lcd(SPI& lcd, DigitalOut& lcdCS) : _lcd(lcd), _lcdCS(lcdCS) 
{
    _lcd.format(8,0);    // FORMAT SPI AS 8-BIT DATA, SPI CLOCK MODE 0
    const long arduinoClock = 16000000;
    long spiFrequency = arduinoClock / 8;
    _lcd.frequency(spiFrequency); // SET SPI CLOCK FREQUENCY
    
    _lcdCS = 1;   // DISABLE SLAVE
    spiDelay = 600;    //DELAY BETWEEN SPI TRANSACTIONS (SO ARDUINO CAN KEEP UP WITH REQUESTS)
    
    commsFailures = 0;                      // Number of consecutive remote comms failures
    errorIndex = 0;
    
}

int Lcd::sendData(int precursor, int data) 
{
    int response  = 0;
    
    _lcdCS = 0;          // ENABLE REMOTE SPI
    //pc.printf("Enabled \r\n");

    _lcd.write(precursor);   // Prepare arduino to receive data
    wait_us(spiDelay);
    _lcd.write(data);          // SEND DATA
    wait_us(spiDelay);
    response = _lcd.write(255);
    _lcdCS = 1;         // DISABLE REMOTE SPI
    //pc.printf("Disabled \r\n");

    return response;
}


      
void Lcd::setTime(int hr, int min, int sec, int day, int mon, int yr) 
{
    _lcdCS = 0;
    
    _lcd.write(7);
    wait_us(spiDelay);
    _lcd.write(hr);
    wait_us(spiDelay);
    _lcd.write(min);
    wait_us(spiDelay);
    _lcd.write(sec);
    wait_us(spiDelay);
    _lcd.write(day);
    wait_us(spiDelay);
    _lcd.write(mon);
    wait_us(spiDelay);
    _lcd.write(yr);
    wait_us(spiDelay);
    _lcd.write(255);
    
    _lcdCS = 1;   // DISABLE REMOTE SPI
}

void Lcd::sendError(int error) 
{

    bool errorInBuffer = false;
    
    for (int index = 0; index < errorIndex; index++) 
    {
        if (errorBuffer[index] == error) 
        {
            errorInBuffer = true;
            break;
        }
    }
    
    if (errorInBuffer == false) 
    {
        errorBuffer[errorIndex++] = error;
    }
    else 
    {
        errorInBuffer = false;  // reset
    }
    
    sendData(8, errorBuffer[0]);
    

    for (int index = 0; index < errorIndex; index++) 
    {
        errorBuffer[index] = errorBuffer[index + 1];
    }
    errorIndex--;
    
    wait_ms(100);
}
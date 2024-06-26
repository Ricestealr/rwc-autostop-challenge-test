#include <mbed.h>
#include "remoteControl.h"
#include "definitions.h"

Remote::Remote(SPI& remoteControl, DigitalOut& remoteControlCS, SPI& lcdScreen, DigitalOut& lcdScreenCS) : _remoteControl(remoteControl), _remoteControlCS(remoteControlCS), _lcdScreen(lcdScreen), _lcdScreenCS(lcdScreenCS) {
    _remoteControl.format(8,0);    // FORMAT SPI AS 8-BIT DATA, SPI CLOCK MODE 0
    _lcdScreen.format(8,0);
    const long arduinoClock = 16000000;
    long spiFrequency = arduinoClock / 4;
    _remoteControl.frequency(spiFrequency); // SET SPI CLOCK FREQUENCY
    _lcdScreen.frequency(spiFrequency);
    
    _remoteControlCS = 1;   // DISABLE SLAVE
    _lcdScreenCS = 1; // Disable Slave for LCD
    spiDelay = 600;    //DELAY BETWEEN SPI TRANSACTIONS (SO ARDUINO CAN KEEP UP WITH REQUESTS)
    
    commsGood = false;                 // Successful Comms Between Nucleo and Remote Control
    commsFailures = 0;                      // Number of consecutive remote comms failures
    errorIndex = 0;
    
//    remoteSwitchStateTicker.attach(this, &Remote::getSwitchStates, 0.2);
    commsCheckTicker.attach(this, &Remote::commsCheck, 0.2);  // Run the commsCheck function every 0.1s with the commsCheckTicker. - &commsCheck = The address of the function to be attached and 0.1 = the interval
}

int Remote::sendData(int precursor, int data) 
{
    int response  = 0;
    
    _remoteControlCS = 0;          // ENABLE REMOTE SPI
//    pc.printf("Enabled \r\n");
    _remoteControl.write(precursor);   // Prepare arduino to receive data
    wait_us(spiDelay);
    _remoteControl.write(data);          // SEND DATA
    wait_us(spiDelay);
    response = _remoteControl.write(255);
    wait_us(spiDelay);
    
    _remoteControlCS = 1;         // DISABLE REMOTE SPI

    if(precursor == 10)
    {
        _lcdScreenCS = 0;
        _lcdScreen.write(data);
        
        wait_us(spiDelay);
        _lcdScreenCS = 1;
    }
//    pc.printf("Disabling\r\n");

    return response;
}

void Remote::commsCheck() 
{
    // Send a random number to the controller and expect its square in return for valid operation.
    
    // Random number between 2 and 15. Reply should be the squared, and within the 1 byte size limit of 255 for SPI comms.
    // Does not conflict with switch states as they use ASCII A, B C etc which are Decimal 65+
    int randomNumber = rand() % (15 - 2 + 1) + 2;  // rand()%(max-min + 1) + min inclusive of max and min
    int expectedResponse = randomNumber * randomNumber;
    int actualResponse = 0;
    
    actualResponse = sendData(1, randomNumber);

    
    if (actualResponse == expectedResponse) 
    {

        commsGood = true;
        commsFailures = 0;          // Reset consecutive failure count
    }
    else
    {

        
        if (commsFailures++ > 3) 
        {            // Increment consecutive failure count
            commsGood = false;              // Flag comms failure after 3 failures (> 0.3 seconds)

        }
    }

}

// CONVERT BYTE TO BITS
/* 
The received bytes from the remote control uses bitmapping.
Each 0/1 bit represents the on/ off state of a switch.
This function takes each bit and assigned it to a switch variable.
*/
void Remote::ByteToBits(unsigned char character, bool *boolArray)
{
    for (int i=0; i < 8; ++i) 
    {
        boolArray[i] = (character & (1<<i)) != 0;
    }   
}

void Remote::getSwitchStates() 
{
    // GET THE SWITCH STATES FROM THE REMOTE CONTROL

    
    bool bitGroupA[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    bool bitGroupB[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        
    char slaveReceivedA, slaveReceivedB;  // BYTE RECEIVED FROM REMOTE CONTROL
    
    slaveReceivedA = sendData(3, 3);
    slaveReceivedB = sendData(4, 4);
    throttle = sendData(5, 5);
    braking = sendData(6, 6);
            
    ByteToBits(slaveReceivedA, bitGroupA);    // CONVERT GROUP A BYTE TO BITS
    ByteToBits(slaveReceivedB, bitGroupB);    // CONVERT GROUP B BYTE TO BITS
    
    // ASSIGN VARIABLES FROM BIT GROUPS

    start         = bitGroupA[0];
    forward       = bitGroupA[1];
    park          = bitGroupA[2];
    reverse       = bitGroupA[3];
    compressor    = bitGroupA[4];
    autoStop      = bitGroupA[5];
    regenBrake    = bitGroupA[6];
    regenThrottle = bitGroupA[7];
    
    whistle       = bitGroupB[0];
    innovation    = bitGroupB[1];
   
        
}
        
void Remote::setTime(int hr, int min, int sec, int day, int mon, int yr) 
{
    _remoteControlCS = 0;
    
    _remoteControl.write(7);
    wait_us(spiDelay);
    _remoteControl.write(hr);
    wait_us(spiDelay);
    _remoteControl.write(min);
    wait_us(spiDelay);
    _remoteControl.write(sec);
    wait_us(spiDelay);
    _remoteControl.write(day);
    wait_us(spiDelay);
    _remoteControl.write(mon);
    wait_us(spiDelay);
    _remoteControl.write(yr);
    wait_us(spiDelay);
    _remoteControl.write(255);
    
    _remoteControlCS = 1;   // DISABLE REMOTE SPI
}

void Remote::sendError(int error) 
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

    for (int index = 0; index < errorIndex; index++) {
        errorBuffer[index] = errorBuffer[index + 1];
    }
    errorIndex--;
    
    wait_ms(100);
}
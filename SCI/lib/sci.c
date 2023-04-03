/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Created:       November 4, 2022
// Details:       SCI Library
// Credits:       Simon Walker for providing header file and help with implementation
// Revision History
//      each revision will have a date + desc. of changes
//      Nov. 6, 2022:   Implemented SCI0_Init, SCI0_BRead, SCI0_Read, and SCI0_TxByte functions
//      Nov. 14, 2022:  Implemented SCI0_TxStr function
/////////////////////////////////////////////////////////////////////////////

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "sci.h"

// other includes, as *required* for this implementation

/////////////////////////////////////////////////////////////////////////////
// local prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// library variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////
void SCI0_Init(unsigned long ulBusClock, SCI_BaudRate baudRate, SCI_RDRF_InterruptMode interruptMode)
{
    // calculate the value for SCI0BD based on the given busclock and baudrate (11.3.2.1)
    switch (baudRate)
    {
    case BaudRate_75:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 75) + 5) / 10);
        break;
    case BaudRate_110:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 110) + 5) / 10);
        break;
    case BaudRate_300:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 300) + 5) / 10);
        break;
    case BaudRate_600:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 600) + 5) / 10);
        break;
    case BaudRate_1200:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 1200) + 5) / 10);
        break;
    case BaudRate_2400:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 2400) + 5) / 10);
        break;
    case BaudRate_4800:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 4800) + 5) / 10);
        break;
    case BaudRate_9600:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 9600) + 5) / 10);
        break;
    case BaudRate_14400:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 14400) + 5) / 10);
        break;
    case BaudRate_19200:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 19200) + 5) / 10);
        break;
    case BaudRate_38400:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 38400) + 5) / 10);
        break;
    case BaudRate_57600:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 57600) + 5) / 10);
        break;
    case BaudRate_115200:
        SCI0BD = (unsigned int)(((ulBusClock * 10 / 16 / 115200) + 5) / 10);
        break;
    }

    // enable the reciever and transmitter (11.3.2.6)
    SCI0CR2_RE = 1; // reciever enable
    SCI0CR2_TE = 1; // transmitter enable

    // set the Receiver Full Interrupt Enable bit based on the requested mode (11.3.2.6)
    SCI0CR2_RIE = (unsigned char)interruptMode;
}

// read a byte, non-blocking
// returns 0 if byte read, -1 if not
// uses 0 for success because then we can have multiple error codes
// returning true means an error and we can then check the error code to find out what
int SCI0_Read(unsigned char *pData)
{
    // if there is no data, return -1 as an error
    if (!SCI0SR1_RDRF)
        return -1;

    // if this point is reached, there is data in the register
    *pData = SCI0DRL; // set the pointer to the data

    return 0; // return 0 to show that data was found
}

// blocking byte read
// waits for a byte to arrive and returns it
unsigned char SCI0_BRead(void)
{
    // wait for the flag to indicate that data has arrived
    while (!SCI0SR1_RDRF)
        ;

    return SCI0DRL; // return the byte
}

// send a byte over SCI
void SCI0_TxByte_Block(unsigned char data)
{
    // wait for the flag to indicate the register is empty
    while (!SCI0SR1_TDRE)
        ;

    SCI0DRL = data; // write the data in
}

// send a byte over SCI
// will return an error code if the char did not send
// returns 0 if the char was sent to SCI0DRL
int SCI0_TxByte_NonBlock(unsigned char data)
{
    // if TDRE is empty, write the data in
    if (SCI0SR1_TDRE)
    {
        SCI0DRL = data; // write the data in
        return 0;       // return 0 for success
    }

    return -1; // return 1 for data not loaded
}

// send a null-terminated string over SCI
void SCI0_TxStr(char const *strAddr)
{
    // send each character of the string one at a time until null terminator is reached
    while (*strAddr)
    {
        SCI0_TxByte_Block(*strAddr);
        ++strAddr;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

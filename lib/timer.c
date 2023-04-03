#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "timer.h"

/////////////////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////////////////
#define Timer_GetPrescaleBin (TSCR2 & 0b00000111)
#define Timer_GetPrescaleDec (1 << Timer_GetPrescaleBin)

// other includes, as *required* for this implementation

/////////////////////////////////////////////////////////////////////////////
// local prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// library variables
/////////////////////////////////////////////////////////////////////////////
ulong globalBusClock; // stores the global bus clock in MHz

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////

// Initializes the timer and sets up the given timer channel to operate as an output compare channel at the given prescale.
// The channel will have an initial offset
//  Parameters:
//       busClockHz is bus clock (in Hz)
//       prescale is prescale enum (range 0-7)
//       initialOffset_ms is the initial offset for timer (in ms) - function uses busClock and prescale to calculate the number of ticks needed - doesn't work with more than 1 cycle
//       mode enables interrupts according to what is passed in (polling or interrupt mode)
//       pinAction is the action desired based on the PinAction enum
void Timer_Init(unsigned long ulBusRate, Timer_Prescale prescale, unsigned int initialOffset_ms, Timer_Channel channel, Timer_Mode mode, Timer_PinAction pinAction)
{
    unsigned int offsetCount;

    // set the global variables for the bus clock and the prescale
    globalBusClock = ulBusRate;

    // ensure prescale is in range 0-7, if it is not, set it to 7 (prescale 128) by default
    if (prescale < 0 || prescale > 7)
        prescale = 7;
    // Set the prescale (7.3.2.11)
    TSCR2 &= ~0b00000111; // clear the old prescale
    TSCR2 |= prescale;    // set the new prescale

    // Set Input/Output compare channel to be an output compare (7.3.2.1)
    TIOS |= channel;

    // use the calculate count millseconds function to find the number of counts to
    // offset the output compare channel from TCNT by to prepare the timer for the initial offset
    offsetCount = Timer_CalculateCountMilli(initialOffset_ms);

    // set the pin action for the desired channel according to the appropriate parameter (7.3.2.8)
    // and set the initial offset from TCNT for the desired channel to the offsetCount (7.3.2.14)
    switch (channel)
    {
    case Timer_Channel_0:
        TCTL2 &= ~0b00000011;          // clear the pin action bits
        TCTL2 |= pinAction << (2 * 0); // set the pin action based on the parameter
        TC0 = TCNT + offsetCount;      // set the initial offset from TCNT on OC0 to the calculated count
        break;
    case Timer_Channel_1:
        TCTL2 &= ~0b00001100;          // clear the pin action bits
        TCTL2 |= pinAction << (2 * 1); // set the pin action based on the parameter
        TC1 = TCNT + offsetCount;      // set the initial offset from TCNT on OC1 to the calculated count
        break;
    case Timer_Channel_2:
        TCTL2 &= ~0b00110000;          // clear the pin action bits
        TCTL2 |= pinAction << (2 * 2); // set the pin action based on the parameter
        TC2 = TCNT + offsetCount;      // set the initial offset from TCNT on OC2 to the calculated count
        break;
    case Timer_Channel_3:
        TCTL2 &= ~0b11000000;          // clear the pin action bits
        TCTL2 |= pinAction << (2 * 3); // set the pin action based on the parameter
        TC3 = TCNT + offsetCount;      // set the initial offset from TCNT on OC3 to the calculated count
        break;
    case Timer_Channel_4:
        TCTL1 &= ~0b00000011;          // clear the pin action bits
        TCTL1 |= pinAction << (2 * 0); // set the pin action based on the parameter
        TC4 = TCNT + offsetCount;      // set the initial offset from TCNT on OC4 to the calculated count
        break;
    case Timer_Channel_5:
        TCTL1 &= ~0b00001100;          // clear the pin action bits
        TCTL1 |= pinAction << (2 * 1); // set the pin action based on the parameter
        TC5 = TCNT + offsetCount;      // set the initial offset from TCNT on OC5 to the calculated count
        break;
    case Timer_Channel_6:
        TCTL1 &= ~0b00110000;          // clear the pin action bits
        TCTL1 |= pinAction << (2 * 2); // set the pin action based on the parameter
        TC6 = TCNT + offsetCount;      // set the initial offset from TCNT on OC6 to the calculated count
        break;
    case Timer_Channel_7:
        TCTL1 &= ~0b11000000;          // clear the pin action bits
        TCTL1 |= pinAction << (2 * 3); // set the pin action based on the parameter
        TC7 = TCNT + offsetCount;      // set the initial offset from TCNT on OC7 to the calculated count
        break;
    }

    // Clear a pending interrupt (output compare flag) for channel zero (7.3.2.12)
    TFLG1 = (uchar)channel;

    //  Enable or disable interrupts on channel according to parameter (7.3.2.10).
    switch (mode)
    {
    case Timer_PollingMode:
        TIE &= ~channel; // clear the timer interrupt enable bit for the given channel
        break;
    case Timer_InterruptMode:
        TIE |= channel; // set the timer interrupt enable bit for the given channel
        break;
    }

    // Enable the timer (7.3.2.6)
    TSCR1_TEN = 1;
}

// uses the prescale and bus clock to calculate the number of counts TC0 must wait before the next flag is raise
// takes in a number of milliseconds and returns the counts required
uint Timer_CalculateCountMilli(unsigned int ms)
{
    // (busrate / prescale in decimal) * desired time in ms / 1000
    return (uint)(((globalBusClock) / Timer_GetPrescaleDec) * ms / (unsigned int)1E3);
}

// uses the prescale and bus clock to calculate the number of counts TC0 must wait before the next flag is raise
// takes in a number of microseconds and returns the counts required
unsigned int Timer_CalculateCountMicro(unsigned int us)
{
    // (busrate / prescale in decimal) * desired time in us / 1000000
    return (uint)(((globalBusClock) / Timer_GetPrescaleDec) * us / (unsigned long)1E6);
}

// blocking delay for x ms
// initializes timer channel 6 to be used for the sleep
// calculates values required to generate 1ms delay (may not be perfectly accurate) then loops ms times
// uses OC6, no pin action for timing
// if the timer was enabled by this function, it is disabled after the function is finished
void Timer_Sleep(unsigned int ms)
{
    uint numCounts1ms;      // will store the number of counts for 1ms
    ulong counter;          // will store the current count of 1ms that has gone by
    byte timerInitFlag = 0; // flag to store whether or not the timer was started by this sleep call
    // will be used to determine if the timer should be turned off after this call ends

    // could pass an offset of numCOunts1ms and move this to after that calculation
    //  Timer_Init(globalBusClock, Timer_GetPrescaleBin, 0, Timer_Channel_6, Timer_PollingMode, Timer_Pin_Disco);

    // if the timer is enabled, initialize channel 6 using the current busrate and prescale, in polling mode, with no pin action
    if (TSCR1_TEN)
        Timer_Init(globalBusClock, Timer_GetPrescaleBin, 0, Timer_Channel_6, Timer_PollingMode, Timer_Pin_Disco);
    // if the timer is not enabled, set the timerInitFlag so we know to disable the timer at the end of the function
    // then initialize TimerCh6, assuming a busrate of 20MHz at a prescale of 1, in polling mode, with no pin action
    else
    {
        timerInitFlag = 1;
        Timer_Init(20E6, Timer_Prescale_1, 0, Timer_Channel_6, Timer_PollingMode, Timer_Pin_Disco);
    }

    // calculate the number of ticks required for 1ms
    numCounts1ms = Timer_CalculateCountMilli(1);

    for (counter = 0; counter < ms; counter++)
    {
        // wait for the flag to be raised
        while (!TFLG1_C6F)
            ;

        TFLG1 = TFLG1_C6F_MASK; // clear the flag
        TC6 += numCounts1ms;    // rearm the flag for the next event
    }

    // if the timerInitFlag is set, the timer was enabled by this call, so we can turn it back off now
    if (timerInitFlag)
        TSCR1_TEN = 0;
}
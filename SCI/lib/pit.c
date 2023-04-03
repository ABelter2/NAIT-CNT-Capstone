#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "pit.h"

// other includes, as *required* for this implementation

/////////////////////////////////////////////////////////////////////////////
// local prototypes
/////////////////////////////////////////////////////////////////////////////

// finds the ideal or best fitting factors required to make the requested interval in us at a given bus rate
// return array: index 0 = PITMTLDx (8 bit), index 1 = PITLDx (16 bit)
void FindFactors(uint *intervalFactors, unsigned long interval_us, unsigned long ulBusRate_MHz);

/////////////////////////////////////////////////////////////////////////////
// library variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// constants             .
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////

// chan           : desired PIT channel (0-1 as enum)
// intState : turn on channel interrupt or not (0 == no interrupt)
// ulBusRate  : bus frequency, in Hz
// ulInterval_us  : desired interval in us (maximum is bus rate and 2^24 register limit dependent)
//      max interval is ~838.86ms at 20MHz bus rate
void PIT_Init(PIT_Channel chan, PIT_Interrupt intState, unsigned long ulBusRate, unsigned long ulInterval_us)
{
    // use the helper function to find the ideal/best fit factors for the desired interval
    uint intervalFactors[2] = {0};
    FindFactors(intervalFactors, ulInterval_us, ulBusRate); // index 0 = PITMTLDx(8 bit), index 1 = PITLDx(16 bit)

    switch (chan)
    {
    case PIT_Channel_0:
        // subtract 1 because 1 is added automatically by the board
        PITMTLD0 = intervalFactors[0] - 1;
        PITLD0 = intervalFactors[1] - 1;

        // clear a pending interrupt if there is one
        PITTF = PITTF_PTF0_MASK;

        // enable or disable interrupts based on the parameter
        PITINTE_PINTE0 = intState == PIT_Interrupt_On ? 1 : 0;

        // enable chan 0
        PITCE_PCE0 = 1; // 13.3.0.3
        break;
    case PIT_Channel_1:
        PITMUX_PMUX1 = 1; // set channel 1 to use micro time base 1 - 13.3.0.4

        // subtract 1 because 1 is added automatically by the board
        PITMTLD1 = intervalFactors[0] - 1;
        PITLD1 = intervalFactors[1] - 1;

        // clear a pending interrupt if there is one
        PITTF = PITTF_PTF1_MASK; // 13.3.0.6

        // enable or disable interrupts based on the parameter
        PITINTE_PINTE1 = intState == PIT_Interrupt_On ? 1 : 0; // 13.3.0.5

        // enable chan 1
        PITCE_PCE1 = 1; // 13.3.0.3
        break;
    }

    // finally, enable periodic interrupt, normal in wait, PIT stalled in freeze
    // PIT still runs in wait mode (relevant next course)
    // PIT stalls when debugging so PIT events don’t pile up while stepping
    PITCFLMT = PITCFLMT_PITE_MASK | PITCFLMT_PITFRZ_MASK; // 13.3.0.1
}

// blocking sleep for the specified number of ms, using the specified channel
//  requires init to reset enable of channel to reload counts
//  sleep can be longer than expected, as it has to eat the startup time
//   for init, so shorter delays will be longer than expected (.115ms in this lib)
void PIT_Sleep(PIT_Channel chan, unsigned long ulBusRate, unsigned int ms)
{
    ulong counter; // will store the current count of 1ms that has gone by

    // initialize the given channel with interrupts off, at the given bus rate for 1000us (1ms)
    PIT_Init(chan, PIT_Interrupt_Off, ulBusRate, 1000);

    switch (chan)
    {
    case PIT_Channel_0:
        for (counter = 0; counter < ms; ++counter)
        {
            // wait for the flag to be raised
            while (!PITTF_PTF0)
                ;
            // clear the flag
            PITTF = PITTF_PTF0_MASK;
        }
        break;
    case PIT_Channel_1:
        for (counter = 0; counter < ms; ++counter)
        {
            // wait for the flag to be raised
            while (!PITTF_PTF1)
                ;
            // clear the flag
            PITTF = PITTF_PTF1_MASK;
        }
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

// return array: index 0 = PITMTLDx (8 bit), index 1 = PITLDx (16 bit)
void FindFactors(uint *intervalFactors, unsigned long interval_us, unsigned long ulBusRate)
{
    // index 0 = PITMTLDx (8 bit), index 1 = PITLDx (16 bit)
    uint factor8Bit, factor16Bit;
    uint bestFit8Bit = 255, bestFit16Bit = 65535;

    // calculate the number of cycles required for the requested interval
    // divide by 1000000 to compensate for interval being in us
    ulong numCycles = (ulong)(interval_us * (ulBusRate / (ulong)1E6));

    // check if the required count is too long - if it is, set the factors to create the longest possible interval
    if (numCycles > (256 * 65536))
    {
        intervalFactors[0] = 255;
        intervalFactors[1] = 65535;
        return;
    }

    factor8Bit = 0; // set the 8 bit factor to 8

    // increment the 8 bit factor every loop while checking for factors
    // max 8 bit factor is 255
    while (++factor8Bit < 256)
    {
        // if the 16bit factor would be within range
        unsigned long tempFactor16Bit = numCycles / factor8Bit;
        if (tempFactor16Bit < 65536)
        {
            // find the 16 bit factor
            factor16Bit = (uint)tempFactor16Bit;

            // if ideal factors are found, return them
            if (factor8Bit * factor16Bit == numCycles)
            {
                intervalFactors[0] = factor8Bit;
                intervalFactors[1] = factor16Bit;
                return;
            }
            else
            {
                long absDiffBestFit, absDiffFactors;

                // find the absolute value of the difference between the number of
                // required cycles and the product of the best fit factors
                absDiffBestFit = numCycles - ((ulong)bestFit8Bit * bestFit16Bit);
                if (absDiffBestFit < 0)
                    absDiffBestFit *= -1;

                // find the absolute value of the difference between the number of
                // required cycles and the product of the new factors
                absDiffFactors = numCycles - ((ulong)factor8Bit * factor16Bit);
                if (absDiffFactors < 0)
                    absDiffFactors *= -1;

                // if the difference of the new factors is less than the old best fit factors
                // they are closer to the required count, so the best fit factors are updated to them
                if (absDiffFactors < absDiffBestFit)
                {
                    bestFit8Bit = factor8Bit;
                    bestFit16Bit = factor16Bit;
                }

                // ulong p1 = (ulong)factor8Bit * factor16Bit;
                // ulong p2 = (ulong)bestFit8Bit * bestFit16Bit;

                // ulong m1 = numCycles % p1;
                // ulong m2 = numCycles % p2;
                // if (m1 < m2)
                // {
                //     bestFit8Bit = factor8Bit;
                //     bestFit16Bit = factor16Bit;
                // }
            }
        }
    }

    // if this point is reached, the ideal factors were not found and the best fit factors will be used instead.
    intervalFactors[0] = bestFit8Bit;
    intervalFactors[1] = bestFit16Bit;
    return;
}

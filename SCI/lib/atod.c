/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Credits:       Simon Walker for providing guidelines for simple init and read functions
// Created:       November 23, 2022
// Details:       Initialize and read values from the AtoD module
// Revision History
//      each revision will have a date + desc. of changes
//      Nov. 23, 2022:   Implemented AtoD_Init and AtoD_Read.
//                       Created AtoD_InterruptMode and AtoD_Channels enums.
/////////////////////////////////////////////////////////////////////////////

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "atod.h"

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

// initialized the AtoD0 module
// assumes 20MHz bus rate
void AtoD_Init(AtoD_InterruptMode intMode)
{
    // power up the module, fast flag clearing on, run in Wait mode, no external trigger(5.3.2.3)
    ATD0CTL2 = 0b11000000;

    ATD0CTL2_ASCIE = (unsigned char)intMode; // enable or disable interrupts based on the parameter (5.3.2.3)

    ATD0CTL3 = 0b01000000; // scan all channels (5.3.2.4)

    ATD0CTL4 = 0b00000100; // set the prescale to 4 (for 20MHz) (5.3.2.5)

    // right-justified, unsigned results, continuous scan on multiple channels, starting at AN0 (5.3.2.6)
    ATD0CTL5 = 0b10110000;
}

// read the desired channel, assumes fast flag clearing from init
unsigned int AtoD_Read(AtoD_Channels channel)
{
    // check which channel is to be read
    // no need to check the flag because the speed at which samples are taken is so fast
    switch (channel)
    {
    case AtoD_Channel0:
        return ATD0DR0;
    case AtoD_Channel1:
        return ATD0DR1;
    case AtoD_Channel2:
        return ATD0DR2;
    case AtoD_Channel3:
        return ATD0DR3;
    case AtoD_Channel4:
        return ATD0DR4;
    case AtoD_Channel5:
        return ATD0DR5;
    case AtoD_Channel6:
        return ATD0DR6;
    case AtoD_Channel7:
        return ATD0DR7;
    }
    
}
/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Simon Walker
// Details:       Bring up the PIT at the desired interval on the specified channel
// Revision History
//      Created:  OCT 2021
/////////////////////////////////////////////////////////////////////////////

// interrupt handler for PIT Channel 0:
/*
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  PITTF = PITTF_PTF0_MASK; // clear the flag
}
*/

// interrupt handler for PIT Channel 1:
/*
interrupt VectorNumber_Vpit1 void PIT1Int (void)
{
  PITTF = PITTF_PTF0_MASK; // clear the flag
}
*/

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////
typedef enum PIT_Channel
{
  PIT_Channel_0,
  PIT_Channel_1,
  // PIT_Channel_2,
  // PIT_Channel_3
} PIT_Channel;

typedef enum PIT_Interrupt
{
  PIT_Interrupt_Off,
  PIT_Interrupt_On
} PIT_Interrupt;

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

// chan           : desired PIT channel (0-1 as enum)
// intState : turn on channel interrupt or not (0 == no interrupt)
// ulBusRate      : bus frequency, in Hz
// ulInterval_us  : desired interval in us (maximum is bus rate and 2^24 register limit dependent)
//      max interval is ~838.86ms at 20MHz bus rate
void PIT_Init(PIT_Channel chan, PIT_Interrupt intState, unsigned long ulBusRate, unsigned long ulInterval_us);

// blocking sleep for the specified number of ms, using the specified channel
//  requires init to reset enable of channel to reload counts
//  sleep can be longer than expected, as it has to eat the startup time
//  for init, so shorter delays will be longer than expected (.115ms in this lib)
void PIT_Sleep(PIT_Channel chan, unsigned long ulBusRate, unsigned int ms);

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

// finds the ideal or best fitting factors required to make the requested interval in us at a given bus rate
// return array: index 0 = PITMTLDx (8 bit), index 1 = PITLDx (16 bit)
// void FindFactors(uint *intervalFactors, unsigned long interval_us, unsigned long ulBusRate_MHz);
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "swled.h"
#include "timer.h"

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

///////////////////////////////////////////////////////////////
// Initialization - must be called to init port PT1AD1
///////////////////////////////////////////////////////////////

void SWLInit(void)
{
  PT1AD1 &= 0x1F;    // set initial LED
  DDR1AD1 = 0xE0;    // set data direction
  ATD1DIEN1 |= 0x1F; // enable buttons
}

///////////////////////////////////////////////////////////////
// Bitwise manipulation functions for PT1AD1
///////////////////////////////////////////////////////////////

// returns the state of the port bits in the mask
unsigned char SWLRead(byte mask)
{
  return PT1AD1 & mask;
}
void SWLSet(byte mask)
{
  // set the bits that are in the mask
  PT1AD1 |= mask;
}
void SWLClear(byte mask)
{
  // clear the bits that are in the mask
  PT1AD1 &= ~mask;
}
void SWLToggle(byte mask)
{
  // toggle the bits that are in the mask
  PT1AD1 ^= mask;
}

// is a specific switch being pushed? (T/F)
// in this case, only true if ALL bits in switch are
// set. i.e. if two bits set in switch, both switches are down.
unsigned char SWLPressed(eSWLSwitch button)
{
  if (PT1AD1 & button)
    return 1;
  else
    return 0;
}

// returns the true if the switch states have transitioned since they were last read
unsigned char SWLSwitchTransition(void)
{
  static unsigned char lastStates;
  unsigned char currentStates = SWLRead(SWLAny);//read the current state of the switches

  // check if a transition occured and return if one has or not
  if (currentStates != lastStates)
  {
    lastStates = currentStates; // save the current switch state for next time
    return 1;
  }
  else // return 0 (false) if no transition occured
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

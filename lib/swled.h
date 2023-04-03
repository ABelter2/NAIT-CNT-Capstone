/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     8 or 20 MHz (Later Requires Active PLL)
// Author:        AJ Armstrong
// Details:       Switch and indicator LED library
// Revision History:
// May 9th 2014 - Initial Port to C - Simon Walker
// June 2018 - Minor change to AnyButton, typedef on enum added
// Oct 2018 - Added enum switch function, separated debounce/non-debounce versions
// Dec 2020 - Simplified function names
// Sep 2021 - Debounce option added to Transition
// Jan 2022 - Fork by aja for 1212
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// Types
///////////////////////////////////////////////////////////////

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;

// Mask values for LEDs
typedef enum eSWLLED
{
  SWLRed = 0b10000000,
  SWLYellow = 0b01000000,
  SWLGreen = 0b00100000,
  SWLAll = 0b11100000
} eSWLLED;

// Mask values for Switches
typedef enum eSWLSwitch
{
  SWLMiddle = 0b00000001,
  SWLRight = 0b00000010,
  SWLDown = 0b00000100,
  SWLLeft = 0b00001000,
  SWLUp = 0b00010000,
  SWLAny = 0b00011111
} eSWLSwitch;

// values for switch transitons
typedef enum eSwitchTransition
{
  TransitionPressed = 1,
  TransitionReleased = 2,
  NoTransition = 3
} eSwitchTransition;

///////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// Initialization - must be called to init port PT1AD1
///////////////////////////////////////////////////////////////

void SWLInit(void); // Returns 0 for success or error code.

///////////////////////////////////////////////////////////////
// Bitwise manipulation functions for PT1AD1
///////////////////////////////////////////////////////////////

// SWLRead returns the state of the port bits in the mask
// possibly after a debounce delay for buttons.
unsigned char SWLRead(byte mask); // True if any bit in mask is set
void SWLSet(byte mask);
void SWLClear(byte mask);
void SWLToggle(byte mask);

// is a specific switch being pushed? (T/F)
// in this case, only true if ALL bits in switch are
// set. i.e. if two bits set in switch, both switches
// are down.
unsigned char SWLPressed(eSWLSwitch button);

// returns the states of the switches if a transition was detected
unsigned char SWLSwitchTransition(void);
/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

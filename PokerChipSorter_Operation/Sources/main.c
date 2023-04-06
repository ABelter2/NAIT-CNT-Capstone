/* ////////////////////////////////////////////////////////////////////////////
// 9S12X Program: CMPE2965 - Poker Chip Sorter (Operation Mode)
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Details:       This code is for the main operation mode of the Automatic Poker Chip Sorter
// Date:          March 27, 2023
/////////////////////////////////////////////////////////////////////////// */
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "Capstone.h"   /* CMPE2965 capstone specific library */
#include "lcd.h"        /* LCD library */
#include "misc.h"       /* Misc library */
#include "pit.h"        /* PIT library */
#include "pll.h"        /* increase bus speed */
#include "sci.h"        /* SCI Library */
#include "segs.h"       /* 7 segs library */
#include "swled.h"      /* switches and LEDs */
#include "timer.h"      /* timer library */
#include "pulse.h"      /* PWM Library */

// other system includes or your includes go here
// #include <stdlib.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////

// display mode for determining whether to display status updates or chip count
typedef enum DisplayMode
{
  Display_Status,
  Display_Count
} DisplayMode;

typedef enum Colours
{
  Colour_Red = 'r',
  Colour_Green = 'g',
  Colour_Blue = 'b',
  Colour_White = 'w',
  Colour_Black = 'k',
  Colour_Other = 'o',
} Colours;

typedef enum ColourCountIndex
{
  Count_Red,
  Count_Green,
  Count_Blue,
  Count_White,
  Count_Black,
  Count_Other
} ColourCountIndex;

typedef enum OperatingState
{
  State_Isolate,
  State_Analyse,
  State_Pickup,
  State_Place
} OperatingState;

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////

void UpdateDisplay(OperatingState state);
void IsolateChip(void);
void DetermineColour(void);
void UpdateColour(Colours colour);

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

volatile char IsRunning = 0; // 1 = running, 0 = stopped - used as a break condition to stop operation
volatile DisplayMode displayMode = Display_Status;
volatile unsigned int ChipCount[6] = {0};

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

const unsigned long GlobalBusRate = 20E6; // the board bus rate
const unsigned int MinServoDuty = 350;    // min servo duty cycle for lowest servo position
const unsigned int MaxServoDuty = 1250;   // max servo duty cycle for furthest servo position

/////////////////////////////////////////////////////////////////////////////
// Main Entry
/////////////////////////////////////////////////////////////////////////////
void main(void)
{

  /////////////////////////////////////////////////////////////////////////////
  // Local Variables
  /////////////////////////////////////////////////////////////////////////////
  OperatingState opState = State_Isolate;

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  SWLInit();
  LCD_Init();
  Segs_Init();
  Cap_PortAInit();
  PortJ_Init(PortJ_Option_On, PortJ_Option_On);
  Pulse_Init_16Bit(Pulse_Channel7, Pulse_PrescaleStage1_1, 20, Pulse_PolatityPositive, 10000, 0);

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    // update display
    UpdateDisplay(opState);

    // do not proceed if not running operation
    if (!IsRunning)
      continue;

    switch (opState)
    {
    case State_Isolate:
      // chip isolator
      IsolateChip();
      break;
    case State_Analyse:
      // colour detection
      DetermineColour();
      break;
    case State_Pickup:
      // move to chip

      Cap_PortASet(PumpControl); // pump on
      break;
    case State_Place:
      // move to container

      Cap_PortAClear(PumpControl); // pump off
      break;
    }
  } // end main program loop
} // end main method

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

void UpdateDisplay(OperatingState state)
{
  // display the count
  if (displayMode == Display_Count)
  {
    char buffer[21] = {0};

    LCD_StringXY(0, 1, "Chip Count:         ");

    (void)sprintf(buffer, "Red:%5u Grn:%5u ", ChipCount[Count_Red], ChipCount[Count_Green]);
    LCD_StringXY(0, 1, buffer);

    (void)sprintf(buffer, "Blu:%5u Wht:%5u ", ChipCount[Count_Blue], ChipCount[Count_White]);
    LCD_StringXY(0, 2, buffer);

    (void)sprintf(buffer, "Blk:%5u Oth:%5u ", ChipCount[Count_Black], ChipCount[Count_Other]);
    LCD_StringXY(0, 3, buffer);

    return;
  }

  // update display for stopped mode
  if (!IsRunning)
  {
    return;
  }

  switch (state)
  {
  case State_Isolate:
    break;
  case State_Analyse:
    break;
  case State_Pickup:
    break;
  case State_Place:
    break;
  }
}

void IsolateChip(void)
{
  // move slot fully forward (servo to max) and sleep
  Pulse_SetDuty_16Bit(Pulse_Channel7, MinServoDuty);
  PIT_Sleep_ms(PIT_Channel_1, GlobalBusRate, 500);

  // move slot fully out (servo to min) and sleep
  Pulse_SetDuty_16Bit(Pulse_Channel7, MaxServoDuty);
  PIT_Sleep_ms(PIT_Channel_1, GlobalBusRate, 500);
}

void DetermineColour(void)
{
  Colours colour; // declare an instance of Colours to populate with the SCI0 read

  // send start signal 'a' (for analyse) with SCI0
  SCI0_TxByte_Block('a');

  // use a loop to read from SCI0 - allows for breaking from the loop with an exit condition
  //  would have to check the exit condition after so it does not continue
  while (SCI0_Read(&colour) && IsRunning)
    ;

  if (!IsRunning)
    return;

  // display the identified colour and update the chip count
  UpdateColour(colour);
}

void UpdateColour(Colours colour)
{
  // display the colour on the LCD and increment the chip count
  switch (colour)
  {
  case Colour_Red:
    LCD_StringXY(0, 3, "Colour : Red        ");
    ChipCount[Count_Red] += 1;
    break;
  case Colour_Green:
    LCD_StringXY(0, 3, "Colour : Green      ");
    ChipCount[Count_Green] += 1;
    break;
  case Colour_Blue:
    LCD_StringXY(0, 3, "Colour : Blue       ");
    ChipCount[Count_Blue] += 1;
    break;
  case Colour_White:
    LCD_StringXY(0, 3, "Colour : White      ");
    ChipCount[Count_White] += 1;
    break;
  case Colour_Black:
    LCD_StringXY(0, 3, "Colour : Black      ");
    ChipCount[Count_Black] += 1;
    break;
  case Colour_Other:
    LCD_StringXY(0, 3, "Colour : Other      ");
    ChipCount[Count_Other] += 1;
    break;
  default: // should never occur - stops running if somehow encountered
    LCD_StringXY(0, 3, "Unexpected character");
    IsRunning = 0;
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////

interrupt VectorNumber_Vportj void IntJ(void)
{
  // PJ0 for toggling operation mode
  if (PIFJ_PIFJ0)
  {
    // acknowledge interrupt
    PIFJ = PIFJ_PIFJ0_MASK;

    // toggle the operation mode
    IsRunning ^= 1;
  }

  // PJ1 for managing display states
  if (PIFJ_PIFJ1)
  {
    // acknowledge interrupt
    PIFJ = PIFJ_PIFJ1_MASK;

    // if PJ1 is pressed while Middle is also pressed, clear the chip count
    // otherwise toggle the display mode
    if (SWLPressed(SWLMiddle))
    {
      char i;
      // clear the chip count
      for (i = 0; i < 6; ++i)
        ChipCount[i] = 0;
    }
    else
      displayMode ^= 1; // toggle the display mode
  }
}

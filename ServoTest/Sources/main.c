/* ////////////////////////////////////////////////////////////////////////////
// 9S12X Program: YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Details:       Description Here
// Date:          Date Created
/////////////////////////////////////////////////////////////////////////// */
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "swled.h"
#include "segs.h"
#include "timer.h" /* timer library */
#include "pll.h"   /* increase bus speed */
#include "pulse.h" /* PWM Pulse library */

// other system includes or your includes go here
// #include <stdlib.h>
// #include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Main Entry
/////////////////////////////////////////////////////////////////////////////
void main(void)
{

  /////////////////////////////////////////////////////////////////////////////
  // Local Variables
  /////////////////////////////////////////////////////////////////////////////
  unsigned int duty = 500;
  unsigned char sleepDuration = 1;

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  SWLInit();
  Segs_Init();
  Pulse_Init_16Bit(Pulse_Channel7, Pulse_PrescaleStage1_1, 20, Pulse_PolatityPositive, 10000, 0);

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  // #define FindRange
// #define RangeSweep
#define EdgeBounce
  for (;;)
  {
#ifdef FindRange
    // as switches are pressed, the duty value increases or decreases to find the actual range of duty values given a period of 10000
    if (SWLSwitchTransition())
    {
      if (SWLPressed(SWLUp))
        duty += 100;
      else if (SWLPressed(SWLDown))
        duty -= 100;
      else if (SWLPressed(SWLRight))
        duty += 10;
      else if (SWLPressed(SWLLeft))
        duty -= 10;
      Segs_16D(duty, Segs_LineTop);
      Pulse_SetDuty_16Bit(Pulse_Channel7, duty);
      Timer_Sleep(20);
    }
#endif

#ifdef RangeSweep
    // duty value needs to range from 200-1300
    // 350-1250 seems more accurate but not perfect
    while (duty > 350)
    {
      Pulse_SetDuty_16Bit(Pulse_Channel7, duty -= 50);
      Timer_Sleep(sleepDuration);
    }
    while (duty < 1250)
    {
      Pulse_SetDuty_16Bit(Pulse_Channel7, duty += 50);
      Timer_Sleep(sleepDuration);
    }
#endif

#ifdef EdgeBounce
    Pulse_SetDuty_16Bit(Pulse_Channel7, 350);
    Timer_Sleep(500);
    Pulse_SetDuty_16Bit(Pulse_Channel7, 1250);
    Timer_Sleep(500);
#endif

  } // end main program loop
} // end main method

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
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
#include "pll.h"        /* increase bus speed */
#include "swled.h"      /* switches and LEDs */
#include "timer.h"      /* timer library */
#include "segs.h"       /* 7 segs library */
#include "lcd.h"        /* LCD library */
#include "pit.h"        /* PIT library */
#include "misc.h"       /* Misc library */
#include "pulse.h"      /* PWM Pulse library */
#include "sci.h"        /* SCI Library */
#include "escseq.h"     /* Escape Sequency Library */
#include "atod.h"       /* AtoD library */
#include "Capstone.h"   /* Capstone misc library*/

// other system includes or your includes go here
// #include <stdlib.h>
// #include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
volatile char pitFlag = 0;
volatile int lockout = 0;

#define SyncedTest
// #define StaggeredTest

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
  int mStep = 0;
  const int StepSize = 50;
  const int MaxStep = 500;

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  SWLInit();
  Segs_Init();
  PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 100);
  Cap_PortAInit();

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    ////3 times the number of pulses/steps due to the 3:1 gear ratio
    // #ifdef SyncedTest
    //     Cap_MoveEffector(2400, 2400, 2400);
    // #endif

    // #ifdef StaggeredTest
    //     Cap_MoveEffector(2400, 1600, 800);
    // #endif

    // PIT_Sleep_ms(PIT_Channel_1, 20E6, 2000);
    // Cap_MoveEffector(0, 0, 0);
    // PIT_Sleep_ms(PIT_Channel_1, 20E6, 2000);

    if (!lockout && SWLSwitchTransition())
    {
      lockout = 10;
      SWLSet(SWLYellow);

      if (SWLPressed(SWLUp) && mStep - StepSize >= 0)
      {
        SWLSet(SWLGreen);
        mStep -= StepSize;
        Cap_MoveEffector(mStep, mStep, mStep);
        SWLClear(SWLGreen);
      }
      else if (SWLPressed(SWLDown) && mStep + StepSize <= MaxStep)
      {
        SWLSet(SWLRed);
        mStep += StepSize;
        Cap_MoveEffector(mStep, mStep, mStep);
        SWLClear(SWLRed);
      }

      SWLClear(SWLYellow);
    }

  } // end main program loop

} // end main method

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////
interrupt VectorNumber_Vpit0 void PIT0Int(void)
{
  PITTF = PITTF_PTF0_MASK; // clear the flag
  pitFlag = 1;
  if (lockout)
    --lockout;
}

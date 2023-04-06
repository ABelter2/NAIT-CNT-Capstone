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

// #define GeneralTesting
// #define SinglePortM_MotorTest
// #define DoublePortA_MotorTest
// #define SingleContinuousPortA_MotorTest
#define PumpTest

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////
volatile char pitFlag = 0;
volatile int lockout = 0;

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
const int
    M1MaxSteps = 800, // 90 degrees
    M2MaxSteps = 800, // 90 degrees
    M3MaxSteps = 800; // 90 degrees

const int
    M1MinSteps = 0, // 0 degrees
    M2MinSteps = 0, // 0 degrees
    M3MinSteps = 0; // 0 degrees

/////////////////////////////////////////////////////////////////////////////
// Main Entry
/////////////////////////////////////////////////////////////////////////////
#ifdef GeneralTesting
void main(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Local Variables
  /////////////////////////////////////////////////////////////////////////////
  unsigned int
      M1SCurrentSteps = M1MaxSteps,
      M2SCurrentSteps = M2MaxSteps,
      M3SCurrentSteps = M3MaxSteps;

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  SWLInit();
  Segs_Init();
  PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 50);

  // init PortA all bits are outputs
  DDRA = 0xFF;
  PORTA = 0b01000000;
  SWLSet(SWLRed);

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    // check if pulses are needed
    if (pitFlag)
    {
      pitFlag = 0;

      if (!MotorDisable)
      {
        /*
        // motor 1
        if (M1SCurrentSteps >= M1MaxSteps)
          M1Direction = 0;
        else if (M1SCurrentSteps <= M1MinSteps)
          M1Direction = 1;

        M1StepPulse ^= 1;
        if (!M1StepPulse)
        {
          if (M1Direction)
            ++M1SCurrentSteps;
          else
            --M1SCurrentSteps;
        }

        // motor 2
        if (M2SCurrentSteps >= M2MaxSteps)
          M2Direction = 0;
        else if (M2SCurrentSteps <= M2MinSteps)
          M2Direction = 1;
        M2StepPulse ^= 1;
        if (!M2StepPulse)
        {
          if (M2Direction)
            ++M2SCurrentSteps;
          else
            --M2SCurrentSteps;
        }

        // motor 3
        M3StepPulse ^= 1;
        if (M3SCurrentSteps >= M3MaxSteps)
          M3Direction = 0;
        else if (M3SCurrentSteps <= M3MinSteps)
          M3Direction = 1;
        if (!M3StepPulse)
        {
          if (M3Direction)
            ++M3SCurrentSteps;
          else
            --M3SCurrentSteps;
        }
        */
        M3StepPulse ^= 1; // toggle pulse
        SWLToggle(SWLYellow);
        // if (M3StepPulse)
        // {
        //   if (M3Direction)
        //     ++M3SCurrentSteps;
        //   else
        //     --M3SCurrentSteps;
        // }
      }
    }

    // check for top press to enable motors
    if (SWLPressed(SWLUp))
    {
      MotorDisable = 0;
      SWLSet(SWLGreen);
      SWLClear(SWLRed);
    }

    // check for down press to disable motors
    else if (SWLPressed(SWLDown))
    {
      MotorDisable = 1;
      SWLSet(SWLRed);
      SWLClear(SWLGreen);
    }
    else if (SWLPressed(SWLLeft))
      M3Direction = 0;
    else if (SWLPressed(SWLRight))
      M3Direction = 1;

  } // end main program loop
} // end main method

#endif

#ifdef SinglePortM_MotorTest
void main(void)
{
  typedef enum MotorPins
  {
    Motor_Enable = PTM_PTM6_MASK,
    Motor_Direction = PTM_PTM5_MASK,
    Motor_StepPulse = PTM_PTM4_MASK
  } MotorPins;

  /////////////////////////////////////////////////////////////////////////////
  // Local Variables
  /////////////////////////////////////////////////////////////////////////////
  int currentStep = 0;

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  SWLInit();
  PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 50);

  DDRM |= 0b11110000; // PTM4 - 7 as outputs

  // set enable, clear steppulse and direction
  PTM |= Motor_Enable;
  PTM &= ~(Motor_StepPulse & Motor_Direction);

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    // if middle is pressed, toggle the direction
    if (SWLPressed(SWLUp) && currentStep < 800 && !PTM_PTM6)
    {
      PTM |= Motor_Direction;
      if (pitFlag)
      {
        pitFlag = 0;

        if (PTM_PTM4)
          PTM_PTM4 = 0;
        else
        {
          PTM_PTM4 = 1;
          ++currentStep;
        }
      }
    }
    else if (SWLPressed(SWLDown) && currentStep > -800 && !PTM_PTM6)
    {
      PTM &= ~Motor_Direction;
      if (pitFlag)
      {
        pitFlag = 0;
        if (PTM_PTM4)
          PTM_PTM4 = 0;
        else
        {
          PTM_PTM4 = 1;
          --currentStep;
        }
      }
    }
    else if (SWLPressed(SWLMiddle) && !lockout)
    {
      lockout = 5000;
      PTM ^= Motor_Enable;
      if (!PTM_PTM6)
        SWLSet(SWLGreen);
      else
        SWLClear(SWLGreen);
    }

  } // end main program loop
} // end main method
#endif

#ifdef DoublePortA_MotorTest
void main(void)
{
  typedef enum MotorPins
  {
    Motor_Enable = PORTA_PA6_MASK,     // Pin 63
    Motor2_Direction = PORTA_PA3_MASK, // Pin 60
    Motor2_StepPulse = PORTA_PA2_MASK, // Pin 59
    Motor3_Direction = PORTA_PA5_MASK, // Pin 62
    Motor3_StepPulse = PORTA_PA4_MASK  // Pin 61
  } MotorPins;

  /////////////////////////////////////////////////////////////////////////////
  // Local Variables
  /////////////////////////////////////////////////////////////////////////////
  int currentStep = 0;

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  SWLInit();
  PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 50);

  DDRA = 0xFF; // PTA all outputs

  // set enable, clear the rest
  PORTA = 0;
  PORTA |= Motor_Enable;

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    // if middle is pressed, toggle the direction
    if (SWLPressed(SWLUp) && currentStep < 1600 && !(PORTA & Motor_Enable))
    {
      PORTA |= Motor2_Direction & Motor3_Direction;
      if (pitFlag)
      {
        pitFlag = 0;

        PORTA ^= Motor2_StepPulse;
        PORTA ^= Motor3_StepPulse;
        ++currentStep;
      }
    }
    else if (SWLPressed(SWLDown) && currentStep > -1600 && !(PORTA & Motor_Enable))
    {
      PORTA &= ~(Motor2_Direction & Motor3_Direction);
      if (pitFlag)
      {
        pitFlag = 0;

        PORTA ^= Motor2_StepPulse;
        PORTA ^= Motor3_StepPulse;
        --currentStep;
      }
    }
    else if (SWLPressed(SWLMiddle) && !lockout)
    {
      lockout = 5000;
      PORTA ^= Motor_Enable;
      if (!(PORTA & Motor_Enable))
        SWLSet(SWLGreen);
      else
        SWLClear(SWLGreen);
    }

  } // end main program loop
} // end main method
#endif

#ifdef SingleContinuousPortA_MotorTest
void main(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Local Variables
  /////////////////////////////////////////////////////////////////////////////
  int currentStep = 0;

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  SWLInit();
  PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 50);
  Cap_PortAInit();

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    if (pitFlag)
    {
      // clear the flag
      pitFlag = 0;

      // check that the motor is enabled
      if (Cap_PortARead(MotorDisable))
      {
        // toggle the pulse
        Cap_PortAToggle(Motor2StepPulse);
        Cap_PortAToggle(Motor3StepPulse);

        // update current step
        if (Cap_PortARead(Motor3Direction))
          ++currentStep;
        else
          --currentStep;

        // switch directions at boundaries
        if (currentStep < -3200)
        {
          Cap_PortASet(Motor2Direction);
          Cap_PortASet(Motor3Direction);
        }
        else if (currentStep > 3200)
        {
          Cap_PortAClear(Motor2Direction);
          Cap_PortAClear(Motor3Direction);
        }
      }
    }

    if (SWLPressed(SWLMiddle) && !lockout)
    {
      lockout = 5000;
      Cap_PortAToggle(MotorDisable);
      if (Cap_PortARead(MotorDisable))
      {
        SWLSet(SWLGreen);
        SWLClear(SWLRed);
      }
      else
      {
        SWLSet(SWLRed);
        SWLClear(SWLGreen);
      }
    }

  } // end main program loop
} // end main method
#endif

#ifdef PumpTest
void main(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Local Variables
  /////////////////////////////////////////////////////////////////////////////
  int currentStep = 0;

  // main entry point - these two lines must appear first
  _DISABLE_COP();
  EnableInterrupts;

  /////////////////////////////////////////////////////////////////////////////
  // one-time initializations
  /////////////////////////////////////////////////////////////////////////////
  PLL_To20MHz();
  SWLInit();
  // PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 50);
  Cap_PortAInit();

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    if (SWLPressed(SWLMiddle))
      Cap_PortASet(PumpControl);
    else
      Cap_PortAClear(PumpControl);
  } // end main program loop
} // end main method
#endif
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
  if (lockout > 0)
    lockout--;
}

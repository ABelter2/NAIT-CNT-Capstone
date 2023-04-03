/* ////////////////////////////////////////////////////////////////////////////
// 9S12X Program: CMPE2965 - Poker Chip Sorter (Testing Mode)
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Details:       This code is for the testing/verification mode of the Automatic Poker Chip Sorter
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

// other system includes or your includes go here
// #include <stdlib.h>
// #include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////

typedef enum SelectedTest
{
  Test_Instructions,
  Test_ChipIsolator,
  Test_ColourIdentification,
  Test_ChipSorting
} SelectedTest;

typedef enum SortingTestColour
{
  Sorting_Instructions,
  Sorting_Red,
  Sorting_Green,
  Sorting_Blue,
  Sorting_White,
  Sorting_Black,
  Sorting_Other
} SortingTestColour;

typedef enum Colours
{
  Colour_Red = 'r',
  Colour_Green = 'g',
  Colour_Blue = 'b',
  Colour_White = 'w',
  Colour_Black = 'k',
  Colour_Other = 'o',
} Colours;

/////////////////////////////////////////////////////////////////////////////
// Local Prototypes
/////////////////////////////////////////////////////////////////////////////

void UpdateTestDisplay(SelectedTest currentTest);
void UpdateSortingColour(SortingTestColour currentSortingColour);
void PerformTest(SelectedTest currentTest, SortingTestColour currentSortingColour);
void ChipIsolatorTest(void);
void ColourIdentificationTest(void);
void DisplayIdentifiedColour(Colours colour);
void ChipSortingTest(SortingTestColour currentSortingColour);

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

const unsigned long GlobalBusRate = 20E6; // the board bus rate
volatile unsigned int buttonLockout = 0;  // the lockout for switch debouncing

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

  SelectedTest currentTest = Test_Instructions;
  SortingTestColour sortingTestColour = Sorting_Red;
  unsigned char runningTest = 0;

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
  PIT_Init(PIT_Channel_0, PIT_Interrupt_On, GlobalBusRate, 10000); // PIT0 @ 10ms intervals, interrupts on
  SCI0_Init(GlobalBusRate, BaudRate_9600, SCI_RDRF_InterruptOff);  // SCI0 @ 9600 bause
  UpdateTestDisplay(currentTest);
  SWLSet(SWLGreen);

  /////////////////////////////////////////////////////////////////////////////
  // main program loop
  /////////////////////////////////////////////////////////////////////////////
  for (;;)
  {
    // if there is no test currently running and no button lockout, check for button presses
    if (!buttonLockout && SWLSwitchTransition())
    {
      buttonLockout = 10; // set the lockout for 100ms to de-bounce buttons

      // increment sorting test colour if that test is selected and Up is pressed
      if (currentTest == Test_ChipSorting && SWLPressed(SWLUp))
      {
        // increment sortingTestColour, wrapping at end, then update the display
        if (++sortingTestColour > Sorting_Other)
          sortingTestColour = Sorting_Instructions;
        UpdateSortingColour(sortingTestColour);
      }
      // decrement sorting test colour if that test is selected and Down is pressed
      else if (currentTest == Test_ChipSorting && SWLPressed(SWLDown))
      {
        // decrement sortingTestColour, wrapping at start, then update the display
        if (--sortingTestColour < Sorting_Instructions)
          sortingTestColour = Sorting_Other;
        UpdateSortingColour(sortingTestColour);
      }
      // decrement selected test if Left is pressed
      else if (SWLPressed(SWLLeft))
      {
        // decrement currentTest, wrapping at start, then update the display
        if (--currentTest < Test_Instructions)
          currentTest = Test_ChipSorting;
        UpdateTestDisplay(currentTest);
      }
      // increment selected test if Right is pressed
      else if (SWLPressed(SWLRight))
      {
        // increment currentTest, wrapping at end, then update the display
        if (++currentTest > Test_ChipSorting)
          currentTest = Test_Instructions;
        UpdateTestDisplay(currentTest);
      }
      // perform the currently selected test if Middle is pressed
      else if (SWLPressed(SWLMiddle))
      {
        // turn on red and off green to show a test is taking place
        SWLSet(SWLRed);
        SWLClear(SWLGreen);

        // start the test - all tests are blocking so controls are disabled during test
        PerformTest(currentTest, sortingTestColour);

        // turn on green and off red to show the test is complete
        SWLSet(SWLGreen);
        SWLClear(SWLRed);
      }
    }
  } // end main program loop
} // end main method

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

void UpdateTestDisplay(SelectedTest currentTest)
{
  // change the display line based on the currentTest argument
  switch (currentTest)
  {
  case Test_Instructions:
    LCD_StringXY(0, 0, "  Sorter Test Mode  ");
    LCD_StringXY(0, 1, "Left/Right Swap Test");
    LCD_StringXY(0, 2, "Up/Down Test Options");
    LCD_StringXY(0, 3, " Middle starts test ");
    break;
  case Test_ChipIsolator:
    LCD_StringXY(0, 0, " Chip Isolator Test ");
    LCD_StringXY(0, 1, " Fill up chip tower ");
    LCD_StringXY(0, 2, "Pressing middle gets");
    LCD_StringXY(0, 3, " a single poker chip");
    break;
  case Test_ColourIdentification:
    LCD_StringXY(0, 0, "  Chip Colour Test  ");
    LCD_StringXY(0, 1, " Place chip in slot ");
    LCD_StringXY(0, 2, "Press middle to scan");
    LCD_StringXY(0, 3, "Colour displays here");
    break;
  case Test_ChipSorting:
    LCD_StringXY(0, 0, "   Chip Sort Test   ");
    LCD_StringXY(0, 1, " Place chip in slot ");
    LCD_StringXY(0, 2, "Up/Down swaps colour");
    LCD_StringXY(0, 3, "Press middle to move");
    break;
  }
}

void UpdateSortingColour(SortingTestColour currentSortingColour)
{
  // change the colour/instruction line based on the currentSortingColour argument
  switch (currentSortingColour)
  {
  case Sorting_Instructions:
    LCD_StringXY(0, 2, "Up/Down swaps colour");
    break;
  case Sorting_Red:
    LCD_StringXY(0, 2, "Colour : Red        ");
    break;
  case Sorting_Green:
    LCD_StringXY(0, 2, "Colour : Green      ");
    break;
  case Sorting_Blue:
    LCD_StringXY(0, 2, "Colour : Blue       ");
    break;
  case Sorting_White:
    LCD_StringXY(0, 2, "Colour : White      ");
    break;
  case Sorting_Black:
    LCD_StringXY(0, 2, "Colour : Black      ");
    break;
  case Sorting_Other:
    LCD_StringXY(0, 2, "Colour : Other      ");
    break;
  }
}

// calls the required function to perform the selected test
void PerformTest(SelectedTest currentTest, SortingTestColour currentSortingColour)
{
  switch (currentTest)
  {
  case Test_ChipIsolator:
    ChipIsolatorTest();
    break;
  case Test_ColourIdentification:
    ColourIdentificationTest();
    break;
  case Test_ChipSorting:
    ChipSortingTest(currentSortingColour);
    break;
  }
}

// performs the chip isolator test
void ChipIsolatorTest()
{
  // move servo in to grab one chip

  // move servo out to extract one chip
}

// performs the colour identification test
void ColourIdentificationTest()
{
  Colours colour; // declare an instance of Colours to populate with the SCI0 read

  // send start signal 'a' (for analyse) with SCI0
  SCI0_TxByte_Block('a');

  // blocking wait for returned colour character from SCI0
  colour = SCI0_BRead();

  // echo the received character back as an acknowledgement of recieval
  // SCI0_TxByte_Block((unsigned char)colour);//debug line

  // display the colour on the LCD
  DisplayIdentifiedColour(colour);
}

void DisplayIdentifiedColour(Colours colour)
{
  // display the colour on the LCD
  switch (colour)
  {
  case Colour_Red:
    LCD_StringXY(0, 3, "Colour : Red        ");
    break;
  case Colour_Green:
    LCD_StringXY(0, 3, "Colour : Green      ");
    break;
  case Colour_Blue:
    LCD_StringXY(0, 3, "Colour : Blue       ");
    break;
  case Colour_White:
    LCD_StringXY(0, 3, "Colour : White      ");
    break;
  case Colour_Black:
    LCD_StringXY(0, 3, "Colour : Black      ");
    break;
  case Colour_Other:
    LCD_StringXY(0, 3, "Colour : Other      ");
    break;
  default:
    LCD_StringXY(0, 3, "Unexpected character");
    break;
  }
}

// performs the chip sorting test
void ChipSortingTest(SortingTestColour currentSortingColour)
{
  // move to chip

  // turn on pump (pick up chip)

  // based on the selected colour, move the motor to the position mapped to that colour
  switch (currentSortingColour)
  {
  case Sorting_Red:
    break;
  case Sorting_Green:
    break;
  case Sorting_Blue:
    break;
  case Sorting_White:
    break;
  case Sorting_Black:
    break;
  case Sorting_Other:
    break;
  }

  // turn off pump (drop chip in container)

  // return effector to start
}

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
/////////////////////////////////////////////////////////////////////////////

interrupt VectorNumber_Vpit0 void PIT0Int(void)
{
  PITTF = PITTF_PTF0_MASK; // clear the flag

  // decrement the lockout until cleared
  if (buttonLockout)
    --buttonLockout;
}
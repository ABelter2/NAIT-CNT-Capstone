#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "misc.h"

// other includes, as *required* for this implementation
#include <stdio.h>

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

// set up Port J for interrupts
void PortJ_Init(PortJ_Option pj0, PortJ_Option pj1)
{
    PTJ &= 0b11111100; // clear port J or will interrupt once for free (22.3.2.54)

    if (pj0 == PortJ_Option_On)
    {
        DDRJ &= 0b11111101; // j0 inputs (22.3.2.56)
        PPSJ |= 0b00000001; // j0 rising edge (22.3.2.59)
        PIEJ |= 0b00000001; // j0 cause interrupts (22.3.2.60)
    }

    if (pj1 == PortJ_Option_On)
    {
        DDRJ &= 0b11111110; // j1 inputs (22.3.2.56)
        PPSJ |= 0b00000010; // j1 rising edge (22.3.2.59)
        PIEJ |= 0b00000010; // j1 cause interrupts (22.3.2.60)
    }
}

// convert a time in ms to a string in format hh:mm:ss.ms (ms to nearest 100ms (seconds rounded to 1 decimal place))
char *FormatTimeMs(unsigned long ms)
{
  char timeBuffer[200] = {0};
  unsigned int hours, minutes, seconds;

  // find the number of hours and remaining ms
  hours = (unsigned int)(ms / 3600000);
  ms %= 3600000;

  // find the number of minutes and remaining ms
  minutes = (unsigned int)(ms / 60000);
  ms %= 60000;

  // find the number of seconds and remaining ms
  seconds = (unsigned int)(ms / 1000);
  ms %= 1000;

  // round to the nearest 100ms (tenth of a second)
  ms /= 100;

  // fill the buffer with the times calculated in the format hh:mm:ss.ms, then return the buffer
  (void)sprintf(timeBuffer, "%02d:%02d:%02d.%01lu", hours, minutes, seconds, ms);
  return timeBuffer;
}
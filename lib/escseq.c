/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Created:
// Details:       Constants and functions for commonly uses escape sequences for SCI
// Revision History
//          Nov. 15, 2022 - Created constants, enums, and functions for common escape sequences
/////////////////////////////////////////////////////////////////////////////

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "escseq.h"

// other includes, as *required* for this implementation
#include <stdio.h> //for sprintf

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////
// static char const CursorHome[] = "\x1B[{row};{column}H";
void ESC_CursorHome(char* buffer, unsigned int row, unsigned int column)
{
    (void)sprintf(buffer, "\x1B[%i;%iH", row, column);
}

// up=\x1B[{COUNT}A, down=\x1B[{COUNT}B, forward=\x1B[{COUNT}C, backward=\x1B[{COUNT}D
void ESC_MoveCursor(char* buffer, ESC_CursorDirection direction, unsigned int count)
{
    switch (direction)
    {
    case ESC_CursorUp:
        (void)sprintf(buffer, "\x1B[%iA", count);
        break;
    case ESC_CursorDown:
        (void)sprintf(buffer, "\x1B[%iB", count);
        break;
    case ESC_CursorBackwards:
        (void)sprintf(buffer, "\x1B[%iC", count);
        break;
    case ESC_CursorForwards:
        (void)sprintf(buffer, "\x1B[%iD", count);
        break;
    }
}
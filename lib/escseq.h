/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Created:
// Details:       Constants and functions for commonly uses escape sequences for SCI
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
/* Common Escape Sequences */

// Cursor Control :
static char const ESC_SaveCursor[] = "\x1B[s";
static char const ESC_UnsaveCursor[] = "\x1B[u";

// Display Attributes :
// Template : "\x1B[{attr1};...;{attrN}m";
// General :
static char const ESC_ResetAttributes[] = "\x1B[0m";
static char const ESC_GoBright[] = "\x1B[1m";
static char const ESC_GoDim[] = "\x1B[2m";
static char const ESC_UnderscoreCursor[] = "\x1B[4m";
static char const ESC_BlinkCursor[] = "\x1B[5m";
static char const ESC_GoReverse[] = "\x1B[7m";
static char const ESC_HiddenCursor[] = "\x1B[8m";
// Foreground Colour :
static char const ESC_ForeBlack[] = "\x1B[30m";
static char const ESC_ForeRed[] = "\x1B[31m";
static char const ESC_ForeGreen[] = "\x1B[32m";
static char const ESC_ForeYellow[] = "\x1B[33m";
static char const ESC_ForeBlue[] = "\x1B[34m";
static char const ESC_ForeMagenta[] = "\x1B[35m";
static char const ESC_ForeCyan[] = "\x1B[36m";
static char const ESC_ForeWhite[] = "\x1B[37m";
// Background Colour :
static char const ESC_BackBlack[] = "\x1B[40m";
static char const ESC_BackRed[] = "\x1B[41m";
static char const ESC_BackGreen[] = "\x1B[42m";
static char const ESC_BackYellow[] = "\x1B[43m";
static char const ESC_BackBlue[] = "\x1B[44m";
static char const ESC_BackMagenta[] = "\x1B[45m";
static char const ESC_BackCyan[] = "\x1B[46m";
static char const ESC_BackWhite[] = "\x1B[47m";

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////
typedef enum EscSeq_CursorDirection
{
    ESC_CursorUp,
    ESC_CursorDown,
    ESC_CursorBackwards,
    ESC_CursorForwards
} ESC_CursorDirection;

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////
// static char const CursorHome[] = "\x1B[{row};{column}H";
void ESC_CursorHome(char *buffer, unsigned int row, unsigned int column);

// up=\x1B[{COUNT}A, down=\x1B[{COUNT}B, forward=\x1B[{COUNT}C, backward=\x1B[{COUNT}D
void ESC_MoveCursor(char *buffer, ESC_CursorDirection direction, unsigned int count);

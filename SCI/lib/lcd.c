#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "LCD.h"
#include "timer.h"

// other includes, as *required* for this implementation
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////////////////
// macros only apply to library, not for public consumption

// gotta be inputs, set R/W* high
#define LCD_RWUp \
    DDRH = 0;    \
    PORTK |= 2;

// set R/W* low, gotta be outputs
#define LCD_RWDown \
    PORTK &= (~2); \
    DDRH = 0xFF;

// this is *snug*, datasheet says PW_EH must be at least 450ns. (P49 + P58)
// but &= probably implemented as a BCLR instruction (@ 50ns bus, 4 cycles, around 200ns per instruction)
// measured on scope at ~300ns (which would make sense (6 cycles up/down instructions),
// not exactly sure when pin changes state in instruction pair)
// this is actually too short, but appears to work. Either the device is better than expected, or does not match datasheet!
// other LCDs may require a small delay be added to E strobe operations, or data setup time during read.
#define LCD_EUp PORTK |= 1;
#define LCD_EDown PORTK &= (~1);
#define LCD_RSUp PORTK |= 4;
#define LCD_RSDown PORTK &= (~4);

// small delay required on read. LCD timing seems to vary on different devices
// if issues are discovered, increasing __x below should be tested first
#define LCD_MicroDelay \
    {                  \
        char __x = 1;  \
        while (--__x)  \
            ;          \
    } // 20MHz Version

/////////////////////////////////////////////////////////////////////////////
// local prototypes
/////////////////////////////////////////////////////////////////////////////
void LCD_Busy(void);

/////////////////////////////////////////////////////////////////////////////
// library variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////

// note: LCD_Init initializes the timer!
void LCD_Init(void)
{
    // if the timer is not enabled, enable it now
    if (!TSCR1_TEN)
        Timer_Init(20E6, Timer_Prescale_32, 0, Timer_Channel_6, Timer_PollingMode, Timer_Pin_Disco);

    PTH = 0x38; // function set = 8 bit i.f., 2 line, 5 * 7

    LCD_EDown;  // not enabled
    LCD_RWDown; // writing mode
    LCD_RSDown; // command mode

    DDRK |= 0x07; // set port K 0:2 bits as outputs

    // all pins are now low
    // e could have been running a little high
    // cause delay to allow LDC to get it together
    // spec says 40ms after Cvv > 2.7V, but usually needs longer, so we are giving it 100ms
    (void)Timer_Sleep(100);

    // strobe once then sleep
    LCD_EUp;
    LCD_EDown;
    (void)Timer_Sleep(15);

    // strobe again then sleep
    LCD_EUp;
    LCD_EDown;
    (void)Timer_Sleep(1);

    // now the busy flag is available
    // 3rd time, after busy has completed (init)
    LCD_Inst(0x38);

    LCD_Inst(0x0c); // Disp Constrol = Disp On, no cursor, no blink
    LCD_Inst(0x01); // Clear the display
    LCD_Inst(0x06); // Entry mode set = increment cursor, no display shift
}

// send a byte to the inst register of LCD on custom port
void LCD_Inst(unsigned char val)
{
    LCD_Busy(); // wait until the LCD is not busy

    LCD_RWDown; // set to writing mode
    LCD_RSDown; // set to command mode

    PTH = val; // present data on port H

    // strobe E
    LCD_EUp;
    LCD_EDown; // idle state
}

void LCD_Data(unsigned char val)
{
    LCD_Busy(); // wait until the LCD is not busy

    LCD_RWDown; // set to writing mode
    LCD_RSUp;   // set to command mode

    PTH = val; // present data on port H

    // strobe E
    LCD_EUp;
    LCD_EDown; // idle state
}

// set the cursor address
void LCD_Addr(unsigned char addr)
{
    addr |= 0x80; // add the command identifier bit
    LCD_Inst(addr);
}

// set the cursor address based on a given x and y coordinate (row and column)
void LCD_AddrXY(unsigned char ix, unsigned char iy)
{
    // if the coordinates are outside of the LCD bounds, display at 0
    if (ix > 19 || iy > 3)
        LCD_Addr(0);
    else
    {
        switch (iy)
        {
        case 0:
            LCD_Addr(ix);
            break;
        case 1:
            LCD_Addr(ix + 64);
            break;
        case 2:
            LCD_Addr(ix + 20);
            break;
        case 3:
            LCD_Addr(ix + 84);
            break;
        }
    }
}

// output the provided string to the LCD starting at the current position of the cursor
void LCD_String(char *straddr)
{
    // display each character of the string one at a time until the null reference is reached
    while (*straddr)
        LCD_Data(*(straddr++));
}

// will print the given string starting at the home position (0,0) and will print the string with proper wrapping
void LCD_WrappingString(char *straddr)
{
    uchar i;
    int loopEnd = strlen(straddr);
    if (loopEnd > 80)
        loopEnd = 80;

    for (i = 0; i < loopEnd; i++)
    {
        // depending on i, write set the address to display at the proper position/line
        if (i < 20)
            LCD_AddrXY(i, 0);
        else if (i < 40)
            LCD_AddrXY(i - 20, 1);
        else if (i < 60)
            LCD_AddrXY(i - 40, 2);
        else if (i < 80)
            LCD_AddrXY(i - 60, 3);

        // display the character
        LCD_Data(straddr[i]);
    }
}

// uses LCD_AddrXY and LCD_String to display a string at a specified starting location
void LCD_StringXY(unsigned char ix, unsigned char iy, char *straddr)
{
    LCD_AddrXY(ix, iy);
    LCD_String(straddr);
}

// uses LCD_StringXY to clear the provided line
void LCD_ClearLine(unsigned char iy)
{
    LCD_StringXY(0, iy, "                    ");
}

// set up the display control based on
void LCD_DispControl(unsigned char cursorOn, unsigned char blinkOn)
{
    byte command = 0x08 | 0x04;
    if (cursorOn)
        command |= 0x02;
    if (blinkOn)
        command |= 0x01;

    LCD_Inst(command);
}

// clear the LCD
void LCD_Clear(void)
{
    LCD_Inst(0x01); // command for clear
}

// return cursor to home
void LCD_Home(void)
{
    LCD_Inst(0x02); // command for home
}

void LCD_ShiftL(void)
{
    // turn on shifting
    LCD_Inst(0x04 | 0x02 | 0x01); // 0x04 is entry mode, 0x02 is left to right (increment), 0x01 is shift enabled

    // shift left
    LCD_Inst(0x10 | 0x08); // 0x10 is cursor/display shift, 0x08 is display shift
}
void LCD_ShiftR(void)
{
    // turn on shifting
    LCD_Inst(0x04 | 0x02 | 0x01); // 0x04 is entry mode, 0x02 is left to right (increment), 0x01 is shift enabled

    // shift right
    LCD_Inst(0x10 | 0x08 | 0x04); // 0x10 is cursor/display shift, 0x08 is display shift, 0x04 is shift to the right
}

void LCD_CGAddr(unsigned char addr);
void LCD_CGChar(unsigned char cgAddr, unsigned char *cgData);

/////////////////////////////////////////////////////////////////////////////
// hidden helpers (local to implementation)
/////////////////////////////////////////////////////////////////////////////

// wait for the LCD to not by busy (blocking)
// service function, private
void LCD_Busy(void)
{
    uchar inVal = 0;

    LCD_RSDown; // instruction
    LCD_RWUp;   // reading

    // repeat until device is not busy
    do
    {
        LCD_EUp;        // get the LCD's attention
        LCD_MicroDelay; // tested @ 20MHS, delay for tDDR NEEDED (data sheet says 360ns : P58 + P49)
        // @50ns per clock, and average 2.5 cycles per instruction, this is about 3 average assembly instruction
        // this is not a long delay, but it's enough to allow the LCD to read the instructions

        inVal = PTH; // status (d7) and address (d6:0)
        LCD_EDown;   // release device
    } while (inVal & 0x80);
}
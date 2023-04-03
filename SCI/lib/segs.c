// other includes, as *required* for this implementation
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "segs.h"

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
// macros
/////////////////////////////////////////////////////////////////////////////

// write data to the 7Segs
#define SEG_WStrobe \
  PORTA &= (~0x01); \
  PORTA |= 0x01;

// set the mode to command
#define SEG_MCommand \
  PORTA |= 0x02;

// set the mode to data
#define SEG_MData \
  PORTA &= (~0x02);

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////

// setup ports to speak with 7-segs
void Segs_Init(void)
{

  // ensure port pins are high (idle state)
  PORTA |= 0x03;

  // setup the data direction for the ports are outputs
  DDRA |= 0x03;
  DDRB = 0xFF;

  // clear any remaining display
  (void)Segs_ClearAll();
}

// control segs manually
// NOTE: DP bit works opposite to normal for Segs_Custom:
//  1 = ON, 0 = OFF   The method inverts the bit for easier customizing!!!
void Segs_Custom(unsigned char Addr, unsigned char Value)
{
  // trim address 3 bits
  Addr &= 0b111;

  // build the command
  Addr |= 0b01111000; // No Data Coming, Hex, No Decode, Normal Operation, Bank A

  // toggle the decimal bit
  Value ^= Segs_SegDec;

  // send the command
  PORTB = Addr;
  SEG_MCommand;
  SEG_WStrobe;

  // send the data
  PORTB = Value;
  SEG_MData;
  SEG_WStrobe;
}

// show HEX decoding (changes all of display to HEX, w/wo dp)
void Segs_Normal(unsigned char Addr, unsigned char Value, Segs_DPOption dp)
{
  // trim address 3 bits
  Addr &= 0b111;

  // trim value to 4 bits
  Value &= 0b1111;

  // build the command
  Addr |= 0b01011000; // No Data Coming, Hex, Decode, Normal Operation, Bank A

  // decimal point low (0) is on, not 1
  if (dp)
    Value &= ~(0x80);
  else
    Value |= 0x80;

  // send the command
  PORTB = Addr;
  SEG_MCommand;
  SEG_WStrobe;

  // send the data
  PORTB = Value;
  SEG_MData;
  SEG_WStrobe;
}

// deprecated after term 1202
// go code B (changes all of display to code b)
void Segs_CodeB(unsigned char Addr, unsigned char Value, Segs_DPOption dp)
{
  // trim address 3 bits
  Addr &= 0b111;

  // build the command
  Addr |= 0b00010000; // No Data Coming, Hex, Decode, Normal Operation, Bank B

  // decimal point low (0) is on, not 1
  if (dp)
    Value &= ~(0x80);
  else
    Value |= 0x80;

  // send the command
  PORTB = Addr;
  SEG_MCommand;
  SEG_WStrobe;

  // send the data
  PORTB = Value;
  SEG_MData;
  SEG_WStrobe;
}

// show a hex value on the upper or lower display
void Segs_16H(unsigned int Value, Segs_LineOption Line)
{

  // since we need to display from right to left, the initial position is 3
  // if we need to display on the bottom, Line will equal 1, so Line * 4 = 4
  //   adding 4 to the initial 3 will get us position 7, allowing us to display
  //   from position 7 backwards to position 4 aka the bottom line of the 7segs
  byte target = 3 + Line * 4;
  byte i; // index for loop

  // display each digit by right shifting (i*4) times
  // display the digit at location (target-i) where target is the last location in the line
  //   this is because we are displaying from right to left
  for (i = 0; i < 4; i++)
    Segs_Normal(target - i, (uchar)(Value >> (uchar)(i * 4)), Segs_DP_OFF);
}

// show a dec value on the first or second line of the 7-segs
void Segs_16D(unsigned int Value, Segs_LineOption Line)
{

  // since we need to display from right to left, the initial position is 3
  // if we need to display on the bottom, Line will equal 1, so Line * 4 = 4
  //   adding 4 to the initial 3 will get us position 7, allowing us to display
  //   from position 7 backwards to position 4 aka the bottom line of the 7segs
  byte target = 3 + Line * 4;

  byte i; // index for loop

  // display Err if value is greater than 9999, then return
  if (Value > 9999)
  {
    Segs_SayErr(Line);
    return;
  }

  // display each digit by finding (Value % 10) - essentially converting to BCD
  // display the digit at location (target-i) where target is the last location in the line
  //   this is because we are displaying from right to left
  // divide Value by 10 to prep for the next digit
  for (i = 0; i < 4; i++)
  {
    Segs_Normal(target - i, Value % 10, Segs_DP_OFF);
    Value /= 10;
  }
}

// show the 8-bit value starting on the digit as addr (0-6)
void Segs_8H(unsigned char Addr, unsigned char Value)
{
  // store a copy of value to keep the original value intact if we end up constricting the value to 0xFF (see below)
  uint valueCopy = Value;

  uchar Addr2 = Addr + 1;
  if (Addr2 > 7)
    Addr2 = 0;

  // cap valueCopy at 0xFF
  if (valueCopy > 0xFF)
    valueCopy = 0xFF;

  Segs_Normal(Addr, (uchar)(valueCopy >> (uchar)4), Segs_DP_OFF); // display first digit
  Segs_Normal(Addr2, (uchar)valueCopy, Segs_DP_OFF);              // display second digit
}

// clear the display
void Segs_ClearAll(void)
{
  byte target;
  for (target = 0; target < 8; target++)
    Segs_Clear(target);
}

// clear the positoon
void Segs_Clear(unsigned char Addr)
{
  Segs_Custom(Addr, 0);
}

// clear the specified line
void Segs_ClearLine(Segs_LineOption Line)
{
  byte i; // index for loop

  // the initial postion is depending on the line (0 for top line, 4 for bottom line), so i = Line*4
  // we loop as long as i is less than the end position of the line (3 for top, 7 for bottom), so the calculation is (4 + Line*4)
  for (i = Line * 4; i < 4 + Line * 4; i++)
    Segs_Clear(i); // clear the position
}

// say Err on the appropriate line
void Segs_SayErr(Segs_LineOption Line)
{
  // target stores the location to start writing the Err
  // if bottom Line is chosen, Line = 1, so (Line * 4) = 4
  // this sets the initial positon to either 0 or 4 depending on the chosen line
  byte target = Line * 4;

  // write each character, incrementing target after doing so
  Segs_Normal(target++, 0xE, Segs_DP_OFF); // display E
  Segs_Custom(target++, 0b11101010);       // display r1
  Segs_Custom(target++, 0b11101010);       // display r2
}

// deprecated after term 1202
// say 'help' as best the 7-segs can show
void Segs_SayHelp(void)
{
  Segs_CodeB(1, 0xB, Segs_DP_OFF); // Display H
  Segs_CodeB(0, 0xC, Segs_DP_OFF); // Display E
  Segs_CodeB(2, 0xD, Segs_DP_OFF); // Display L
  Segs_CodeB(3, 0xE, Segs_DP_OFF); // Display P
}
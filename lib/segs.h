/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Simon Walker
// Details:       library for ICM7212A 7-segment display
// Revision History :
// Created : *unknown*
// May 9th 2014   - Port/Fixup to C library
// June 2018      - Cleanup
// Dec 07 2020    - simplified helpers to be #defines
//                - tested library for 20MHz operation
// Dec    2020    - changed name, simplified naming
//                - made arg type same, simplified
// Mar 13 2021    - added line specific Err function
//                - deprecate CodeB use and HELP function after term 1202
//                - added enum for line targeting (16H, 16D, Err)

// device connection
// PA0 - W*
// PA1 - MODE
// PB0-PB7 - IDO-ID7

// note - decode applies to entire device, not per segment
//  switching decode will flip the whole device contents
// note - even in no decode commands, the device will
//  briefly change to whatever coding is present in ID6
//  so this bit, while described as 'don't care' should
//  probably be kept high to maintain HEX decoding
//  -- only use code b bit for help/help function

// muxled - for custom decode:
//   40
// 02  20
//   04
// 08  10
//   01   80(off)
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

// for functions that want to add/kill the decimal point
typedef enum Segs_DPOption
{
  Segs_DP_OFF,
  Segs_DP_ON
} Segs_DPOption;

// for functions that specify the upper or lower full display
typedef enum Segs_LineOption
{
  Segs_LineTop,
  Segs_LineBottom
} Segs_LineOption;

// for targetting individual segments on a single display
//    can use bitwise OR ( | ) to combine these if multiple are wanted at once
// the DP is 128 instead of 0 because my Segs_Custom method inverts the DP bit for us
//    this allows us to send a 1 in the DP bit to turn it on and allows us to use
//    the bitwise OR on the DP without having to take an extra step to fix the inversion
typedef enum Segs_Segments
{
  Segs_SegD = 1,
  Segs_SegF = 2,
  Segs_SegG = 4,
  Segs_SegE = 8,
  Segs_SegC = 16,
  Segs_SegB = 32,
  Segs_SegA = 64,
  Segs_SegDec = 128
} Segs_Segments;

// setup ports to speak with 7-segs
void Segs_Init(void);

// control segs manually
void Segs_Custom(unsigned char Addr, unsigned char Value);

// show HEX decoding (changes all of display to HEX, w/wo dp)
void Segs_Normal(unsigned char Addr, unsigned char Value, Segs_DPOption dp);

// deprecated after term 1202
// go code B (changes all of display to code b)
void Segs_CodeB(unsigned char Addr, unsigned char Value, Segs_DPOption dp);

// show a 16-bit value on the upper or lower display
void Segs_16H(unsigned int Value, Segs_LineOption Line);

// show a decimal value on the first or second line of the 7-segs
void Segs_16D(unsigned int Value, Segs_LineOption Line);

// show the 8-bit value starting on the digit as addr (0-6)
void Segs_8H(unsigned char Addr, unsigned char Value);

// clear the display
void Segs_ClearAll(void);

// clear the postion
void Segs_Clear(unsigned char Addr);

//clear the specified line
void Segs_ClearLine(Segs_LineOption Line);

// say Err on the appropriate line
void Segs_SayErr(Segs_LineOption Line);

// deprecated after term 1202
// say 'help' as best the 7-segs can show
void Segs_SayHelp(void);
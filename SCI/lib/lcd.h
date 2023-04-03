/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Simon Walker
// Details:       LCD Library
// Revision History :
// Created : Unknown
//  Dec 07 2020 - Modified Documentation
//  Dec    2020 - Modified names, modified to use timer for delays
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////
// 8-Bit interface init on LCD
// LCD is wired to PTH for data, PK0:2 for control :
// 2     1     0     PTK 
// A     R/W*  E     LCD 
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// LCD Address Scheme (HEX):
// 00 01 ... 12 13
// 40 41 ... 52 53
// 14 15 ... 26 27
// 54 55 ... 66 67
////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

// note: LCD_Init initializes the timer!
void LCD_Init (void);
void LCD_Inst (unsigned char val);
void LCD_Data (unsigned char val);
void LCD_Addr (unsigned char addr);
void LCD_AddrXY (unsigned char ix, unsigned char iy);
void LCD_String (char * straddr);
void LCD_WrappingString(char *straddr);
void LCD_StringXY (unsigned char ix, unsigned char iy, char * straddr);
void LCD_ClearLine(unsigned char iy);

void LCD_DispControl (unsigned char cursorOn, unsigned char blinkOn);
void LCD_Clear (void);
void LCD_Home (void);
void LCD_ShiftL (void);
void LCD_ShiftR (void);

//void LCD_CGAddr (unsigned char addr);
//void LCD_CGChar (unsigned char cgAddr, unsigned char * cgData);


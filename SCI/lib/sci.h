// SCI library
// Last Modified : June 08 2015
// Revision History:
// May 13th 2014 - Initial Port to C - Simon Walker
// June 8th 2015 - Genric BAUD init added, others removed except for IRDA
// May 23rd 2019 - cleanup, real use of derivative file definitions
// Mar 15th 2022 - cleanup, added interrupt (RDRF only) to init
//               - added ISR template to header
// Nov. 4, 2022  - Added enums for BaudRate and iRDRF_interrupts - Andrew Belter
// Nov. 14, 2022 - Added consts for common escape sequences - Andrew Belter

/*
interrupt VectorNumber_Vsci0 void ISR_SCI0(void)
{
  // only source of interrupt currently is RDRF, so only managing that
  //BRead reads the RDRF flag and then read data, so the flag is auto-cleared
  unsigned char input = SCI0_BRead();
}
*/

typedef enum SCI_BaudRate
{
  BaudRate_75,
  BaudRate_110,
  BaudRate_300,
  BaudRate_600,
  BaudRate_1200,
  BaudRate_2400,
  BaudRate_4800,
  BaudRate_9600,
  BaudRate_14400,
  BaudRate_19200,
  BaudRate_38400,
  BaudRate_57600,
  BaudRate_115200
} SCI_BaudRate;

typedef enum SCI_RDRF_InterruptMode
{
  SCI_RDRF_InterruptOff,
  SCI_RDRF_InterruptOn
} SCI_RDRF_InterruptMode;

// SCI 0 - normal mode ********************************
// initialized SCI0 to the given baud rate
void SCI0_Init(unsigned long ulBusClock, SCI_BaudRate baudRate, SCI_RDRF_InterruptMode interruptMode);

// read a byte, non-blocking
// returns 0 if byte read, -1 if not
// uses 0 for success because then we can have multiple error codes
// returning true means an error and we can then check the error code to find out what
int SCI0_Read(unsigned char *pData);

// blocking byte read
// waits for a byte to arrive and returns it
unsigned char SCI0_BRead(void);

// send a byte over SCI
// will block until able to send
void SCI0_TxByte_Block(unsigned char data);

// send a byte over SCI
// will return an error code if the char did not send
// returns 0 if the char was sent to SCI0DRL
int SCI0_TxByte_NonBlock(unsigned char data);

// send a null-terminated string over SCI
// can block code for a long time if string is long
void SCI0_TxStr(char const *strAddr);

// receive a string from the SCI
// up to buffer size-1 (string always NULL terminated)
// number of characters is BufferSize minus one for null
// once user enters the max characters, null terminate and return
// if user enters 'enter ('\r')' before-hand, return with current entry (null terminated)
// echo valid characters (non-enter) back to the terminal
// return -1 on any error, otherwise string length
int SCI0_RxStr(char *const pTarget, int BufferSize);
// SCI 0 - normal mode ********************************

// these items are optional, for other SCI ports
// SCI 1 - IRDA mode **********************************
void SCI1_Init(void);
void SCI1_TxByte(unsigned char data);
int SCI1_Read(unsigned char *pData);
// SCI 1 - IRDA mode **********************************

// SCI 2 - normal mode - shared with port J interrupts
unsigned long SCI2_Init(unsigned long iBaudRate);
int SCI2_Read(unsigned char *pData);
// SCI 2 - normal mode - shared with port J interrupts
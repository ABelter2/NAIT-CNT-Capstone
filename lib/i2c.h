/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     8MHz or 20 MHz (Requires Active PLL)
// Author:        Simon Walker
// Details:       Base library for abstract I2C operations.
// Revision History
//      Created:  Unknown, see implementation comments
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

// the number of times to loop before calling it blocking
#define I2C_SPIN_COUNTMAX 10000

// simple true/false flags to make code easier to read
#define I2C_STOP 1
#define I2C_NOSTOP 0
#define I2C_WRITE 0  
#define I2C_READ 1
#define I2C_ACK 1
#define I2C_NACK 0
#define I2C_WAIT 1
#define I2C_NOWAIT 0

// enum for desired I2C bus rate
typedef enum
{
  I2CBus100,  // I2C bus @ 100 kHz
  I2CBus400   // I2C bus @ 400 kHz
} I2C_BusRate;

// enum for current micro bus rate
typedef enum
{
  I2CMicro8MHz,  // micro @ 8MHz bus
  I2CMicro20MHz   // micro @ 20MHz bus
} I2C_MicroBusRate;

// generic I2C functions *****************************
// init bus 
void I2C_Init0 (I2C_MicroBusRate eBus, I2C_BusRate eRate, int IntsOn); 

// wait for bus to be free (pseudo-blocking)
int I2C_WaitForBus ();

// send target address, indicate R/W
//  and flag for bus ready (bus is idle for starts)
//  bus is hot for restarts (no wait)
int I2C_SendAddressRW (unsigned char address, unsigned char IsRead, unsigned char WaitForBus);

// write a byte, with or without stop
//  no stop, generally when more bytes are on the way
int I2C_WriteByte (unsigned char val, unsigned char IssueStop);

// one-stop shopping for read
// give target for output byte, then provide:
// I2C_ACKON (true), I2C_ACKOFF (false) for rx ack
// I2C_STOP (true), I2C_NOSTOP (false) for stop
int I2C_RXByte (unsigned char * buff, unsigned char IssueAck, unsigned char IssueStop);

// issue a restart (some devices use this)
void I2C_IssueRestart (void);
// ***************************************************

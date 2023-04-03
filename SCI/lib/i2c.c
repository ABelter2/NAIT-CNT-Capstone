#include <hidef.h>
#include "derivative.h"
#include "i2c.h"

///////////////////////////////////////////////////////////////////////////////////////////
// I2C - Library for operating the I2C Devices
// 
// Last Modified November 21 2014 - Unified library for all I2C devices (single init)
//               December 02 2015 - Modified front end for EEPROM, added seq read/write
//               Feb      01 2016 - unified lib and added DAC functions
//               Dec         2018 - complete unified front end for I2C + device functs 
//               Nov      13 2019 - updated to use benefit of derivative defs (vs.code)
//               Nov      30 2020 - updated to include 20MHz/8MHz clock parameterization 
///////////////////////////////////////////////////////////////////////////////////////////

// general theory for all I2C devices
// GENERAL:
// if a device is present it will respond to address announcement, exception being
//  the EEPROM during a write cycle

// READ:
// send address of device with read intent
// set ack on all but last byte to read (if multi-byte read supported by device)
// set nack on last byte to read, and issue stop (some devices don't require nack
//  if the byte length is fixed)
// stop required to free bus

// WRITE:
// send address of device with write intent
// write data with nostop until last byte (stop) (if multi-byte write supported)
// stop required to free bus

// RESTART:
// used to send a register address, then read from device
// send address of device with write intent
// write as above, but no stop is issued
// issue a restart (with function)
// send address of device with read intent, but don't wait for bus to be free
//  (we have it in use, it won't become free)
// read as above


// general init
void I2C_Init0 (I2C_MicroBusRate eBus, I2C_BusRate eRate, int IntsOn)
{
  IIC0_IBCR_IBEN = 0; // kill the bus (attempt to tear-down previous activity)

  // test and see if the data line of the bus is being held low, if so, attempt to toggle the clock line a bit
  //  this should 'shake-off' any lingering conversations that an external device will be waiting on
  if (!(PTJ & 0x40))
  {
    DDRJ |= 0x80;// clock out (safe to drive clock, not data line, as clock is always driven by micro)
    {
      int q;
      for (q = 0; q < 12; q++) // gonna do 12 just to be sure, but 9 would run out a byte with ack/nack
      {
        PTJ |= 0x80;
        {
          uint i = 0;
          while (i < 300) // adjusted for 20MHz bus
            ++i;
        }
        PTJ &= (~0x80);
        {
          uint i = 0;
          while (i < 300) // adjusted for 20MHz bus
            ++i;
        }
      }
    }

    DDRJ &= (~0x80); // set back to an input before module takes over
  } 

  // bus clearing delay, not sure if required but generally a short delay to init not a big deal
  {
    unsigned long i = 0;
    while (i < 60000)
      ++i;
  }

  if (eBus == I2CMicro8MHz)
  {
    if (eRate == I2CBus400)
    {
      IIC0_IBFD = 0x00;     // 400kHz freq (8MHz/20)
                            // SDA Hold = 7 clocks
                            // SCL Hold (Start) = 6
                            // SCL Hold (Stop) = 11
    }
    else // 100 KHz I2C bus
    {
      IIC0_IBFD = 0x47;     // 100KHz freq (8MHz/80) 
                            // SDA Hold = 20 clocks
                            // SCL Hold (Start) = 32
                            // SCL Hold (Stop) = 42
    }
  }
  else // 20MHz bus rate
  {
    if (eRate == I2CBus400)
    {
      IIC0_IBFD = 0x43;     // 384.615KHz (20MHz/52 <= close as possible) // measured at 370KHz on SCL
                            // SDA Hold = 16 clocks
                            // SCL Hold (Start) = 18
                            // SCL Hold (Stop) = 28
    }
    else // 100 KHz I2C bus
    {
      IIC0_IBFD = 0x56;     // 100KHz (20MHz/208 <= close as possible) // calculated at 96.154KHz
                            // SDA Hold = 42 clocks
                            // SCL Hold (Start) 92
                            // SCL Hold (Stop) 106                                 
    }
  }
                      
  // it's not necessary to set our slave address unless
  //  we are operating as a slave (we are not usually)
  
  IIC0_IBCR |= 0x80;    // enable the bus (must be done before)
                        //  any other mods to IBCR
  if (IntsOn)
  {
    IIC0_IBCR_IBIE = 1;  // want interrupts (only for notification that arb is lost)
    IIC0_IBSR = IIC0_IBSR_IBIF;
  }
  else
  {
    IIC0_IBCR_IBIE = 0;
  }
  
  IIC0_IBCR_IBSWAI = 0; // keep running in WAI

  // testing - can a reset of the device bring the device back?
  // downing the module and restarting does not
  // has been observed to be stuck with D low with C high
  // sending an arbirtary stop here if IBB is 1 and/or IBAL is 1
  // might release the bus, but not sure if the u will detect this state
  // bus goes weird if interrupted during a transfer
  //if ((IIC0_IBSR & IIC0_IBSR_IBB_MASK) || (IIC0_IBSR & IIC0_IBSR_IBAL_MASK))
  // attempt to become master then slave
  // this will work or cause loss of arbitration, either way, the bus gets freed
  
  // nope, none of this helped; as a master we literally can't touch the bus
  //  if it appears non-idle
  /*
  if (IIC0_IBSR & IIC0_IBSR_IBAL_MASK)
    IIC0_IBSR |= IIC0_IBSR & IIC0_IBSR_IBAL_MASK;
  
  IIC0_IBCR |= IIC0_IBCR_MS_SL_MASK; 

  // inject ~5ms delay
  asm ldx #13107;
  asm dbne X,*;

  // if bus was busy we will get arb lost, otherwise ok, either way free bus
  IIC0_IBCR &= (~(byte)IIC0_IBCR_MS_SL_MASK);
  */
}

/////////////////////////////////////////////////////////////////////
// generic I2C routines to be used by device specific implementations
/////////////////////////////////////////////////////////////////////

// wait for the bus to be not busy (non-blocking)
// this should always be the case in a single master situation
// *** unless *** we are attempting a restart, in that case
//  we would NOT look for the bus to be free (as *we* are using it)
int I2C_WaitForBus ()
{
  int iSpinCount = 0;
  
  while (++iSpinCount < I2C_SPIN_COUNTMAX)
    if (!(IIC0_IBSR_IBB))  // IBB must be zero (bus free)
      return 0; 
  
  return -1;
}

// write the become master bits and write an address out with R/W intention
// indicate device address, intent to read or write, and wait for bus flag
//  we would not wait for the bus if this is done after a restart condition
int I2C_SendAddressRW (unsigned char address, unsigned char IsRead, unsigned char WaitForBus)
{       
    // returns -1 on no bus (if requested) or failed to ACK
    
    // try to get the bus if not a restart
    //  if bus is 'hot' it's busy, so don't try to wait for it
    if (WaitForBus)
    {      
      if (I2C_WaitForBus())
        return -1;
    }
    
    // got bus
    IIC0_IBCR |= IIC0_IBCR_MS_SL_MASK | IIC0_IBCR_TX_RX_MASK; // master mode, transmitting (start)    
    if (!IsRead)
      IIC0_IBDR = address & 0xFE; // send slave address w/write
    else
      IIC0_IBDR = address | 0x01; // send slave address w/read
      
    // 9.7.1.2 wait for IBB to set (may not be necessary)
    //while (!(IIC0_IBSR_IBB))
    //  ;
    
    // 9.7.1.3 -> monitor IBIF, not TCF
    while (!(IIC0_IBSR_IBIF))
      ;         

    // clear interrupt/flag from send address operation (req'd)
    IIC0_IBSR_IBIF = 1;
    
    // test for ack on command
    if (!(IIC0_IBSR_RXAK))
    {
      // got ack, good to go (all devices ack on the address announcement)
      //  if no ack, prolly means no device at that address to respond
      return 0;  
    }
    else
    {
      // no ack, issue stop                           
      IIC0_IBCR_MS_SL = 0;       
    }
    
    return -1; // bad condition
}

int I2C_WriteByte (unsigned char val, unsigned char IssueStop)
{
  int iSpinCount = 0;

  // send byte  
  IIC0_IBDR = val;                
  
  // wait for interrupt flag w/bail on timeout
  while ( (!(IIC0_IBSR_IBIF)) && (++iSpinCount < I2C_SPIN_COUNTMAX) )
    ;
  
  // did we timeout?
  if (iSpinCount >= I2C_SPIN_COUNTMAX)
    return -1;
  
  // no timeout, got flag, issue stop and clear flag (must be in this order)
  if (IssueStop)
    IIC0_IBCR_MS_SL = 0;                   
  IIC0_IBSR_IBIF = 1; // (not necessary?) 9.7.1.4             
  
  // no error
  return 0;
}

// done with bus "hot", assumes this is the case, anyway
void I2C_IssueRestart (void)
{
  // issue a restart
  IIC0_IBCR_RSTA = 1;           
}

// one-stop shopping for read
// give target for output byte, then provide
// I2C_ACKON (true), I2C_ACKOFF (false) for rx ack
// I2C_STOP (true), I2C_NOSTOP (false) for stop
int I2C_RXByte (unsigned char * buff, unsigned char IssueAck, unsigned char IssueStop)
{
  byte junk; // necessary for rx starting dummy read
  
  // make us a receiver
  IIC0_IBCR_TX_RX = 0;

  // ACK setup (reverse from expected logic) (9.3.2.3)
  if (IssueAck)
    IIC0_IBCR_TXAK = 0;  // on
  else
    IIC0_IBCR_TXAK = 1;
         
  // start read process         
  junk = IIC0_IBDR;           

  while (!(IIC0_IBSR_IBIF))
    ;// wait for interrupt flag (read done) 
  
  // issue stop (done while flag set)
  if (IssueStop)
    IIC0_IBCR_MS_SL = 0; 
  
  // clear flag (must be done in this order)
  IIC0_IBSR_IBIF = 1;          

  // get the byte actual  
  *buff = IIC0_IBDR;      
  
  return 0; // good condition  
}


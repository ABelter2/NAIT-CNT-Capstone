/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Created:       Dec. 6, 2022
// Details:       Library for using LTC2633 DAC
// Revision History
//      each revision will have a date + desc. of changes
/////////////////////////////////////////////////////////////////////////////

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "LTC2633.h"

// other includes, as *required* for this implementation
#include "i2c.h"

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
int LTC2633_WriteChan(unsigned int Value, LTC2633_CHAN_SELECT chan)
{
    // address the device
    if (I2C_SendAddressRW(LTC2633ADDR, I2C_WRITE, I2C_WAIT))
        return -1;
    // send command (write chan, power up all)
    if (chan == LTC2633_CHAN_A)
        (void)I2C_WriteByte(0b00100000, I2C_NOSTOP); // P18, datasheet
    else if (chan == LTC2633_CHAN_B)
        (void)I2C_WriteByte(0b00100001, I2C_NOSTOP); // P18, datasheet
    else                                             // assume all channels
        (void)I2C_WriteByte(0b00101111, I2C_NOSTOP); // P18, datasheet

    // send msb data (data is 12 bits, oddly, left aligned, P18, datasheet)
    (void)I2C_WriteByte((unsigned char)(Value >> 4), I2C_NOSTOP); // 0x0123 becomes 0x12

    // send lsb data
    (void)I2C_WriteByte((unsigned char)(Value << 4), I2C_STOP); // 0x0123 becomes 0x30

    return 0; // good condition
}

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

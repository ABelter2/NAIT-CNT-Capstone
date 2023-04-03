/////////////////////////////////////////////
// LTC2633 - Dual 12-bit I2C DAC Library
// 7-bit device address 0x10, but 0x20 as command
// this device can go to 400kHz
/////////////////////////////////////////////

// this is a 12-bit, 4.096V (1mV/step), two channel DAC
// write time per value is ~118.5us @ 400KHz bus (measured)
// general form (write only):
// <ADDR><C3:C0,A3:A0><D11:D4[DAC Value]><D3:D0[0]>

// C3 C2 C1 C0
//  0  0  0  0 - write to register n
//  0  0  0  1 - update (power up) DAC, register n
//  0  0  1  0 - write to input register n, update (power up) all
//  0  0  1  1 - write to and update (power up) DAC register n
//  0  1  0  0 - power down n
//  0  1  0  1 - power down chip
//  0  1  1  0 - select (power up) internal reference
//  0  1  1  1 - select external reference
//  1  1  1  1 - no op
// NOTE: left-aligned data

// A3 A2 A1 A0
//  0  0  0  0 - DAC A
//  0  0  0  1 - DAC B
//  1  1  1  1 - all DACs

// command form (8-bit) of address is 0x20 (0x10 << 1)
#define LTC2633ADDR 0x20

// channel decode tags for LTC write function
typedef enum LTC2633_CHAN_SELECT
{
  LTC2633_CHAN_A,
  LTC2633_CHAN_B,
  LTC2633_CHAN_BOTH
} LTC2633_CHAN_SELECT;

// write a channel
int LTC2633_WriteChan (unsigned int Value, LTC2633_CHAN_SELECT chan);
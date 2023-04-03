/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Details:       Bring up the desired PWM channel to produce a square wave with a desired duty cycle
// Created: Oct. 2022
// Revision History
//      Oct. 21, 2022:  Created prototypes and enums for PWM operations
//                      Implemented function to initialize the PWM in 8-bit mode
//      Oct. 22, 2022:  Implemented functions to set period and duty for 8-bit mode
//                      Implemented functions to select Clock and set polarity
//      Oct. 25, 2022:  Implemented functions to set clock prescales and to enable/disable a channel manually
//                      Implemented functions to set Period/Duty for 16-bit
//                      Implemented functions to initialize a 16-bit PWM channel
/////////////////////////////////////////////////////////////////////////////

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "pulse.h"

// other includes, as *required* for this implementation

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
void Pulse_Init_8Bit(
    Pulse_Channel channel,
    Pulse_PrescaleStage1 prescaleStage1,
    unsigned char prescaleStage2,
    Pulse_Polarity polarity,
    unsigned char period,
    unsigned char duty)
{
    // set the prescales for the clock
    Pulse_SetClockPrescales(channel, prescaleStage1, prescaleStage2);

    Pulse_SetPeriod_8Bit(channel, period);      // set the period (8.3.2.13)
    Pulse_SetDuty_8Bit(channel, duty);          // set the duty (8.3.2.14)
    Pulse_SelectClock(channel, prescaleStage2); // select which clock the channel will use (8.3.2.3)
    // for selecting clock: if prescaleStage2 = 0, use ClockA/B, otherwise use ClockSA/SB

    // set the polarity for the channel (8.3.2.2)
    Pulse_SetPolarity(channel, polarity);

    // enable the channel (8.3.2.1)
    Pulse_ChannelControl(channel, Pulse_ControlEnabled);
}

void Pulse_Init_16Bit(
    Pulse_Channel channel,
    Pulse_PrescaleStage1 prescaleStage1,
    unsigned char prescaleStage2,
    Pulse_Polarity polarity,
    unsigned int period,
    unsigned int duty)
{
    // ensure channel is valid for 16-bit mode
    // if it is not, change channel to its paired channel for 16-bit mode
    switch (channel)
    {
    case Pulse_Channel0:
        channel = Pulse_Channel1;
        break;
    case Pulse_Channel2:
        channel = Pulse_Channel3;
        break;
    case Pulse_Channel4:
        channel = Pulse_Channel5;
        break;
    case Pulse_Channel6:
        channel = Pulse_Channel7;
        break;
    }

    // set the prescales for the clock
    Pulse_SetClockPrescales(channel, prescaleStage1, prescaleStage2);

    Pulse_SetPeriod_16Bit(channel, period);     // set the period (8.3.2.13)
    Pulse_SetDuty_16Bit(channel, duty);         // set the duty (8.3.2.14)
    Pulse_SelectClock(channel, prescaleStage2); // select which clock the channel will use (8.3.2.3)
    // for selecting clock: if prescaleStage2 = 0, use ClockA/B, otherwise use ClockSA/SB

    // set the polarity for the channel (8.3.2.2)
    Pulse_SetPolarity(channel, polarity);

    // set as a 16-bit channel by concatenating channels (8.3.2.6)
    switch (channel)
    {
    case Pulse_Channel1:
        PWMCTL_CON01 = 1; // concatenate channels 0 and 1
        break;
    case Pulse_Channel3:
        PWMCTL_CON23 = 1; // concatenate channels 2 and 3
        break;
    case Pulse_Channel5:
        PWMCTL_CON45 = 1; // concatenate channels 4 and 5
        break;
    case Pulse_Channel7:
        PWMCTL_CON67 = 1; // concatenate channels 6 and 7
        break;
    }

    // enable the channel (8.3.2.1)
    Pulse_ChannelControl(channel, Pulse_ControlEnabled);
}

void Pulse_SetClockPrescales(Pulse_Channel channel, Pulse_PrescaleStage1 prescaleStage1, unsigned char prescaleStage2)
{
    // set the clock values
    if (channel & Pulse_GroupA) // set the values for Group A
    {
        // set Clock A to divide by prescaleStage1 (8.3.2.4)
        PWMPRCLK &= 0b11111000;     // clear the old Group A Prescale
        PWMPRCLK |= prescaleStage1; // set the new Group A Prescale

        // set Clock SA to divide by prescaleStage2 * 2 (8.3.2.9)
        PWMSCLA = prescaleStage2; // automatically multiplies by 2
    }
    else // set the values for Group B
    {
        // set Clock B to divide by prescaleStage1 (8.3.2.4)
        PWMPRCLK &= 0b10001111;            // clear the old Group B Prescale
        PWMPRCLK |= (prescaleStage1 << 4); // set the new Group B Prescale
        // NOTE: The PrescalerStage1 value must be bit shifted because
        //       both Group A and B share a register in the format 0bXBBBXAAA

        // set Clock SB to divide by prescaleStage2 * 2 (8.3.2.10)
        PWMSCLB = prescaleStage2; // automatically multiplies by 2
    }
}

void Pulse_SetPeriod_8Bit(Pulse_Channel channel, unsigned char period)
{
    // set the period value on the given channel
    switch (channel)
    {
    case Pulse_Channel0:
        PWMPER0 = period;
        break;
    case Pulse_Channel1:
        PWMPER1 = period;
        break;
    case Pulse_Channel2:
        PWMPER2 = period;
        break;
    case Pulse_Channel3:
        PWMPER3 = period;
        break;
    case Pulse_Channel4:
        PWMPER4 = period;
        break;
    case Pulse_Channel5:
        PWMPER5 = period;
        break;
    case Pulse_Channel6:
        PWMPER6 = period;
        break;
    case Pulse_Channel7:
        PWMPER7 = period;
        break;
    }
}

void Pulse_SetPeriod_16Bit(Pulse_Channel channel, unsigned int period)
{
    // set the period value on the given channel
    switch (channel)
    {
    case Pulse_Channel1:
        PWMPER01 = period;
        break;
    case Pulse_Channel3:
        PWMPER23 = period;
        break;
    case Pulse_Channel5:
        PWMPER45 = period;
        break;
    case Pulse_Channel7:
        PWMPER67 = period;
        break;
    }
}

void Pulse_SetDuty_8Bit(Pulse_Channel channel, unsigned char duty)
{
    // set the duty value on the given channel
    // if the duty value > period value, the duty maxes at 100% so no sanitization is needed
    switch (channel)
    {
    case Pulse_Channel0:
        PWMDTY0 = duty;
        break;
    case Pulse_Channel1:
        PWMDTY1 = duty;
        break;
    case Pulse_Channel2:
        PWMDTY2 = duty;
        break;
    case Pulse_Channel3:
        PWMDTY3 = duty;
        break;
    case Pulse_Channel4:
        PWMDTY4 = duty;
        break;
    case Pulse_Channel5:
        PWMDTY5 = duty;
        break;
    case Pulse_Channel6:
        PWMDTY6 = duty;
        break;
    case Pulse_Channel7:
        PWMDTY7 = duty;
        break;
    }
}

void Pulse_SetDuty_16Bit(Pulse_Channel channel, unsigned int duty)
{
    // set the duty value on the given channel
    // if the duty value > period value, the duty maxes at 100% so no sanitization is needed
    switch (channel)
    {
    case Pulse_Channel1:
        PWMDTY01 = duty;
        break;
    case Pulse_Channel3:
        PWMDTY23 = duty;
        break;
    case Pulse_Channel5:
        PWMDTY45 = duty;
        break;
    case Pulse_Channel7:
        PWMDTY67 = duty;
        break;
    }
}

void Pulse_SetPolarity(Pulse_Channel channel, Pulse_Polarity polarity)
{
    // set the polarity for the channel (8.3.2.2)
    // positive polarity = 1, negative polarity = 0
    switch (polarity)
    {
    case Pulse_PolatityPositive:
        PWMPOL |= channel; // set the polarity bit for the channel
        break;
    case Pulse_PolatityNegative:
        PWMPOL &= ~channel; // clear the polarity bit for the channel
        break;
    }
}

void Pulse_SelectClock(Pulse_Channel channel, unsigned char useStage2Clock)
{
    // select which clock the channel will use (8.3.2.3)
    switch (channel)
    {
    case Pulse_Channel0:
        PWMCLK_PCLK0 = useStage2Clock ? 1 : 0; // use Clock SA is useStage2Clock is true
        break;
    case Pulse_Channel1:
        PWMCLK_PCLK1 = useStage2Clock ? 1 : 0; // use Clock SA is useStage2Clock is true
        break;
    case Pulse_Channel2:
        PWMCLK_PCLK2 = useStage2Clock ? 1 : 0; // use Clock SB is useStage2Clock is true
        break;
    case Pulse_Channel3:
        PWMCLK_PCLK3 = useStage2Clock ? 1 : 0; // use Clock SB is useStage2Clock is true
        break;
    case Pulse_Channel4:
        PWMCLK_PCLK4 = useStage2Clock ? 1 : 0; // use Clock SA is useStage2Clock is true
        break;
    case Pulse_Channel5:
        PWMCLK_PCLK5 = useStage2Clock ? 1 : 0; // use Clock SA is useStage2Clock is true
        break;
    case Pulse_Channel6:
        PWMCLK_PCLK6 = useStage2Clock ? 1 : 0; // use Clock SB is useStage2Clock is true
        break;
    case Pulse_Channel7:
        PWMCLK_PCLK7 = useStage2Clock ? 1 : 0; // use Clock SB is useStage2Clock is true
        break;
    }
}

void Pulse_ChannelControl(Pulse_Channel channel, Pulse_Control control)
{
    // enable/disable the channel (8.3.2.1)
    switch (control)
    {
    case Pulse_ControlDisabled:
        PWME &= ~channel; // disable the channel
        break;
    case Pulse_ControlEnabled:
        PWME |= channel; // enable the channel
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////
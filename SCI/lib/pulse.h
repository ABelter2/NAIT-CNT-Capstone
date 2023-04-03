/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Details:       Bring up the desired PWM channel to produce a square wave with a desired duty cycle
// Created: Oct. 2022
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////
/*
Channel 0: Blue LED in RGB LED
Channel 1: Green LED in RGB LED
Channel 2: Not assigned
Channel 3: LCD Backlight
Channel 4: Red LED in RGB LED
Channel 5: Not assigned
Channel 6: Speaker
Channel 7: Not assigned
*/
typedef enum Pulse_Channel
{
    Pulse_Channel0 = 0b00000001, // Blue LED in RGB LED
    Pulse_Channel1 = 0b00000010, // Green LED in RGB LED
    Pulse_Channel2 = 0b00000100, // Not assigned
    Pulse_Channel3 = 0b00001000, // LCD Backlight
    Pulse_Channel4 = 0b00010000, // Red LED in RGB LED
    Pulse_Channel5 = 0b00100000, // Not assigned
    Pulse_Channel6 = 0b01000000, // Speaker
    Pulse_Channel7 = 0b10000000  // Not assigned
} Pulse_Channel;

typedef enum Pulse_Control
{
    Pulse_ControlDisabled,
    Pulse_ControlEnabled
} Pulse_Control;

typedef enum Pulse_Group
{
    Pulse_GroupA = 0b00110011,
    Pulse_GroupB = 0b11001100
} Pulse_Group;

typedef enum Pulse_Polarity
{
    Pulse_PolatityNegative = 0,
    Pulse_PolatityPositive = 1
} Pulse_Polarity;

typedef enum Pulse_PrescaleStage1
{
    Pulse_PrescaleStage1_1 = 0,
    Pulse_PrescaleStage1_2 = 1,
    Pulse_PrescaleStage1_4 = 2,
    Pulse_PrescaleStage1_8 = 3,
    Pulse_PrescaleStage1_16 = 4,
    Pulse_PrescaleStage1_32 = 5,
    Pulse_PrescaleStage1_64 = 6,
    Pulse_PrescaleStage1_128 = 7
} Pulse_PrescaleStage1;

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////
/*
    Initializes the PWM Module to product a square wave according to the parameters given in 8-bit mode.

    Channel = The channel to produce the waveform on
    PrescaleStage1 = The ClockA/ClockB prescale to divide the bus rate by
    PrescaleStage2 = The SA/SB prescale to divide the ClockA/ClockB by
    Polarity = The polarity of the waveform (positive/negative)
    Period = The program period value (max 8-bit = 255)
    Duty = The program duty value (max 8-bit = 255)
*/
void Pulse_Init_8Bit(
    Pulse_Channel channel,
    Pulse_PrescaleStage1 PrescaleStage1,
    unsigned char PrescaleStage2,
    Pulse_Polarity polarity,
    unsigned char period,
    unsigned char duty);

/*
    Initializes the PWM Module to product a square wave according to the parameters given in 16-bit mode.

    Channel = The channel to produce the waveform on
    PrescaleStage1 = The ClockA/ClockB prescale to divide the bus rate by
    PrescaleStage2 = The SA/SB prescale to divide the ClockA/ClockB by
    Polarity = The polarity of the waveform (positive/negative)
    Period = The program period value (max 8-bit = 255)
    Duty = The program duty value (max 8-bit = 255)
*/
void Pulse_Init_16Bit(
    Pulse_Channel channel,
    Pulse_PrescaleStage1 PrescaleStage1,
    unsigned char PrescaleStage2,
    Pulse_Polarity polarity,
    unsigned int period,
    unsigned int duty);

/*
    Set the ClockA/B and ClockSA/SB prescales

    Channel = The channel to produce the waveform on
    PrescaleStage1 = The ClockA/ClockB prescale to divide the bus rate by
    PrescaleStage2 = The SA/SB prescale to divide the ClockA/ClockB by
 */
void Pulse_SetClockPrescales(Pulse_Channel channel, Pulse_PrescaleStage1 prescaleStage1, unsigned char prescaleStage2);

/*
    Update the value of the period on a given channel.

    Channel = The channel to produce the waveform on
    Period = The new period value to set on the channel
*/

void Pulse_SetPeriod_8Bit(Pulse_Channel channel, unsigned char period);
/*
    Update the value of the period on a given channel.

    Channel = The channel to produce the waveform on
    Period = The new period value to set on the channel
*/
void Pulse_SetPeriod_16Bit(Pulse_Channel channel, unsigned int period);

/*
    Update the value of the duty on a given channel.

    Channel = The channel to produce the waveform on
    Duty = The new duty value to set on the channel
*/
void Pulse_SetDuty_8Bit(Pulse_Channel channel, unsigned char duty);
/*
    Update the value of the duty on a given channel.

    Channel = The channel to produce the waveform on
    Duty = The new duty value to set on the channel
*/
void Pulse_SetDuty_16Bit(Pulse_Channel channel, unsigned int duty);

/*
    Set which clock stage (A/B or SA/SB) will be used by the given channel

    Channel = The channel to produce the waveform on
    UseStage2Clock = If true, SA/SB will be used. If false, ClockA/B will be used
*/
void Pulse_SelectClock(Pulse_Channel channel, unsigned char useStage2Clock);

/*
    Update the value of the polarity on a given channel.

    Channel = The channel to produce the waveform on
    Polarity = The polarity of the waveform (positive/negative)
*/
void Pulse_SetPolarity(Pulse_Channel channel, Pulse_Polarity polarity);

/*
    Enable or disable the given channel.

    Channel = The channel to produce the waveform on
    Control = Enable or disable the channel
*/
void Pulse_ChannelControl(Pulse_Channel channel, Pulse_Control control);
/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

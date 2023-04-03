/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Created:       November 23, 2022
// Details:       Initialize and read values from the AtoD module
/////////////////////////////////////////////////////////////////////////////

// ISR:
/*
interrupt VectorNumber_Vatd0 void INT_AD0 (void)
{
 // read channel values (reading any clears interrupt flag)
 ADVal[0] = ATD0DR0;
 //ADVal[1] = ATD0DR1;
 //ADVal[2] = ATD0DR2;
 //ADVal[3] = ATD0DR3;
 //ADVal[4] = ATD0DR4;
 //ADVal[5] = ATD0DR5;
 //ADVal[6] = ATD0DR6;
 //ADVal[7] = ATD0DR7;
}
*/

// Display voltage on LCD:
/*
    (void)sprintf(buffer, "Ch5 = %0.02fV", sample * 5E-3);
    LCD_StringXY(0, 0, buffer);
*/

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////
typedef enum AtoD_InterruptMode
{
    AtoD_InterruptsOff,
    AtoD_InterruptsOn
} AtoD_InterruptMode;

typedef enum AtoD_Channels
{
    AtoD_Channel0,
    AtoD_Channel1,
    AtoD_Channel2,
    AtoD_Channel3,
    AtoD_Channel4,
    AtoD_Channel5,
    AtoD_Channel6,
    AtoD_Channel7
} AtoD_Channels;

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

// initialized the AtoD0 module
// assumes 20MHz bus rate
// assumes interrupts are available on the A/D
void AtoD_Init(AtoD_InterruptMode intMode);

// read the desired channel, assumes fast flag clearing from init
unsigned int AtoD_Read(AtoD_Channels channel);

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

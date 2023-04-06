/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Created:       March 20, 2023
// Details:       Port A control for CMPE2965 poker chip sorter project.
//                Allows for control of PortA outputs with masking using enums.
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////
typedef enum PortAMasks
{
    PumpControl = 0b10000000,
    MotorDisable = 0b01000000,
    Motor3Direction = 0b00100000,
    Motor3StepPulse = 0b00010000,
    Motor2Direction = 0b00001000,
    Motor2StepPulse = 0b00000100,
    Motor1Direction = 0b00000010,
    Motor1StepPulse = 0b00000001
} PortAMasks;

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////
void Cap_PortAInit(void);
unsigned char Cap_PortARead(unsigned char mask);
void Cap_PortASet(unsigned char mask);
void Cap_PortAClear(unsigned char mask);
void Cap_PortAToggle(unsigned char mask);

void Cap_MoveEffector(int m1TargetStep, int m2TargetStep, int m3TargetStep);

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

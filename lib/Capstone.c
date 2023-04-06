/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        Andrew Belter
// Created:       March 20, 2023
// Details:       Port A control for CMPE2965 poker chip sorter project.
//                Allows for control of PortA outputs with masking using enums.
// Revision History
//      March 20, 2023 - Created lib and PortA enum masks
//      March 21, 2023 - Implemented PortA management functions
//      March 23, 2023 - Implemented motor movement functions - untested
/////////////////////////////////////////////////////////////////////////////

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include <math.h>

#include "capstone.h"
#include "pit.h"

// other includes, as *required* for this implementation

/////////////////////////////////////////////////////////////////////////////
// local prototypes
/////////////////////////////////////////////////////////////////////////////
void Cap_MotorStep(unsigned char mask);
void Cap_SetMotorDirection(unsigned char motorDirectionMask, int currentStep, int targetStep);

// inverse kinematics functions translated from JavaScript (source : https://www.marginallyclever.com/other/samples/fk-ik-test.html)

void Cap_InitInvKinGlobals(void);
unsigned char Cap_CalcAngleXYZ(float x, float y, float z, float *theta);
unsigned char Cap_CalcInvKin(float *thetas, float x, float y, float z);
int Cap_AngleToStep(float theta);

/////////////////////////////////////////////////////////////////////////////
// library variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////

// inverse kinematics constants (mm)
const float EffectorRadius = 35;
const float BaseRadius = 100;
const float ForearmLength = 280;
const float BicepLength = 285;
const float BaseToFloor = 200;

// number of motor steps per revolution
const unsigned int StepsPerRev = 3200;

/////////////////////////////////////////////////////////////////////////////
// function implementations
/////////////////////////////////////////////////////////////////////////////

// Initializes Port A as output with all bits initially set to 0 except PA6 (motor disable)
void Cap_PortAInit(void)
{
    // clear all bits except enable and set data direction to all outputs
    PORTA &= MotorDisable;
    DDRA = 0xFF;
}

// Returns all bits set in the mask
unsigned char Cap_PortARead(unsigned char mask)
{
    return PORTA & mask;
}

// Sets all bits in the mask
void Cap_PortASet(unsigned char mask)
{
    PORTA |= mask;
}

// Clears all bits in the mask
void Cap_PortAClear(unsigned char mask)
{
    PORTA &= ~mask;
}

// Toggles all bits in the mask
void Cap_PortAToggle(unsigned char mask)
{
    PORTA ^= mask;
}

// Moves the effector the to requried position based on the target step for each motor
void Cap_MoveEffector(int m1TargetStep, int m2TargetStep, int m3TargetStep)
{
    static int m1CurrentStep = 0;
    static int m2CurrentStep = 0;
    static int m3CurrentStep = 0;
    unsigned char motorsToMove = 0;

    // setup the direction bits for all the motors before starting to step
    Cap_SetMotorDirection(Motor1Direction, m1CurrentStep, m1TargetStep);
    Cap_SetMotorDirection(Motor2Direction, m2CurrentStep, m2TargetStep);
    Cap_SetMotorDirection(Motor3Direction, m3CurrentStep, m3TargetStep);

    // enable the motors
    Cap_PortAClear(MotorDisable);

    // step the motors until all motors have reached their target step
    do
    {
        // reset mask to use as an end condition flag
        motorsToMove = 0;

        // check which motors still need to move, adding their StepPulse to the motorsToMove mask if they need to
        if (m1CurrentStep != m1TargetStep)
        {
            motorsToMove |= Motor1StepPulse;
            // m1CurrentStep = Cap_PortARead(Motor1Direction) ? m1CurrentStep + 1 : m1CurrentStep - 1;//dir high = increase step
            m1CurrentStep = Cap_PortARead(Motor1Direction) ? m1CurrentStep - 1 : m1CurrentStep + 1;//dir high = decrease step
        }
        if (m2CurrentStep != m2TargetStep)
        {
            motorsToMove |= Motor2StepPulse;
            // m2CurrentStep = Cap_PortARead(Motor2Direction) ? m2CurrentStep + 1 : m2CurrentStep - 1;//dir high = increase step
            m2CurrentStep = Cap_PortARead(Motor2Direction) ? m2CurrentStep - 1 : m2CurrentStep + 1;//dir high = decrease step
        }
        if (m3CurrentStep != m3TargetStep)
        {
            motorsToMove |= Motor3StepPulse;
            // m3CurrentStep = Cap_PortARead(Motor3Direction) ? m3CurrentStep + 1 : m3CurrentStep - 1;//dir high = increase step
            m3CurrentStep = Cap_PortARead(Motor3Direction) ? m3CurrentStep - 1 : m3CurrentStep + 1;//dir high = decrease step
        }

        // if there are motors to move, step them
        if (motorsToMove)
            Cap_MotorStep(motorsToMove);
    } while (motorsToMove);

    // disable the motors
    Cap_PortASet(MotorDisable);
}

/////////////////////////////////////////////////////////////////////////////
// Hidden Helpers (local to implementation only)
/////////////////////////////////////////////////////////////////////////////

// Steps all motors in the mask once by setting the mask bits, sleeping for 100us, then clearing the mask bits.
void Cap_MotorStep(unsigned char mask)
{
    // set the mask bits, sleep for 100us, then clear the mask bits
    // the mask contains the StepPulse bits of all motors to be steped
    Cap_PortASet(mask);
    PIT_Sleep_us(PIT_Channel_1, 20E6, 100);
    Cap_PortAClear(mask);
}

// Sets or clears the motor's direction bit based on if the motor's current step is above or below the target step
// Bit does not change if currentStep = targetStep
void Cap_SetMotorDirection(unsigned char motorDirectionMask, int currentStep, int targetStep)
{
    // if the current step count is less than the target count,
    // clear the direction bit to increase step count while stepping
    if (currentStep < targetStep)
        Cap_PortAClear(motorDirectionMask);//dir low = increase step
    // if the current step count is greater than the target count,
    // set the direction bit to decrease step count while stepping
    else if (currentStep > targetStep)
        Cap_PortASet(motorDirectionMask);//dir high = decrease step
}

// inverse kinematics functions translated from JavaScript (source : https://www.marginallyclever.com/other/samples/fk-ik-test.html)

double sqrt3, pi, sin120, cos120, tan60, sin30, tan30;
void Cap_InitInvKinGlobals()
{
    sqrt3 = sqrt(3);
    pi = _M_PI;
    sin120 = sqrt3 / 2.0;
    cos120 = -0.5;
    tan60 = sqrt3;
    sin30 = 0.5;
    tan30 = 1.0 / sqrt3;
}

// returns 0 for success, -1 for error
unsigned char Cap_CalcAngleXYZ(float x0, float y0, float z0, float *theta)
{
    float y1 = -0.5 * tan30 * BaseRadius;
    y0 -= 0.5 * tan30 * EffectorRadius; // shift center to edge
    {
        float aV = (x0 * x0 + y0 * y0 + z0 * z0 + BicepLength * BicepLength - ForearmLength * ForearmLength - y1 * y1) / (2 * z0);
        float bV = (y1 - y0) / z0;

        // discriminant
        float d = -(aV + bV * y1) * (aV + bV * y1) + BicepLength * (bV * bV * BicepLength + BicepLength);
        if (d < 0) // non-exisiting power - return error -1, theta = 0
        {
            *theta = 0;
            return -1;
        }
        {
            float yj = (y1 - aV * bV - sqrt(d)) / (bV * bV + 1);
            float zj = aV + bV * yj;
            *theta = atan2(-zj, y1 - yj) * 180.0 / pi;
        }
    }
    return 0;
}

// returns 0 for success, -# for error - # is what step it failed on
unsigned char Cap_CalcInvKin(float *thetas, float x0, float y0, float z0)
{
    float theta1, theta2, theta3;

    if (Cap_CalcAngleXYZ(x0, y0, z0, &theta1))
        return -1;
    thetas[0] = theta1;

    if (Cap_CalcAngleXYZ(x0 * cos120 + y0 * sin120, y0 * cos120 - x0 * sin120, z0, &theta2))
        return -2;
    thetas[1] = theta2;

    if (Cap_CalcAngleXYZ(x0 * cos120 - y0 * sin120, y0 * cos120 + x0 * sin120, z0, &theta3))
        return -3;
    thetas[2] = theta3;

    return 0;
}

// converts an angle (in degrees) to a step number
int Cap_AngleToStep(float theta)
{
    return (int)(theta * StepsPerRev / 360);
}
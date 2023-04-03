// Timer Library
// Revision History:
// Created - Unknown
// Dec 2020 - Simplified function names, added prescale enum

typedef enum Timer_Prescale
{
  Timer_Prescale_1 = 0,
  Timer_Prescale_2 = 1,
  Timer_Prescale_4 = 2,
  Timer_Prescale_8 = 3,
  Timer_Prescale_16 = 4,
  Timer_Prescale_32 = 5,
  Timer_Prescale_64 = 6,
  Timer_Prescale_128 = 7
} Timer_Prescale;

typedef enum Timer_Channel
{
  Timer_Channel_0 = 0b00000001,
  Timer_Channel_1 = 0b00000010,
  Timer_Channel_2 = 0b00000100,
  Timer_Channel_3 = 0b00001000,
  Timer_Channel_4 = 0b00010000,
  Timer_Channel_5 = 0b00100000,
  Timer_Channel_6 = 0b01000000,
  Timer_Channel_7 = 0b10000000
} Timer_Channel;

// actions pins can take on compare event
typedef enum Timer_PinAction
{
  Timer_Pin_Disco = 0,
  Timer_Pin_Toggle = 1,
  Timer_Pin_Clear = 2,
  Timer_Pin_Set = 3
} Timer_PinAction;

typedef enum Timer_Mode
{
  Timer_PollingMode = 0,
  Timer_InterruptMode = 1
} Timer_Mode;

// Initializes the timer and sets up the given timer channel to operate as an output compare channel at the given prescale.
// The channel will have an initial offset
//  Parameters:
//       ulBusRate is bus clock (in Hz)
//       prescale is prescale enum (range 0-7)
//       initialOffset_ms is the initial offset for timer (in ms) - function uses busClock and prescale to calculate the number of ticks needed - doesn't work with more than 1 cycle
//       mode enables interrupts according to what is passed in (polling or interrupt mode)
//       pinAction is the action desired based on the PinAction enum
void Timer_Init(unsigned long ulBusRate, Timer_Prescale prescale, unsigned int initialOffset_ms, Timer_Channel channel, Timer_Mode mode, Timer_PinAction pinAction);

// blocking delay for x ms
// initializes timer channel 6 to be used for the sleep
// calculates values required to generate 1ms delay (may not be perfectly accurate) then loops ms times
// uses OC6, no pin action for timing
// if the timer was enabled by this function, it is disabled after the function is finished
void Timer_Sleep(unsigned int ms);

// uses the global prescale and bus clock variables initialized by the Timer_Init function
// to calculate the number of counts TC0 must wait before the next flag is raise
// takes in a number of milliseconds and returns the counts required
unsigned int Timer_CalculateCountMilli(unsigned int ms);

// uses the global prescale and bus clock variables initialized by the Timer_Init function
// to calculate the number of counts TC0 must wait before the next flag is raise
// takes in a number of microseconds and returns the counts required
unsigned int Timer_CalculateCountMicro(unsigned int us);

/////////////////////////////////////////////////////////////////////////////
// Code Templates
/////////////////////////////////////////////////////////////////////////////
/*
interrupt VectorNumber_Vtimch0 void IOC0 (void){
  TFLG1 = TFLG1_C0F_MASK;//clear the flag
  TC0 += TimerInterval;//rearm for the next event
}
*/
#ifndef HEADER_AND_C_FILES_TIMER_H_
#define HEADER_AND_C_FILES_TIMER_H_

#include "em_cmu.h"
#include "em_device.h"
#include "em_letimer.h"


extern uint16_t Minutes; //Minutes that have passed since power on
extern uint32_t CurrentSeconds; //Seconds that have passed since power on
extern uint32_t PrevSeconds; //Seconds that had passed the previous loop through the main function
extern uint32_t CurrentTime; //Current count on the timer
extern uint32_t PrevTime; //Count on the timer the previous loop through the main function

void Timer_SetupAll(void); //Initialize and setup the timer

//Set timer interval
void Timer_Set_Interval(float frac);

void Timer_Start(void); //Start the timer

void Timer_Stop(void); //Stop the timer

void Timer_EnableInterrupt(void); //Enables timer interrupt

void Timer_DisableInterrupt(void); //Disables timer interrupt

void Timer_IncrementMinutes(void); //Increments the minute counter and resets some variables

#endif /* HEADER_AND_C_FILES_TIMER_H_ */

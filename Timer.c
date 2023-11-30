#include "Timer.h"

uint16_t Minutes = 0;
uint32_t CurrentSeconds = 0;
uint32_t CurrentTime = 0;
uint32_t PrevTime = 0;


void Timer_SetupAll(void){
  //Also turned this off to be safe..
  //CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
  //Could this be the problem??
  //CMU_HFRCOBandSet(cmuHFRCOFreq_2M0Hz);
  ////
  //CMU_ClockEnable(cmuClock_HF,true);
  //CMU_ClockSelectSet(cmuClock_HF, cmuSelect_LFXO); //
  ////
  ///
  //CMU_ClockSelectSet(cmuClock_CORE, cmuSelect_LFXO);
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); //
//  CMU->LFACLKSEL = CMU_LFACLKSEL_LFA_ULFRCO; //1kHz clock
  CMU_ClockEnable(cmuClock_LFA, true);
  CMU_ClockEnable(cmuClock_LETIMER0, true);

  LETIMER_Init_TypeDef LPT_init = LETIMER_INIT_DEFAULT;
  LPT_init.enable = false;
  LPT_init.comp0Top = true;
  LETIMER_Init(LETIMER0, &LPT_init);
  LETIMER_CompareSet(LETIMER0, 0, 1 * CMU_ClockFreqGet(cmuClock_LFA));
  LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF);
  NVIC_EnableIRQ(LETIMER0_IRQn);
}

void Timer_Set_Interval(float frac){
  LETIMER_CompareSet(LETIMER0, 0, frac * CMU_ClockFreqGet(cmuClock_LFA));
}

void Timer_Start(void){
  LETIMER_Enable(LETIMER0, true);
}

void Timer_Stop(void){
  LETIMER_Enable(LETIMER0, false);
}

void Timer_EnableInterrupt(void){
  LETIMER_IntEnable(LETIMER0, LETIMER_IEN_UF);
}

void Timer_DisableInterrupt(void){
  LETIMER_IntDisable(LETIMER0, LETIMER_IEN_UF);
}

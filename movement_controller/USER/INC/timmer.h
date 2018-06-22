#ifndef _TIMMER_H
#define _TIMMER_H

#include "stm32f10x_conf.h"
void CounterConfig(void);
void HeartBeat(FunctionalState NewState);
void Timmer_TIM2(FunctionalState NewState);
void SystemInitSuccessTimmer(FunctionalState NewState);
#endif







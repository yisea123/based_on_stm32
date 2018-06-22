#ifndef __DELAY_H
#define __DELAY_H

//#include "system_stm32f10x.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_systick.h"
#include "misc.h"
#include "core_cm3.h"

void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);
void Systick_Configuration(void);

#endif

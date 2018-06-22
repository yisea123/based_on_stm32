/**
 * 
 */

#ifndef __CURRENT_PROTECT_H
#define __CURRENT_PROTECT_H

#include "stm32f10x_conf.h"
#include "command.h"
#include "usart.h"
#include "delay.h"

#define  FORWARD_TRIGGER_PIN        GPIO_Pin_8
#define  RESERVED_TRIGGER_PIN       GPIO_Pin_9


void CurrentTriggerConfigurate(void);
void CurrentTrigger_IO_Configurate(void);
#endif


#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x_conf.h"
#include "blink.h"
#include "delay.h"
#include "spi.h"
#include "usart.h"
//#include "exti_interrupt.h"
#include "system_config.h"
#include "AD7190.h"
//#include "key.h"


#define SYSCLK 			(uint8_t)72000000UL

void systemInit(void);


#endif



/************************************* END LINE ************************************************/


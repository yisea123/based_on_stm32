#ifndef __BLINK_H
#define __BLINK_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "command.h"


#define LED1_PORT 		GPIO_Pin_14
#define LED2_PORT 		GPIO_Pin_15
#define LED3_PORT 		GPIO_Pin_5

#define LED1_OFF() 		GPIO_SetBits(PORTB, LED1_PORT)
#define LED1_ON() 		GPIO_ResetBits(PORTB, LED1_PORT)

#define LED2_OFF() 		GPIO_SetBits(PORTB, LED2_PORT)
#define LED2_ON() 		GPIO_ResetBits(PORTB, LED2_PORT)

#define LED3_OFF() 		GPIO_SetBits(PORTB, LED3_PORT)
#define LED3_ON() 		GPIO_ResetBits(PORTB, LED3_PORT)

typedef enum {
	Mode1            		= 1,
	Mode2            		= 2,
	Mode3            		= 3,
	SysInitMode      		= 4,
    SysRuningMode 			= 5,
	CAN_RecMode      		= 6,
	SystemSelfCheckError 	= 7,
	SystemWaitting 			= 8

} blinkMode;

void BlinkInit(void);
void Blink(blinkMode mode, uint32_t timmer);

#endif

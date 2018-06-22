#ifndef __BLINK_H
#define __BLINK_H

#include "stm32f10x_conf.h"
#include "delay.h"
#define LED0		GPIOB
#define LED1 		GPIOB

#define LED0_Pin    GPIO_Pin_0
#define LED1_Pin    GPIO_Pin_1

#define SET_LED0()	GPIO_ResetBits(LED0, LED0_Pin);
#define CLR_LED0() 	GPIO_SetBits(LED0, LED0_Pin);

#define SET_LED1()	GPIO_ResetBits(LED1, LED1_Pin);
#define CLR_LED1() 	GPIO_SetBits(LED1, LED1_Pin);

void GPIO_Config(void);
void blink(void);
void CountBlink(uint8_t num, uint8_t count, uint8_t ms);

void LED_AllSet(void);
void LED_AllDim(void);

#endif


/******************************* END LINE *************************************/


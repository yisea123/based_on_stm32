/**************************************************************
* key define:
		1. key_increase		-> PE4	alias key0
		2. key_subtract		-> PE2	alias key2
		3. key_register		-> PE3	alias key1
***************************************************************/

#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x_conf.h"
//#include "AD9834.h"
#include "blink.h"

//#define     TRUE            	1
//#define     FALSE           	0

#define 	Key_GPIO			GPIOE
#define 	key_increase		GPIO_Pin_4
#define		key_subtract 		GPIO_Pin_2
#define		key_FreqRegister	GPIO_Pin_3

extern uint8_t key_increase_Interrup;
extern uint8_t key_subtract_Interrup;
extern uint8_t registerChage_flag;

extern uint32_t IncreaseCounterTimes;
extern uint32_t FreqRegisterCounterTimes;
extern uint32_t sbutractCounterTimes;

typedef struct
{
	uint8_t increase_flag;
	uint8_t sbutract_flag;
	uint8_t FreqRegisterChage_flag;
}key_FlagTypedef;




void key_Init(void);
//void key_scan(void);

#endif

/************************************* END LINE ************************************************/

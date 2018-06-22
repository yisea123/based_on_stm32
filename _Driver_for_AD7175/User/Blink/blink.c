/***********************************************************************************
* LED set
************************************************************************************/
#include "blink.h"

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED0_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = LED1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void blink(void)
{
	SET_LED0();
	delay_ms(1000);
	SET_LED1();
	delay_ms(1000);
	CLR_LED0();
	delay_ms(1000);
	CLR_LED1();
	delay_ms(1000);
}

void CountBlink(uint8_t num, uint8_t count, uint8_t ms)
{
	if(num == 0)
	{
		for (; count > 0; count--)
		{
			SET_LED0();
			delay_ms(ms);
			CLR_LED0();
			delay_ms(ms);
		}
	}
	else
	{
		for (; count > 0; count--)
		{
			SET_LED1();
			delay_ms(ms);
			CLR_LED1();
			delay_ms(ms);
		}
	}
}
void LED_AllSet(void)
{
	SET_LED0();
	SET_LED1();
}
void LED_AllDim(void)
{
	CLR_LED0();
	CLR_LED1();
}

/******************************* END LINE *************************************/


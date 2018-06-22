/*-------------------------------------------------
 *@Brif: file is about LED status and system watching LED configuration
 *@Author: V
 *@Date: 2017-11-09
 ------------------------------------------------*/
#include "blink.h"

/**
 * @brief: Initialize the LED GPIO
 * @param: None
 * @return: None
 */
void BlinkInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStruction;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStruction.GPIO_Pin = LED1_PORT | LED2_PORT | LED3_PORT;
	GPIO_InitStruction.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStruction.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(PORTB, &GPIO_InitStruction);
}

/**
 * @brief: through blinkmode typedef to switch the blink function
 * @param: blinkmode  typedef struct file, can be set in BlinkMode typedef strct
 * @param: timmer     timmer for blink delay times
 * @return: None
 */
void Blink(blinkMode mode, uint32_t timmer)
{	
	if(mode == Mode1)
	{
		for(uint8_t times = 0; times < 3; times++)
		{
			LED1_ON();
			LED2_ON();
			LED3_ON();
			DelayMs(timmer);
			LED1_OFF();
			LED2_OFF();
			LED3_OFF();
            DelayMs(timmer);
		}
	}
	else if(mode == Mode2)
	{
		LED1_ON();
		DelayMs(timmer);
		LED1_OFF();
		DelayMs(timmer);
	}
    else if(mode == Mode3)
    {
        LED2_ON();
        DelayMs(timmer);
        LED2_OFF();
        DelayMs(timmer);
    }
	else if(mode == SysInitMode)
	{
		for(uint8_t LED_Counter = 0; LED_Counter < 3; LED_Counter++)
		{
			LED1_ON();
			LED2_ON();
			LED3_ON();
			DelayMs(timmer);
			LED1_OFF();
			LED2_OFF();
			LED3_OFF();
            DelayMs(timmer);
		}
	}
	else if(mode == SysRuningMode)
	{
		LED3_ON();
		DelayMs(timmer);
		LED3_OFF();
		DelayMs(timmer);
	}
	else if (mode == CAN_RecMode)
	{
		LED2_ON();
		DelayMs(timmer);
		LED2_OFF();
		DelayMs(timmer);
	}
	else if(mode == SystemSelfCheckError)
	{
		for(uint8_t blinkTimes = 0; blinkTimes < 3; blinkTimes++)
		{
			LED1_ON();
			LED2_ON();
			LED3_ON();
			DelayMs(300);
			LED1_OFF();
			LED2_OFF();
			LED3_OFF();
			DelayMs(300);
		}
		for (uint8_t blinkTimes = 0; blinkTimes < 3; blinkTimes++)
		{
            DelayMs(1000);
			LED1_ON();
			LED2_ON();
			LED3_ON();
			DelayMs(1000);
			LED1_OFF();
			LED2_OFF();
			LED3_OFF();
			DelayMs(1000);
            
		}
        DelayMs(1000);
	}
	else if (mode == SystemWaitting)
	{
		LED1_ON();
		DelayMs(timmer);
		LED1_OFF();
		DelayMs(timmer);
	}
}
/*-------------------------------------- END LINE -----------------------------------------*/


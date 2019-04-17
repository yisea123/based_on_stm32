/*
 * key.c
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#include <global_cmd.h>
#include "key.h"

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE ()
	;

	/*Configure GPIO pins : PBPin PBPin */
	GPIO_InitStruct.Pin = TRIGGER1_Pin | TRIGGER2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( TRIGGER_PORT, &GPIO_InitStruct);
	key_nvic_set();
}

void key_nvic_set(void)
{
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 1);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 2);

}

void enable_key_trigger(void)
{
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	trigger_mark = untrigger;
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_8);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_11);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin) {
	case TRIGGER1_Pin:
		if (0 == HAL_GPIO_ReadPin( TRIGGER_PORT, TRIGGER1_Pin)) {
			trigger_mark = triggered;
			key_triggered_times++;
		}
		standby_mark = off;
		break;
	case TRIGGER2_Pin:
		if (0 == HAL_GPIO_ReadPin( TRIGGER_PORT, TRIGGER2_Pin)) {
			trigger_mark = triggered;
			key_triggered_times++;
		}
		standby_mark = off;
		break;
	default:
		break;
	}
}


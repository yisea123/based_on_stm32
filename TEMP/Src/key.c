/*
 * key.c
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */


#include "key.h"
#include "gloable_cmd.h"

void
key_init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE ();

	  /*Configure GPIO pins : PBPin PBPin */
	GPIO_InitStruct.Pin = TRIGGER1_Pin | TRIGGER2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init ( TRIGGER_PORT, &GPIO_InitStruct );
	key_nvic_set ();
}

void
key_nvic_set (void)
{
	HAL_NVIC_SetPriority ( EXTI9_5_IRQn, 0, 0 );
	HAL_NVIC_EnableIRQ ( EXTI9_5_IRQn );
	HAL_NVIC_SetPriority ( EXTI15_10_IRQn, 0, 1 );
	HAL_NVIC_EnableIRQ ( EXTI15_10_IRQn );
}

void
EXTI9_5_IRQHandler (void)
{
	HAL_GPIO_EXTI_IRQHandler ( GPIO_PIN_8 );
}

void
EXTI15_10_IRQHandler (void)
{
	HAL_GPIO_EXTI_IRQHandler ( GPIO_PIN_11 );
}

void
HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
	HAL_Delay ( 100 );
	switch (GPIO_Pin) {
		case TRIGGER1_Pin:
			if (0 == HAL_GPIO_ReadPin ( TRIGGER_PORT, TRIGGER1_Pin )) {
				trigger_mark = triggered;
			}
		break;
		case TRIGGER2_Pin:
			if (0 == HAL_GPIO_ReadPin ( TRIGGER_PORT, TRIGGER2_Pin ))
							{
				trigger_mark = triggered;
			}
		break;
	}
}

/*
 * coil.c
 *
 *  Created on: Apr 16, 2019
 *      Author: QUAN
 */

#ifndef COIL_C_
#define COIL_C_

#include "coil.h"

void coil_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = COIL_CTRL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(COIL_CTRL_PORT, &GPIO_InitStruct);
	coil_power(off);
}

void coil_power(gpio_status status)
{
	HAL_GPIO_WritePin(COIL_CTRL_PORT, COIL_CTRL_PIN, status);
	blink_heatting(status);
}

void coil_keep_warm(gpio_status status)
{
	blink_keep_warming();
}

#endif /* COIL_C_ */

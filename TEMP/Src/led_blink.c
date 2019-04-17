/*
 * led_blink.c
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#include "led_blink.h"

void blink_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE ();
	__HAL_RCC_GPIOB_CLK_ENABLE ();

	GPIO_InitStruct.Pin = LED1_BLUE_Pin | LED1_GREEN_Pin | LED1_RED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( LED1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED2_BLUE_Pin | LED2_GREEN_Pin | LED2_RED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( LED2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED3_BLUE_TIM1_CH1_Pin | LED3_GREEN_TIM1_CH2_Pin
			| LED3_RED_TIM1_CH3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( LED3_PORT, &GPIO_InitStruct);

	blink_statu_reset(on);
}

void LEDx_Set(led_list LEDx, led_color color, gpio_status status)
{
	switch (LEDx) {
	case led1:
		if (led1_blue == color)
			HAL_GPIO_WritePin( LED1_PORT, LED1_BLUE_Pin, status);
		else if (led1_green == color)
			HAL_GPIO_WritePin( LED1_PORT, LED1_GREEN_Pin, status);
		else if (led1_red == color)
			HAL_GPIO_WritePin( LED1_PORT, LED1_RED_Pin, status);
		break;
	case led2:
		if (led2_blue == color)
			HAL_GPIO_WritePin( LED2_PORT, LED2_BLUE_Pin, status);
		else if (led2_green == color)
			HAL_GPIO_WritePin( LED2_PORT, LED2_GREEN_Pin, status);
		else if (led2_red == color)
			HAL_GPIO_WritePin( LED2_PORT, LED2_RED_Pin, status);
		break;
	case led3:
		if (led3_blue == color)
			HAL_GPIO_WritePin( LED3_PORT, LED3_BLUE_TIM1_CH1_Pin, status);
		else if (led3_green == color)
			HAL_GPIO_WritePin( LED3_PORT, LED3_GREEN_TIM1_CH2_Pin, status);
		else if (led3_red == color)
			HAL_GPIO_WritePin( LED3_PORT, LED3_RED_TIM1_CH3_Pin, status);
		break;
	}
}

void blink_statu_reset(gpio_status status)
{
	LEDx_Set(led1, led1_blue, status);
	LEDx_Set(led1, led1_green, status);
	LEDx_Set(led1, led1_red, status);
	LEDx_Set(led2, led2_blue, status);
	LEDx_Set(led2, led2_green, status);
	LEDx_Set(led2, led2_red, status);
	LEDx_Set(led3, led3_blue, status);
	LEDx_Set(led3, led3_green, status);
	LEDx_Set(led3, led3_red, status);
}

void blink_standby(void)
{
	if (on == standby_mark) {
		blink_statu_reset(on);
		HAL_Delay(200);
		LEDx_Set(led2, led2_green, off);
		LEDx_Set(led2, led2_red, off);
		HAL_Delay(200);
		LEDx_Set(led2, led2_green, on);
		LEDx_Set(led2, led2_red, on);
		HAL_Delay(200);
	}
}

void blink_keep_warming(void)
{
	blink_statu_reset(on);
	HAL_Delay(100);
	LEDx_Set(led1, led1_green, off);
	HAL_Delay(100);
	LEDx_Set(led2, led2_green, off);
	HAL_Delay(100);
	LEDx_Set(led3, led3_green, off);
	HAL_Delay(100);
}

void blink_heatting(gpio_status status)
{
	blink_statu_reset(on);
	HAL_Delay(100);
	LEDx_Set(led1, led1_green, !status);
	LEDx_Set(led1, led1_red, !status);
	HAL_Delay(100);
	LEDx_Set(led2, led2_green, !status);
	LEDx_Set(led2, led2_red, !status);
	HAL_Delay(100);
	LEDx_Set(led3, led3_green, !status);
	LEDx_Set(led3, led3_red, !status);
	HAL_Delay(100);
}

void blink_overheat(void)
{
	blink_statu_reset(on);
	while (on == over_heart_mark) {
		LEDx_Set(led1, led1_red, on);
		LEDx_Set(led2, led2_red, on);
		LEDx_Set(led3, led3_red, on);
		HAL_Delay(300);
		LEDx_Set(led1, led1_red, off);
		LEDx_Set(led2, led2_red, off);
		LEDx_Set(led3, led3_red, off);
		HAL_Delay(300);
	}
	blink_statu_reset(on);
}

void blink_self_check_pass(void)
{
	blink_statu_reset(off);
	HAL_Delay(100);
	blink_statu_reset(on);
	HAL_Delay(100);
}

void blink_self_check_error(void)
{
	blink_statu_reset(on);
	while (on == error_self_check_mark) {
		LEDx_Set(led1, led1_red, on);
		LEDx_Set(led2, led2_red, on);
		LEDx_Set(led3, led3_red, on);
	}
	blink_statu_reset(on);
}


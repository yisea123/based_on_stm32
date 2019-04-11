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
	__HAL_RCC_GPIOA_CLK_ENABLE ()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE ()
	;

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

	blink_statu_reset();
}

void LEDx_Set(led_list LEDx, led_color color, gpio_status status)
{
	switch (LEDx) {
	case led1:
		if (led1_blue == color)
			HAL_GPIO_WritePin( TRIGGER_PORT, LED1_BLUE_Pin, status);
		else if (led1_green == color)
			HAL_GPIO_WritePin( TRIGGER_PORT, LED1_GREEN_Pin, status);
		else if (led1_red == color)
		    HAL_GPIO_WritePin( TRIGGER_PORT, LED1_RED_Pin, status);
		break;
	case led2:
		if (led2_blue == color)
			HAL_GPIO_WritePin( TRIGGER_PORT, LED2_BLUE_Pin, status);
		else if (led2_green == color)
			HAL_GPIO_WritePin( TRIGGER_PORT, LED2_GREEN_Pin, status);
		else if (led2_red == color)
		    HAL_GPIO_WritePin( TRIGGER_PORT, LED2_RED_Pin, status);
		break;
	case led3:
		if (led3_blue == color)
			HAL_GPIO_WritePin( TRIGGER_PORT, LED3_BLUE_TIM1_CH1_Pin, status);
		else if (led3_green == color)
			HAL_GPIO_WritePin( TRIGGER_PORT, LED3_GREEN_TIM1_CH2_Pin, status);
		else if (led3_red == color)
		    HAL_GPIO_WritePin( TRIGGER_PORT, LED3_RED_TIM1_CH3_Pin, status);
		break;
	}
}

void blink_statu_reset(void)
{
	LEDx_Set(led1, led1_blue, off);
	LEDx_Set(led1, led1_green, off);
	LEDx_Set(led1, led1_red, off);
	LEDx_Set(led2, led2_blue, off);
	LEDx_Set(led2, led2_green, off);
	LEDx_Set(led2, led2_red, off);
	LEDx_Set(led3, led3_blue, off);
	LEDx_Set(led3, led3_green, off);
	LEDx_Set(led3, led3_red, off);
}

void blink_standby(void)
{
	blink_statu_reset();
	while(on == standby_mark) {
		LEDx_Set(led3, led3_green, off);
		HAL_Delay(1000);
		LEDx_Set(led3, led3_green, on);
		HAL_Delay(1000);
	}
}

void blink_runing(void)
{
	blink_statu_reset();
	LEDx_Set(led1, led1_green, on);
	HAL_Delay(500);
	LEDx_Set(led2, led2_green, on);
	HAL_Delay(500);
	LEDx_Set(led3, led3_green, on);
	HAL_Delay(500);
}

void blink_overheat(void)
{
	blink_statu_reset();
	while(on == over_heart_mark) {
		LEDx_Set(led1, led1_red, on);
		LEDx_Set(led2, led2_red, on);
		LEDx_Set(led3, led3_red, on);
		HAL_Delay(500);
		LEDx_Set(led1, led1_red, off);
		LEDx_Set(led2, led2_red, off);
		LEDx_Set(led3, led3_red, off);
		HAL_Delay(1000);
	}
	blink_statu_reset();
}

void blink_self_check_pass(void)
{
	blink_statu_reset();
	LEDx_Set(led1, led1_green, on);
	HAL_Delay(500);
	LEDx_Set(led2, led2_green, on);
	HAL_Delay(500);
	LEDx_Set(led3, led3_green, on);
	HAL_Delay(500);
	LEDx_Set(led1, led1_green, on);
	LEDx_Set(led2, led2_green, on);
	LEDx_Set(led3, led3_green, on);
	HAL_Delay(500);
}

void blink_self_check_error(void)
{
	blink_statu_reset();
	while(on == error_self_check_mark) {
		LEDx_Set(led1, led1_red, on);
		LEDx_Set(led2, led2_red, on);
		LEDx_Set(led3, led3_red, on);
	}
	blink_statu_reset();
}


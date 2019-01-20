/*
 * blink.c
 *
 *  Created on: Jan 19, 2019
 *      Author: QUAN
 */
#include "blink.h"


void blink_init(void)
{
	LED_Green_RCC(1);
	LED_Orange_RCC(1);
	LED_Red_RCC(1);
	LED_Blue_RCC(1);

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin   = LED_Green_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_Green_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LED_Green_Port, LED_Green_Pin, GPIO_PIN_SET);

	GPIO_InitStruct.Pin   = LED_Orange_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_Orange_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LED_Orange_Port, LED_Orange_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : LED4_Pin */
	GPIO_InitStruct.Pin   = LED_Red_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_Red_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LED_Red_Port, LED_Red_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : LED1_Pin */
	GPIO_InitStruct.Pin   = LED_Blue_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_Blue_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LED_Blue_Port, LED_Blue_Pin, GPIO_PIN_SET);
}

status_enum gpiox_on_off(led_color_enum ledx, led_state_enum state)
{
	if(green == ledx) {

		HAL_GPIO_WritePin(LED_Green_Port, LED_Green_Pin, state);

	} else if(orange == ledx) {
		HAL_GPIO_WritePin(LED_Orange_Port, LED_Orange_Pin, state);

	} else if (red == ledx) {
		HAL_GPIO_WritePin(LED_Red_Port, LED_Red_Pin, state);

	} else if (blue == ledx) {
		HAL_GPIO_WritePin(LED_Blue_Port, LED_Blue_Pin, state);

	} else {
		return error;
	}
	return success;
}

status_enum gpiox_blink(led_color_enum ledx, unsigned int microSeconds)
{
	if(green == ledx) {
		HAL_GPIO_WritePin(LED_Green_Port, LED_Green_Pin, GPIO_PIN_SET);
		HAL_Delay(microSeconds);
		HAL_GPIO_WritePin(LED_Green_Port, LED_Green_Pin, GPIO_PIN_RESET);
		HAL_Delay(microSeconds);

	} else if(orange == ledx) {
		HAL_GPIO_WritePin(LED_Orange_Port, LED_Orange_Pin, GPIO_PIN_SET);
		HAL_Delay(microSeconds);
		HAL_GPIO_WritePin(LED_Orange_Port, LED_Orange_Pin, GPIO_PIN_RESET);
		HAL_Delay(microSeconds);

	} else if (red == ledx) {
		HAL_GPIO_WritePin(LED_Red_Port, LED_Red_Pin, GPIO_PIN_SET);
		HAL_Delay(microSeconds);
		HAL_GPIO_WritePin(LED_Red_Port, LED_Red_Pin, GPIO_PIN_RESET);
		HAL_Delay(microSeconds);

	} else if (blue == ledx) {
		HAL_GPIO_WritePin(LED_Blue_Port, LED_Blue_Pin, GPIO_PIN_SET);
		HAL_Delay(microSeconds);
		HAL_GPIO_WritePin(LED_Blue_Port, LED_Blue_Pin, GPIO_PIN_RESET);
		HAL_Delay(microSeconds);

	} else {
		return error;
	}
	return success;
}

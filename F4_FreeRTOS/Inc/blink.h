/*
 * blink.h
 *
 *  Created on: Jan 19, 2019
 *      Author: QUAN
 */

#ifndef BLINK_H_
#define BLINK_H_

#include "main.h"
#include "cmd.h"


typedef enum {
	off = 1,
	on 	= 0
}led_state_enum;

typedef enum {
	red 	= 0,
	orange 	= 1,
	blue 	= 2,
	green 	= 3

} led_color_enum;

#define LED_Green_Pin 			GPIO_PIN_6
#define LED_Green_Port 			GPIOG

#define LED_Orange_Pin 			GPIO_PIN_4
#define LED_Orange_Port 		GPIOD

#define LED_Red_Pin 			GPIO_PIN_5
#define LED_Red_Port 			GPIOD

#define LED_Blue_Pin 			GPIO_PIN_3
#define LED_Blue_Port 			GPIOK

#define LED_Green_RCC(x)		do {											\
										if(1 == x) {							\
											  __HAL_RCC_GPIOG_CLK_ENABLE();		\
										} 										\
								} while(0)

#define LED_Orange_RCC(x)		do {											\
										if(1 == x) {							\
										__HAL_RCC_GPIOD_CLK_ENABLE();			\
										}										\
								} while(0)

#define LED_Red_RCC(x)			do {											\
										if(1 == x) {							\
										__HAL_RCC_GPIOD_CLK_ENABLE();			\
									}											\
								} while(0)

#define LED_Blue_RCC(x)		do {												\
									if(1 == x) {								\
									__HAL_RCC_GPIOK_CLK_ENABLE();				\
									}											\
							} while(0)

void blink_init(void);
status_enum gpiox_on_off(led_color_enum ledx, led_state_enum state);
status_enum gpiox_blink(led_color_enum ledx, unsigned int microSeconds);

#endif /* BLINK_H_ */

/*
 * led_blink.h
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#ifndef LED_BLINK_H_
#define LED_BLINK_H_

#include "main.h"
#include "adc.h"
#include "key.h"
#include "tim.h"

typedef enum {
	led1 = 0, led2 = 1, led3 = 2
} led_list;

typedef enum {
	led1_blue = 0,
	led1_green = 1,
	led1_red = 2,
	led2_blue = 3,
	led2_green = 4,
	led2_red = 5,
	led3_blue = 6,
	led3_green = 7,
	led3_red = 8
} led_color;

#define LED1_BLUE_Pin 						GPIO_PIN_5
#define LED1_GREEN_Pin 						GPIO_PIN_4
#define LED1_RED_Pin 						GPIO_PIN_3
#define LED1_PORT 							GPIOA

#define LED2_BLUE_Pin 						GPIO_PIN_13
#define LED2_GREEN_Pin 						GPIO_PIN_12
#define LED2_RED_Pin 						GPIO_PIN_14
#define LED2_PORT 							GPIOB

#define LED3_BLUE_TIM1_CH1_Pin 				GPIO_PIN_8
#define LED3_GREEN_TIM1_CH2_Pin 			GPIO_PIN_9
#define LED3_RED_TIM1_CH3_Pin 				GPIO_PIN_10
#define LED3_PORT 							GPIOA

void
LEDx_Set (led_list LEDx, led_color color, gpio_status status);

void
blink_init (void);
void
blink_standby (void);
void
blink_runing (void);
void
blink_overheat (void);
void
blink_self_check_pass (void);
void
blink_self_check_error (void);
void
blink_pwr_voltage_error (void);












#endif /* LED_BLINK_H_ */

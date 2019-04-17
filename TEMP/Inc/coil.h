/*
 * coil.h
 *
 *  Created on: Apr 16, 2019
 *      Author: QUAN
 */

#ifndef COIL_H_
#define COIL_H_

#include "main.h"
#include "global_cmd.h"
#include "led_blink.h"

#define COIL_CTRL_PIN			GPIO_PIN_6
#define COIL_CTRL_PORT 			GPIOA



void coil_init(void);
void coil_power(gpio_status status);
void coil_keep_warm(gpio_status status);

#endif /* COIL_H_ */

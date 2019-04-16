/*
 * key.h
 *
 *  Created on: Apr 3, 2019
 *      Author: shush
 */

#ifndef KEY_H_
#define KEY_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "led_blink.h"
#include "global_cmd.h"

#define TRIGGER2_Pin 			GPIO_PIN_8
#define TRIGGER1_Pin 			GPIO_PIN_11
#define TRIGGER_PORT 			GPIOB

void key_init(void);

void key_nvic_set(void);
void enable_key_trigger(void);

#ifdef __cplusplus
}
#endif

#endif /* KEY_H_ */

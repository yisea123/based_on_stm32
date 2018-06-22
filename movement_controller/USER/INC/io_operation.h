/**
 * 
 */
#ifndef __IO_OPERATION_H
#define __IO_OPERATION_H

#include "system_stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "delay.h"
#include "command.h"

#define _IO_INPUT_NUMBER                (uint8_t)0x06
#define _IO_OUTPUT_NUMBER               (uint8_t)0x06
/*-------------------- Operate IO define -----------------------*/
#define _IO_PORTC               GPIOC
#define _IO_PORTD               GPIOD
/*-------------------- Output IO define -----------------------*/
#define _IO_OUTPUT_PIN_1                GPIO_Pin_0
#define _IO_OUTPUT_PIN_2                GPIO_Pin_1
#define _IO_OUTPUT_PIN_3                GPIO_Pin_2
#define _IO_OUTPUT_PIN_4                GPIO_Pin_3
#define _IO_OUTPUT_PIN_5                GPIO_Pin_4
#define _IO_OUTPUT_PIN_6                GPIO_Pin_5

/*-------------------- Input IO define -----------------------*/
#define _IO_INPUT_PIN_1                GPIO_Pin_9
#define _IO_INPUT_PIN_2                GPIO_Pin_10
#define _IO_INPUT_PIN_3                GPIO_Pin_11
#define _IO_INPUT_PIN_4                GPIO_Pin_12
#define _IO_INPUT_PIN_5                GPIO_Pin_13
#define _IO_INPUT_PIN_6                GPIO_Pin_2

// #define _IO_INPUT_PIN_(x)       ((x) == (_IO_INPUT_PIN_1)|\
//                                 (x) == (_IO_INPUT_PIN_2) |\
//                                 (x) == (_IO_INPUT_PIN_3) |\
//                                 (x) == (_IO_INPUT_PIN_4) |\
//                                 (x) == (_IO_INPUT_PIN_5) |\
//                                 (x) == (_IO_INPUT_PIN_6))

void _IO_PortInit(void);
uint8_t ReadSinglePin(uint8_t pinNumber);
ErrorStatus WriteSinglePin(uint8_t write_Pin, uint8_t writeValue);
uint8_t ReadMultiPin(uint8_t startLabel, uint8_t endLabel);
ErrorStatus WriteMultiPin(uint8_t startLabel, uint8_t endLabel, uint8_t _IO_Param);

#endif


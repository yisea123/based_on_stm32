/*
 * usart.h
 *
 *  Created on: Jan 20, 2019
 *      Author: QUAN
 */

#ifndef USART_H_
#define USART_H_

#include "main.h"
#include "cmsis_os.h"

#define USART3_TX_Pin 		GPIO_PIN_10
#define USART3_RX_Pin 		GPIO_PIN_11
#define USART3_Port 		GPIOB


#define USART6_TX_Pin 		GPIO_PIN_14
#define USART6_RX_Pin 		GPIO_PIN_9
#define USART6_Port 		GPIOG



extern UART_HandleTypeDef 			huart3;
extern UART_HandleTypeDef 			huart6;

void MX_USARTx_Init(USART_TypeDef * USARTx, uint32_t baudrate);

int uasrtx_send_string(UART_HandleTypeDef * huart, uint8_t *str, uint16_t len);

#endif /* USART_H_ */

/**
  ******************************************************************************
  * @file    usart.h
  * @author  jiangquan
  * @version V1.0
  * @date    30-Oct.-2017
  * @brief   the file contains the headers about usart.c file.
  ******************************************************************************
*/

#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"      // error status
#include "stdio.h"
#include "stm32f10x_usart.h"
#include "string.h"
#include "delay.h"
#include "modbus.h"
#include "command.h"

#define   USART_BUFF_SIZE     60
extern const uint8_t endMark[3];

#define 	USART1_TX			GPIO_Pin_6
#define 	USART1_RX			GPIO_Pin_7

#define 	USART3_TX			GPIO_Pin_10
#define		USART3_RX			GPIO_Pin_11

void USARTx_Configuration(USART_TypeDef * USARTx, uint32_t baudRate);

#endif

/*********************************** END LINE **************************************/


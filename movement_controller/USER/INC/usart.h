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



typedef struct{
    uint8_t Usart1Buff[USART_BUFF_SIZE];
    uint8_t Usart2Buff[USART_BUFF_SIZE];
    uint8_t Usart3Buff[USART_BUFF_SIZE];
    uint8_t usart1RecLen;
    uint8_t usart2RecLen;
    uint8_t usart3RecLen;
}UsartBuffTypeDef;


typedef struct {
    ErrorStatus usart1;
    ErrorStatus usart2;
    ErrorStatus usart3;
    FlagStatus  usart1RecOver;
    FlagStatus  usart2RecOver;
    FlagStatus  usart3RecOver;
    
}UsartxRecFlag;
extern UsartBuffTypeDef usartBuff;
extern uint8_t usartSendBuff[USART_BUFF_SIZE];
extern UsartxRecFlag usartxRecFlag;


void USARTx_Configuration(USART_TypeDef * USARTx, uint32_t baudRate);

#endif

/*********************************** END LINE **************************************/


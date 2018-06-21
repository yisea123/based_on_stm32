// #ifndef __USART_H
// #define __USART_H
// #include "STM32F10x_CONF.h"
// #include "stdio.h"
// #include "stm32f10x_conf.h"
// #include "string.h"
// #include "ctype.h"



// #define BUFF_SIZE  60
// //#define yBUFF_SIZE	60
// #define Usart_Baund 115200

// extern uint8_t Usart_RX_BUFF[BUFF_SIZE];
// extern uint8_t Usart_TX_BUFF[BUFF_SIZE];
// extern uint8_t const *END_Mark;
// typedef struct
// {
//     uint8_t Rx_BusyStatus;
//     uint8_t Rx_DataCount;
//     uint8_t Rx_RecSize_X;
//     uint8_t Rx_RecSize_Y;

// }Usart_RX_Status_InitTypedef;

// typedef struct 
// {
// 		uint8_t Tx_BusyStatus;
// 		uint8_t Tx_RecSize_X;
// 		uint8_t Tx_RecSize_Y;
		
// }Usart_TX_Status_InitTypedef;

// void Usart_GPIO_Config(void);
// void Usart_Init(void);

// uint8_t UsartRecByte(void);
// uint8_t UsartSendByte(uint8_t byte);
// uint8_t UsartSendString(uint8_t *data);

// uint8_t RecDataAnalys(uint8_t *arry);

// void DataAnalys(uint8_t *arry);

// void protical_go(uint8_t code);

// void Usart_SendString(uint8_t *string);

// #endif

#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x_conf.h"
#include <stdio.h>

//#define USER_USART1_IRQ

//extern unsigned char Receive_Complete;

// void USART_SET(void);
// void USART_SendString(char *nbuff);

typedef struct Usart_BuffInit
{
	uint8_t Usart_SendBuff[20];
	uint8_t Usart_RecBuff[20];
}Usart_Buff;

extern uint8_t counter;




void USART_GPIO_conf(void);
void USART_conf(void);
// void Usart_SendString(Usart_BuffInit *pBuff, uint8_t length);


#endif

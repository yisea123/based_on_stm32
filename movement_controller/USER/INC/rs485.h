#ifndef __RS485_H
#define __RS485_H

#include "usart.h"

extern uint8_t rs485_RecBuff[USART_BUFF_SIZE];
extern uint8_t *pRS485_RecStart;
extern uint8_t *pRS485_RecEnd;
extern uint8_t rs485_Counter;

void RS485_TestProtocol(void);
ErrorStatus RS485_SendByte(uint8_t byte);
ErrorStatus RS485_SendString(uint8_t rs485_RecBuff[]);
ErrorStatus RS485_RecData(void);
#endif


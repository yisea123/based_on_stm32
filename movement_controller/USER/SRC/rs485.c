#include "rs485.h"

uint8_t rs485_RecBuff[USART_BUFF_SIZE] = {'\0'};
uint8_t *pRS485_RecStart               = NULL;
uint8_t *pRS485_RecEnd                 = NULL;
uint8_t rs485_Counter                  = 0;


void RS485_TestProtocol(void)
{
    for (uint8_t i = 0; i < 60; i++)
    {
        rs485_RecBuff[i] = i * sin(i);
    }
    RS485_SendString(rs485_RecBuff);
    DelayMs(2000);
}
ErrorStatus RS485_SendByte(uint8_t byte)
{
    USART3 ->DR = byte;
    return SUCCESS;
}
ErrorStatus RS485_SendString(uint8_t rs485_RecBuff[])
{
    uint8_t rs485SendCounter = 0;
    while ('\0' != rs485_RecBuff[rs485SendCounter])
    {
        USART3 ->DR = (uint16_t)rs485_RecBuff[rs485SendCounter];
        rs485SendCounter++;
    }
    return SUCCESS;
}
ErrorStatus RS485_RecData(void)
{
    rs485_RecBuff[rs485_Counter] = USART_ReceiveData(USART3);
    rs485_Counter++;
    pRS485_RecEnd = &rs485_RecBuff[rs485_Counter];
    return SUCCESS;
}





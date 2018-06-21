//#include "stdint.h"
//#include "stm32f10x.h"
//#include "INIT.H"
#ifndef __uart_H__
#define __uart_H__
#define UART1 1
#define UART2 2
#define UART3 3
extern uint8_t receiveover1;
extern uint8_t ReceLen1;
extern uint8_t ReceBUFF1[100];
extern uint8_t ReceBUFF1Pre[100];
extern uint8_t SendBuff1[100];
extern uint8_t ReceLenCount1;
extern u8 UART_BUSY_FLAG;
//uart2
extern uint8_t receiveover2;
extern uint8_t ReceLen2;
extern uint8_t ReceBUFF2[100];
extern uint8_t SendBuff2[100];
extern uint8_t ReceLenCount;
//uart3
extern uint8_t receiveover3;
extern uint8_t ReceLen3;
extern uint8_t ReceLen3Save;
extern uint8_t ReceBUFF3Pre[100];
extern uint8_t ReceBUFF3[100];
extern uint8_t SendBuff3[100];
extern uint8_t SendLen3;
extern uint8_t SendBuff3[100];
extern uint8_t UartOutPut;

extern 	uint8_t wrBuf[5]; 

//#endif
//#ifdef UART_GLOBALS
void Uart1Init(u32 Baud);
void Uart2Init(u32 Baud);
void Uart3Init(u32 Baud);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void UartSend1Byte(USART_TypeDef* USARTx,u8 SendData);
void UartSend2Byte(USART_TypeDef* USARTx,u16 SendData);
void USARTTran_ArrayData(USART_TypeDef* USARTx,uint8_t *point);
void USARTTranlen(USART_TypeDef* USARTx,uint8_t *point,uint len);
void SendStrings(uchar *str);
void UART1_Operation_function(void);
//void USART1_printf ( char *fmt, ...);
//#else
extern void Uart1Init(u32 Baud);
extern void Uart2Init(u32 Baud);
extern void Uart3Init(u32 Baud);
extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);
extern void USART3_IRQHandler(void);
extern void UartSend1Byte(USART_TypeDef* USARTx,u8 SendData);
extern void UartSend2Byte(USART_TypeDef* USARTx,u16 SendData);
extern void USARTTran_ArrayData(USART_TypeDef* USARTx,uint8_t *point);
extern void USARTTranlen(USART_TypeDef* USARTx,uint8_t *point,uint len);
extern void convertLongtoChar(ulong number);
extern void SendStrings(uchar *str);
extern void UART1_Operation_function(void);
//extern void USART1_printf ( char *fmt, ...);
#endif

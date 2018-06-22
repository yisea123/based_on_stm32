/**
  ******************************************************************************
  * @file    usart.c

  * @author  jiangquan
  * @version V1.0
  * @date    30-Oct.-2017
  * @brief   the file contain the function about set usart wave
  ******************************************************************************
*/

#include "usart.h"
/********************** extern and struct init **************************/
const uint8_t endMark[3] = {0x0d, 0x0a, '\0'};          //equal to "\r\n"

/***********************************************************************/
/********************* usart suppoert printf function *******************/

#pragma import(__use_no_semihosting)
void _sys_exit(int x)
{
	x = x;
}
struct __FILE
{
    int handle;
};

FILE __stdout;

int fputc(int ch, FILE *f)
{

    USART_SendData(USART1, (uint8_t)  ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {}

    return (ch);
}
/************************************************************************/


/**
 *    [USARTx_Configuration description]
 *
 *    @param  USARTx    USARTx channel need to be configurate
 *            USART1 for RS232, USART3 for RS485.
 *    @param  baudRate  baudrate to be set to usartx
 */

void USARTx_Configuration(USART_TypeDef * USARTx, uint32_t baudRate)
{
    GPIO_InitTypeDef        GPIO_InitStruction;
    USART_InitTypeDef       Usartx_InitStruction;
    NVIC_InitTypeDef        NVIC_InitStruction;

    if (USARTx == USART1)
    {
        USART_DeInit(USART1);
        /*----------------------------- usart1 -----------------------------*/
        /** Pin Remap ---------------------------------------*/
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
        /** usart1 GPIO config ---------------------------------------*/
        GPIO_InitStruction.GPIO_Pin   = USART1_TX;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(PORTB, &GPIO_InitStruction);

        GPIO_InitStruction.GPIO_Pin   = USART1_RX;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(PORTB, &GPIO_InitStruction);

        Usartx_InitStruction.USART_BaudRate            = baudRate;
        Usartx_InitStruction.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
        Usartx_InitStruction.USART_WordLength          = USART_WordLength_8b;
        Usartx_InitStruction.USART_StopBits            = USART_StopBits_2;
        Usartx_InitStruction.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        Usartx_InitStruction.USART_Parity              = USART_Parity_No;              //
        USART_Init(USART1, &Usartx_InitStruction);

        /************** set usart1 NVIC ***************************/
        NVIC_InitStruction.NVIC_IRQChannel                   = USART1_IRQn;
        NVIC_InitStruction.NVIC_IRQChannelPreemptionPriority = 2;
        NVIC_InitStruction.NVIC_IRQChannelSubPriority        = 0;
        NVIC_InitStruction.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStruction);

        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //set usart1 interrupt type  USART_IT_IDLE usart line not busy
        USART_Cmd(USART1, ENABLE);
    }
  
    else if (USARTx == USART3)
    {
        USART_DeInit(USART3);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        /*----------------------------- usart3 -----------------------------*/
        GPIO_InitStruction.GPIO_Pin   = USART3_TX;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(PORTB, &GPIO_InitStruction);

        GPIO_InitStruction.GPIO_Pin   = USART3_RX;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(PORTB, &GPIO_InitStruction);

        Usartx_InitStruction.USART_BaudRate            = baudRate;
        Usartx_InitStruction.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
        Usartx_InitStruction.USART_WordLength          = USART_WordLength_8b;
        Usartx_InitStruction.USART_StopBits            = USART_StopBits_2;
        Usartx_InitStruction.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        Usartx_InitStruction.USART_Parity              = USART_Parity_No;
        USART_Init(USART3, &Usartx_InitStruction);

        /************** set usart3 NVIC ***************************/
        NVIC_InitStruction.NVIC_IRQChannel                   = USART3_IRQn;
        NVIC_InitStruction.NVIC_IRQChannelPreemptionPriority = 2;
        NVIC_InitStruction.NVIC_IRQChannelSubPriority        = 2;
        NVIC_InitStruction.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStruction);

        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  //set usart3 interrupt type
        USART_Cmd(USART3, ENABLE);
    }
    
}
/**
 *    [USART1_IRQHandler description]
 */
void USART1_IRQHandler(void)
{
//    uint8_t temp = 0;
    if (SET == USART_GetFlagStatus(USART1, USART_IT_RXNE)) // received message storage in usart recbuff
    {
//        temp = USART1->DR;
//        USART1->DR = temp;
    }
    USART_ClearFlag(USART1, USART_IT_RXNE);
}

/**
 *    [USART3_IRQHandler description]
 */
void USART3_IRQHandler(void)
{
//    uint8_t temp = 0;
    if (SET == USART_GetITStatus(USART3, USART_IT_RXNE))
    {
//        temp = USART3->DR;
//        USART3->DR = temp;
    }
    USART_ClearFlag(USART3, USART_IT_RXNE);
}
/*------------------------------------- END LINE ------------------------------------------*/

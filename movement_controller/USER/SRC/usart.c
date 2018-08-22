

#include "usart.h"
/********************** extern and struct init **************************/
const uint8_t endMark[3] = {0x0d, 0x0a, '\0'};          //equal to "\r\n"
UsartBuffTypeDef usartBuff;
uint8_t usartSendBuff[USART_BUFF_SIZE];
UsartxRecFlag usartxRecFlag;

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

    static int mmm = 0;
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
        USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
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
    if(0 == mmm)
    {    
        memset((void *)&usartBuff, '\0', sizeof((void *)&usartBuff));
        usartxRecFlag.usart1 = ERROR;
        usartxRecFlag.usart2 = ERROR;
        usartxRecFlag.usart3 = ERROR;
        usartxRecFlag.usart1RecOver = RESET;
        usartxRecFlag.usart2RecOver = RESET;
        usartxRecFlag.usart3RecOver = RESET;
        usartBuff.usart1RecLen = 0;
        usartBuff.usart2RecLen = 0;
        usartBuff.usart3RecLen = 0;
        mmm += 1;
    }
}
/**
 *    [USART1_IRQHandler description]
 */
void USART1_IRQHandler(void)
{
    uint8_t clear = 0;
    if (SET == USART_GetITStatus(USART1, USART_IT_RXNE)) // received message storage in usart recbuff
    {
        usartBuff.Usart1Buff[usartBuff.usart1RecLen++] = USART1->DR;
    }
    else if(SET == USART_GetITStatus(USART1, USART_IT_IDLE)) //set usart3 interrupt type)
    {
        clear = USART1->SR; //clear USART_IT_IDLE
        clear= USART1->DR;  //clear USART_IT_IDLE
        
        usartBuff.usart1RecLen = usartBuff.usart1RecLen -1;     // 减掉最后一位
        
        if(usartBuff.usart1RecLen > 1 && (0x0A == usartBuff.Usart1Buff[usartBuff.usart1RecLen]) && (0x0D == usartBuff.Usart1Buff[usartBuff.usart1RecLen - 1]))
        {

            usartxRecFlag.usart1RecOver = SET;
            printf("usart1 rec: %s", usartBuff.Usart1Buff);
        }
        else
        {
            usartBuff.usart1RecLen = 0;
            memset(usartBuff.Usart1Buff, '\0', USART_BUFF_SIZE);
        }
        usartBuff.usart1RecLen = 0;
        memset(usartBuff.Usart1Buff, '\0', USART_BUFF_SIZE);
        
    }
}

/**
 *    [USART3_IRQHandler description]
 */
void USART3_IRQHandler(void)
{
   uint8_t temp = 0;
    if (SET == USART_GetITStatus(USART3, USART_IT_RXNE))
    {
       temp = USART3->DR;
       USART3->DR = temp;
    }
    USART_ClearFlag(USART3, USART_IT_RXNE);
}
/*------------------------------------- END LINE ------------------------------------------*/

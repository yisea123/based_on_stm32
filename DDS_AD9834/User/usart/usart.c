#include "usart.h"



Usart_RX_Status_InitTypedef Usart_RX_Status;
Usart_TX_Status_InitTypedef Usart_TX_Status;

uint8_t Usart_RX_BUFF[BUFF_SIZE] = {0};
uint8_t Usart_TX_BUFF[BUFF_SIZE] = {0};

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

//Usart_RX_Status.Rx_RecSize_X = 0;
//Usart_RX_Status.Rx_RecSize_Y = 0;
//Usart_TX_Status.Tx_RecSize_X = 0;
//Usart_TX_Status.Tx_RecSize_Y = 0;

//Usart_RX_Status.Rx_DataCount = 0;


//uint8_t const *END_Mark = {"\r\n"};

void Usart_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef Usart_GPIO_Init;

    Usart_GPIO_Init.GPIO_Pin   = GPIO_Pin_9;
    Usart_GPIO_Init.GPIO_Speed = GPIO_Speed_10MHz;
    Usart_GPIO_Init.GPIO_Mode  = GPIO_Mode_AF_PP;
    
    Usart_GPIO_Init.GPIO_Pin   = GPIO_Pin_10;
    Usart_GPIO_Init.GPIO_Speed = GPIO_Speed_10MHz;
    Usart_GPIO_Init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &Usart_GPIO_Init);
}
void Usart_Init(void)
{
    Usart_GPIO_Config();
    
/******************************************Usart NVIC Config*************************************************/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
/******************************************Usart Config*************************************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);      //Open usart1 clock
    
    USART_InitTypeDef Usart_InitStructure;
    Usart_InitStructure.USART_BaudRate            = Usart_Baund;
    Usart_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    Usart_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    Usart_InitStructure.USART_Parity              = USART_Parity_No;
    Usart_InitStructure.USART_StopBits            = USART_StopBits_1;
    Usart_InitStructure.USART_WordLength          = USART_WordLength_8b;
    
    USART_Init(USART1, &Usart_InitStructure);
    
    USART_ITConfig(USART1, USART_FLAG_TXE | USART_IT_RXNE | USART_IT_TC, ENABLE);       //Open send and receive interrupt
    USART_Cmd(USART1, ENABLE);
        
}
//return 1, send string success
void Usart_SendString(uint8_t *string)
{
    while(USART_GetITStatus(USART1, USART_IT_TC) != RESET);
    while(*string != '\0')
    {
        USART_SendData(USART1, *string++);
    }
}
/************************************
*Usart Interrupu function
*Usart Set to 115200bps, StopBit = 1, Communicate stop with "\r\n"
*
*
*************************************/
void USART1_IRQHandlle(void)
{
	if((USART_GetITStatus(USART1, USART_IT_RXNE)) != RESET)     //Received data
    {
        Usart_RX_Status.Rx_BusyStatus = 1;
        if(Usart_RX_Status.Rx_DataCount <= BUFF_SIZE - 2)
        {
            Usart_RX_BUFF[Usart_RX_Status.Rx_DataCount] = USART_ReceiveData(USART1); 
            if(Usart_RX_BUFF[Usart_RX_Status.Rx_DataCount] == '\n')
            {
                if(Usart_RX_BUFF[Usart_RX_Status.Rx_DataCount - 1] == '\r')        //receive succes
                {
                    //clear end mark
                    Usart_RX_BUFF[Usart_RX_Status.Rx_DataCount] = 0;
                    Usart_RX_BUFF[Usart_RX_Status.Rx_DataCount - 1] = 0;

                    Usart_RX_Status.Rx_DataCount = 0;
                    Usart_RX_Status.Rx_BusyStatus = 0;

                    //Usart_SendString(systemack);        //data accepted   
                }
                else        //Receive data error
                {
                    memset(Usart_RX_BUFF, '\0', BUFF_SIZE);
                    Usart_RX_Status.Rx_DataCount = 0; 
                    Usart_RX_Status.Rx_BusyStatus = 0;
                }
            }
            else
            {
                Usart_RX_BUFF[Usart_RX_Status.Rx_DataCount] = USART_ReceiveData(USART1);
                Usart_RX_Status.Rx_DataCount++;
            }

        }
        else        //  Receive data over temp array
        {
            //Usart_RX_Status.RX_Status = 1;
            memset(Usart_RX_BUFF,'\0',BUFF_SIZE);
            Usart_RX_Status.Rx_DataCount = 0;
        }
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);      //clearn RX interrupt bit
//        USART_ClearFlag(USART1, USART_IT_RXNE);     //clear flage status, 读取后自动清除标志，可以不加
        //Usart_RX_Status.RX_Status = 1;
    }
//    if((USART_GetFlagStatus(USART1, USART_FLAG_TXE)) != RESET)    //Send completed. Send regeister empty
//    {
//        Usart_RX_Status.TX_Status = 1;
//        USART_ClearITPendingBit(USART1, USART_FLAG_TC);
//        Usart_RX_Status.TX_Status = 0;
//    }
    
}


/******************printf fuction support***************/
#if 1
#pragma import(__use_no_semihosting)    //标准库需要的支持函数
struct __FILE
{
    int handle;
}
FILE_stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}
#endif
/*******************************************************/






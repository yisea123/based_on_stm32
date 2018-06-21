#include "usart.h"


uint8_t counter = 0;


/********************************* can use ********************************************/
// #include "stm32f10x_conf.h"
// #include "usart.h"

// uint8_t timer = 0;
//  int main(void)
//  {  
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//     USART_conf();
//     USART_GPIO_conf();
//     LED_Conf();
//     while(1)
//      {
//         if(timer > 10)
//         {
//             LED_Digital_True;
//             Delay(5000);
//             LED_Digital_False;
//             timer = 0;
            
//         }
//      }
     
//  }
void USART_GPIO_conf(void)
{
    GPIO_InitTypeDef    GPIO_Struct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Struct.GPIO_Pin = GPIO_Pin_9;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA,&GPIO_Struct);

    GPIO_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Struct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA,&GPIO_Struct);
    
    
}
void USART_conf(void)
{
    USART_InitTypeDef   USART_Struct;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /****************************** USART配置*********************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);   
    USART_DeInit(USART1);   

    USART_Struct.USART_BaudRate = 115200;
    USART_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_Struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    
    USART_Struct.USART_Parity = USART_Parity_No;    
    USART_Struct.USART_StopBits = USART_StopBits_1;     
    USART_Struct.USART_WordLength = USART_WordLength_8b;    
    
    USART_Init(USART1,&USART_Struct);      
    USART_Cmd(USART1, ENABLE);     
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        //开启中断,开启接收中断
    // USART_ITConfig(USART1, USART_IT_RXNE | USART_IT_TXE, ENABLE);        //开启中断,开启接收中断
    
/************************** Init usart interrupt  ***************************/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;           //串口1中断
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     //IRQ通道使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;             
    
    NVIC_Init(&NVIC_InitStructure);                             
    
}
void USART1_IRQHandler(void)
{

    Usart_Buff *pBuff;

    if(USART_GetITStatus(USART1, USART_IT_RXNE))    //Received interrupt
    {
        pBuff -> Usart_RecBuff[counter] = USART_ReceiveData(USART1);       //接收到数据保存到变量
        counter++;
//        USART_SendData(USART1, pBuff -> Usart_RecBuff[counter]);              //send test
    }
}



// void USART1_IRQHandler(void)
// {
//     uint8_t Rec = 0;
//     typedef struct Usart_BUffInit *pBuff;

//     if(USART_GetITStatus(USART1, USART_IT_RXNE))    //Received interrupt
//     {
//         Rec = USART_ReceiveData(USART1);       //接收到数据保存到变量

//     }
    // else if(USART_GetITStatus(USART1, USART_IT_TXE))
    // {
    //     Usart_SendString(); 
    // }
// }

// void Usart_SendString(Usart_BuffInit *pBuff, uint8_t length)
// {

// }
/*************************** usart suppoert printf function *****************************/

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

  USART_SendData(USART1,(uint8_t)  ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {}

  return (ch);
}
/***************************************************************************************/

/************************************** END LINE ***************************************/


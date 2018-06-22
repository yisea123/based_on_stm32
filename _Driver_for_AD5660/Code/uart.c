//#define UART_GLOBALS
//#include "GlobalDef.h"
//#include <stdarg.h>
#include "config.h"
uint8_t receiveover1=0;
uint8_t ReceLen1=0;
uint8_t ReceBUFF1[100];
uint8_t ReceBUFF1Pre[100];
uint8_t SendBuff1[100];
uint8_t ReceLenCount1=0;
u8 UART_BUSY_FLAG;
//uart2
uint8_t receiveover2=0;
uint8_t ReceLen2=0;
uint8_t ReceBUFF2[100];
uint8_t SendBuff2[100];
uint8_t ReceLenCount2=0;
//uart3
uint8_t receiveover3=0;
uint8_t ReceLen3=0;
uint8_t ReceLen3Save=0;
uint8_t ReceBUFF3[100]= {0x01,0x02,0x03,0x04,0x05,0x06,0x07};
uint8_t ReceBUFF3Pre[100]= {0x01,0x02,0x03,0x04,0x05,0x06,0x07};
uint8_t SendLen3=0;
uint8_t SendBuff3[100];
uint8_t UartOutPut;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
//extern GPIO_InitTypeDef GPIO_InitStructure;
//extern USART_InitTypeDef USART_InitStructure;
//extern GPIO_InitTypeDef  GPIO_InitStructure;
//USART_InitTypeDef USART_InitStructure;
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
    x = x;
}
//重定义fputc函数 //即 printf函数
int fputc(int ch, FILE *f)
{
    if(UartOutPut==UART1)
    {
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
        {
            ;
        }
        USART_SendData(USART1,(uint8_t)ch);
    }
    else if(UartOutPut==UART2)
    {
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
        {
            ;
        }
        USART_SendData(USART2,(uint8_t)ch);
    }
    else if(UartOutPut==UART3)
    {
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET)
        {
            ;
        }
//        PrvLCDBuffDeal[SendLen3]=(uint8_t)ch;
        SendLen3++;// USART_SendData(USART3,(uint8_t)ch);
    }
    else
    {
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
        {
            ;
        }
        USART_SendData(USART1,(uint8_t)ch);
    }
//    Delayus(20);
    return ch;
}
#endif
//#define CMD_BUFFER_LEN 30
//void USART1_printf ( char *fmt, ...)
// {
//   char buffer[CMD_BUFFER_LEN+1];  // CMD_BUFFER_LEN???????
//   u8 i = 0;
//   va_list arg_ptr;
//   va_start(arg_ptr, fmt);
//   vsnprintf(buffer, CMD_BUFFER_LEN+1, fmt, arg_ptr);
//   while ((i < CMD_BUFFER_LEN) && buffer)
//   {
//                   USART_SendData(USART1, (u8) buffer[i++]);
//                   while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//   }
//   va_end(arg_ptr);
// }
/************************
功能:UART1 INIT
版本:V1.00   日期:11.08.07
修改:xiaodong.liuxiaodong.liuxiaodong.liu
作者:value
************************/
void Uart1Init(u32 Baud)
{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟
    USART_DeInit(USART1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
//  #if EN_USART1_RX
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//   #endif
    USART_Cmd(USART1, ENABLE);
}
/************************
功能:UART2 INIT
版本:V1.00   日期:11.08.07
修改:xiaodong.liuxiaodong.liu
作者:value
************************/
void Uart2Init(u32 Baud)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //注意UART2与UART1时钟不一样
    //  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    //Configure USART2 Tx (PA2) as alternate function push-pull
    //不定义引脚方向则导致可以从ISP 下载完成后“开跑”运行，但重上电运行没反应
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure USART2 Rx (PA3) as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_DeInit(USART2);
    USART_InitStructure.USART_BaudRate =Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //enable usart2 receive interrupt
    USART_Cmd(USART2, ENABLE);
}
/************************
功能:UART3 INIT
版本:V1.00   日期:11.10.06
修改:xiaodong.liuxiaodong.liu
作者:value
************************/
void Uart3Init(u32 Baud)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //注意UART3与UART1时钟不一样
    //Configure USART3 Tx (PB10) as alternate function push-pull
    //不定义引脚方向则导致可以从ISP 下载完成后“开跑”运行，但重上电运行没反应
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //Configure USART3 Rx (PB11) as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_DeInit(USART3);
    USART_InitStructure.USART_BaudRate =Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //enable usart3 receive interrupt
    USART_Cmd(USART3, ENABLE);
}
/************************
功能:usart1 receive interrupt dell
版本:V1.00   日期:11.10.06
修改:xiaodong.liuxiaodong.liu
作者:value
************************/
void USART1_IRQHandler(void)
{
    uint8_t i;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
    {
//          if(UART_BUSY_FLAG==1)USARTTran_ArrayData(USART1,"BUSY!\r");
        i=(USART_ReceiveData(USART1));//& 0x7F; //一定要转化为7为&0x7f
        USART_ClearFlag(USART1, USART_IT_RXNE);     //UART1_RXNEF = 0;
        //UartSend1Byte(USART1, 0x33);  ///
        if(receiveover1==0)
        {
            ReceBUFF1Pre[ReceLen1]=i;
            ReceLen1++;
            ReceLenCount1++;
            if(i=='\r')
            {
                receiveover1=1;   //\\r=0x0d,n=0x0a,
            }
        }
        //UartSend1Byte(USART1,i);
    }
}
/************************
功能:usart2 receive interrupt dell
版本:V1.00   日期:11.10.06
修改:xiaodong.liuxiaodong.liu
作者:value
************************/
void USART2_IRQHandler(void)
{
    uint8_t i;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
    {
        i=(USART_ReceiveData(USART2));//& 0x7F; //一定要转化为7为&0x7f
//      UartSend1Byte(USART2, 0x31);  ///
//          UartSend1Byte(USART2,i);
        if(receiveover2==0)
        {
            ReceBUFF2[ReceLen2]=i;
            if(ReceBUFF2[0]=='S')
            {
//                   UartSend1Byte(USART2, 0x32);  ///
//               UartSend1Byte(USART2,i);
                ReceBUFF2[ReceLen2]=i;
                ReceLen2++;
                ReceLenCount2++;
                if(ReceLen2==2)
                {
                    receiveover2=1;   //\\r=0x0d,n=0x0a,
                }
            }
        }
        USART_ClearFlag(USART2, USART_IT_RXNE);     //UART2_RXNEF = 0;
        //UartSend1Byte(USART2,i);
    }
}
void USART3_IRQHandler(void)
{
    uint8_t i;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收中断
    {
        i=(USART_ReceiveData(USART3));//& 0x7F; //一定要转化为7为&0x7f
        //  UartSend1Byte(USART1, 0x33);  ///
        if(receiveover3==0)
        {
            ReceBUFF3Pre[ReceLen3]=i;
            if(ReceBUFF3Pre[0]==0xEE)
            {
                // if(i==0xFC){receiveover3=1;ReceBUFF3[0]=0;}
                if(ReceLen3>4)
                {
                    if((ReceBUFF3Pre[ReceLen3]==0xFF)&&(ReceBUFF3Pre[ReceLen3-1]==0xFF)&&(ReceBUFF3Pre[ReceLen3-2]==0xFC)&&(ReceBUFF3Pre[ReceLen3-3]==0xFF))//收到FF FC FF FF表示一条命令接收完
                    {
                        receiveover3=1;    //\\r=0x0d,n=0x0a,
                    }
                }
                ReceLen3++;
//            ReceLenCount3++;
            }
        }
        USART_ClearFlag(USART3, USART_IT_RXNE);     //UART3_RXNEF = 0;
        //UartSend1Byte(USART3,i);
    }
}
/************************
功能:UART send one byte
版本:V1.00   日期:11.08.07
修改:xiaodong.liu
作者:value
************************/
void UartSend1Byte(USART_TypeDef* USARTx,u8 SendData)
{
    if(USARTx==USART2)
    {
//        EN_T485;
        Delayus(10);
    }
    USART_ClearFlag(USARTx,USART_FLAG_TC);    //清楚发送完成标志
    USART_SendData(USARTx,SendData);
    while(!USART_GetFlagStatus(USARTx,USART_FLAG_TC))
    {
        ;    //等待发送标志完成
    }
//   EN_R485;
}
/************************
功能:UART send two byte
版本:V1.00   日期:14.04.05
修改:xiaodong.liu
作者:xiaodong.liu
************************/
void UartSend2Byte(USART_TypeDef* USARTx,u16 SendData)
{
    UartSend1Byte(USARTx,SendData>>8);
    UartSend1Byte(USARTx,SendData&0XFF);
}
/************************
功能:只适合以'/'为结束符的发送  定义字符时最后以'/0'结束
版本:V1.00   日期:11.08.08
修改:xiaodong.liu
作者:value
************************/
void USARTTran_ArrayData(USART_TypeDef* USARTx,uint8_t *point)
{
    u8 *p=point;
    if(USARTx==USART2)
    {
//        EN_T485;
        Delayus(10);
    }
    while(*p!='\0')//字符数组结束符
    {
        UartSend1Byte(USARTx,*p);
        p++;
    }
//   EN_R485;
}
/************************
功能:发送固定长度的数据
版本:V1.00   日期:13.08.08
修改:xiaodong.liu
作者:xiaodong.liu
************************/
//void USARTTranlen(USART_TypeDef* USARTx,uint8_t *point,uint len)
//{
//    u8 *p=point;
//    if(USARTx==USART2)
//    {
//        EN_T485;
//        Delayus(10);
//    }
//    while(len)//字符数组结束符
//    {
//        UartSend1Byte(USARTx,*p);
//        p++;
//        len--;
//    }
//    EN_R485;
//}
/************************
功能:发送字符串
版本:V1.00   日期:13.08.08
修改:xiaodong.liu
作者:xiaodong.liu
************************/
void SendStrings(uchar *str)
{
    while(*str)
    {
        UartSend1Byte(USART2,*str);
        str++;
    }
}
//=========================================================
//===============================================================================
u8 MY_String_Compare(unsigned char *Command,unsigned char *Command1)
{
    unsigned char i=0, flag=0;
    while( (*Command) != '\0')                              //??????????,???????
    {
        if((*Command)!=(*Command1))
        {
            flag=1;    //?????????,?????,??flag=1
            break;
        }
        else
        {
            Command++;         //????????,??????????
            Command1++;
            i++;
            flag=0;
        }
    }
    if((*Command) == '\0')                                      //?????????
    {
        if(((*Command1) == '\0')||((*Command1) == '\n')||((*Command1) == '\r'))
        {
            flag=0;
        }
        else
        {
            flag=1;
        }
    }
    return flag;
}
void UART1_Operation_function(void)
{
    u8 i=0;
    float Vx=0.0;
//  float Vref=0.0;
    if(receiveover1==1)     //????????
    {
        receiveover1 = 0;
        printf("Command=%s\r\n",ReceBUFF1Pre);
        if( MY_String_Compare("AD",ReceBUFF1Pre)==0 )
        {
            Get_ADC_Reault(0x8016,0x0320);
        }
//========================================================================================power 		
		else if( MY_String_Compare("power1V8 on",ReceBUFF1Pre)==0 )
        {
            Power1V8_On;
			printf("power1V8 on pass\r\n");
        }
		else if( MY_String_Compare("power1V8 off",ReceBUFF1Pre)==0 )
        {
            Power1V8_Off;
			printf("power1V8 off pass\r\n");
        }	
		else if( MY_String_Compare("Power3V3 On",ReceBUFF1Pre)==0 )
        {
            Power3V3_On;
			printf("power3V3 on pass\r\n");
        }
		else if( MY_String_Compare("power3V3 off",ReceBUFF1Pre)==0 )
        {
            Power3V3_Off;
			printf("power3V3 off pass\r\n");
        }

//==================================================================================test		
		else if( MY_String_Compare("uA_test_200",ReceBUFF1Pre)==0 )
        {
			Vx = CurrentTest(1,200);
			printf("Current=%fmA\r\n",Vx);
        }
		else if( MY_String_Compare("uA_test_100",ReceBUFF1Pre)==0 )
        {
			Vx = CurrentTest(1,100);
			printf("Current=%fmA\r\n",Vx);
        }
		else if( MY_String_Compare("uA_test_50",ReceBUFF1Pre)==0 )
        {
			Vx = CurrentTest(1,50);
			printf("Current=%fmA\r\n",Vx);
        }
		else if( MY_String_Compare("uA_test_25",ReceBUFF1Pre)==0 )
        {
			Vx = CurrentTest(1,25);
			printf("Current=%fmA\r\n",Vx);
        }
//================================================================================================		
		else if( MY_String_Compare("Current0",ReceBUFF1Pre)==0 )
        {	
			AD5660_Out(2500);
			printf("Current1\r\n");
        }
		else if( MY_String_Compare("Current1",ReceBUFF1Pre)==0 )
        {	
			AD5660_Out(2000);
			printf("Current1\r\n");
        }
		else if( MY_String_Compare("Current2",ReceBUFF1Pre)==0 )
        {	
			AD5660_Out(1000);
			printf("Current2\r\n");
        }
		else if( MY_String_Compare("Current3",ReceBUFF1Pre)==0 )
        {	
			AD5660_Out(500);
			printf("Current3\r\n");
        }
        else if( MY_String_Compare("Current4",ReceBUFF1Pre)==0 )
        {	
			AD5660_Out(200);
			printf("Current3\r\n");
        }
        else if( MY_String_Compare("Current5",ReceBUFF1Pre)==0 )
        {	
			AD5660_Out(100);
			printf("Current3\r\n");
        }
		

		
		
//====================================================================================================		
        else
        {
            printf("Command_ERR\r\n");
        }
        for(i=0; i<=ReceLen1; i++)
        {
            ReceBUFF1Pre[i]='\0';
            
        }
		ReceLen1 = 0;
    }
}

//#define UART_GLOBALS
//#include "GlobalDef.h"
//#include <stdarg.h>
#include "config.h"
	uint8_t wrBuf[5]; 

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

/************************/
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

/************************/
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

void SendStrings(uchar *str)
{
    while(*str)
    {
        UartSend1Byte(USART2,*str);
        str++;
    }
}
//=========================================================
unsigned short getNunb(uint8_t *Point)
{
    uint8_t *P1 = Point;
    char P2[5] = {0};
    unsigned short A=0;
    signed char n = 0;
    unsigned short i = 1;
    while(*P1!='\0')
    {
        if((*P1>='0')&&(*P1<='9'))
        {
            P2[n++] = *P1 - '0';
        }
        P1++;
    }
    P1 = P2+n-1;
    for(; n>0; n--)
    {
        A += *P1*i;
        i = i*10;
        P1--;
    }
    return A;
}
//===============================================================================
u8 MY_String_Compare(char *Command,unsigned char *Command1)
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
        if(((*Command1) == '\0')||((*Command1) == '\n')||((*Command1) == '\r')||((*Command1) == '@'))
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
void process(uint8_t *s)
{
    int i = 0;
    for(i=0; '\0' != s[i]; i++)
    {
        if(s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] += 32;
        }
    }
}
void UART1_Operation_function(void)
{
    u8 i=0;
    float Vx=0.0;
    uint16_t Tem16;
	uint8_t Addr;
	uint8_t Byte_Num;
	uint8_t buftabl[4];
	uint8_t cmd_mode,cmd_rate;
	uint32_t Data32;
	uint32_t Data32a;

	char * str = "";
//  float Vref=0.0;
    if(receiveover1==1)     //????????
    {
        receiveover1 = 0;
        printf("%s\r\n",ReceBUFF1Pre);
        process(ReceBUFF1Pre);
        if( MY_String_Compare("fixture_information",ReceBUFF1Pre)==0 )
        {
            printf("Name:ControlBroad\r\n");
            printf("Vendor: Intelligent\r\n");
            printf("HW Version:V0.1\r\n");
            printf("SW Version:V0.1\r\n");
        }
        else if( MY_String_Compare("fixture_id",ReceBUFF1Pre)==0 )
        {
            printf("Fixture ID:IA0001\r\n");
        }
        else if( MY_String_Compare("help",ReceBUFF1Pre)==0 )
        {
            printf("Fixture_Information\r\n");
            printf("Fixture_ID\r\n");
            printf("Reset_Fixture\r\n");
            printf("1v8_power_on\r\n");
            printf("1v8_power_off\r\n");
            printf("3v3_power_on\r\n");
            printf("3v3_power_off\r\n");
			printf("mv_1v8\r\n");
			printf("mv_vdd_main\r\n");
			printf("mc_1v8\r\n");
			printf("mc_vdd_main\r\n");
			printf("configure0a\r\n");
			printf("configure0\r\n");
			printf("configure1\r\n");
			printf("configure2\r\n");
			printf("temp_read\r\n");

//			printf("adc_setup\r\n");
//			printf("adc_Modes\r\n");
//			printf("strain_gage_direct_coupled\r\n");
//			printf("ntc_read\r\n");
			printf("temp_sensor_setup\r\n");
            printf("test_pass\r\n");
            printf("test_fail\r\n");
        }
        else if( MY_String_Compare("reset_fixture",ReceBUFF1Pre)==0 )
        {
            printf("reset fixture\r\n");
        }
//========================================================================================Test
        else if( MY_String_Compare("1v8_power_on",ReceBUFF1Pre)==0 )
        {
            Power1V8_On;
            printf("1v8 power on pass\r\n");
        }
        else if( MY_String_Compare("1v8_power_off",ReceBUFF1Pre)==0 )
        {
            Power1V8_Off;
            printf("1v8 power off pass\r\n");
        }
        else if( MY_String_Compare("3v3_power_on",ReceBUFF1Pre)==0 )
        {
            Power3V3_On;
			DUT_Detecte_On;
            printf("3v3 power on pass\r\n");
        }
        else if( MY_String_Compare("3v3_power_off",ReceBUFF1Pre)==0 )
        {
            Power3V3_Off;
            printf("3v3 power off pass\r\n");
        }
        else if( MY_String_Compare("adc_setup",ReceBUFF1Pre)==0 )
        {
            printf("ADC setup pass\r\n");
        }
        else if( MY_String_Compare("adc_modes",ReceBUFF1Pre)==0 )
        {
            printf("ADC Modes pass\r\n");
        }
        else if( MY_String_Compare("strain_gage_direct_coupled",ReceBUFF1Pre)==0 )
        {
            printf("Strain Gage Direct Coupled pass\r\n");
        }
        else if( MY_String_Compare("strain_gage_hpf_read",ReceBUFF1Pre)==0 )
        {
            printf("Strain Gage HPF Read pass\r\n");
        }
        else if( MY_String_Compare("ntc_read",ReceBUFF1Pre)==0 )
        {
            printf("NTC Read pass\r\n");
        }
        else if( MY_String_Compare("temp_sensor_setup",ReceBUFF1Pre)==0 )
        {
            printf("Temp Sensor Setup pass\r\n");
        }
		else if( MY_String_Compare("mv_vdd_main",ReceBUFF1Pre)==0 )
        {
			SW_CH1;
			Delayms(10);
			Vx = Get_ADC_Reault_ADC_average(0x8043,0x1300);
			
            printf("Vx=%3.2fV\r\n",Vx);
        }
		else if( MY_String_Compare("mv_1v8",ReceBUFF1Pre)==0 )
        {
			SW_CH2;
			Delayms(10);
			Vx = Get_ADC_Reault_ADC_average(0x8043,0x1300);
			Delayms(10);
            printf("Vx=%3.2fV\r\n",Vx);
        }
		else if( MY_String_Compare("mc_vdd_main",ReceBUFF1Pre)==0 )
        {
			I_SW1;
			PGA_200_3V3;
			Delayms(10);
			Vx = Get_ADC_Reault_ADC_average(0x8001,0x1300);
            printf("Vx=%fmA\r\n",Vx*1000/200);
        }
		else if( MY_String_Compare("mc_1v8",ReceBUFF1Pre)==0 )
        {
			I_SW2;
			PGA_200_1V8;
			Delayms(10);

			Vx = Get_ADC_Reault_ADC_average(0x8001,0x1300);
            printf("Vx=%fmA\r\n",Vx*1000/200);
        }
		else if( MY_String_Compare("temp_read",ReceBUFF1Pre)==0 )
        {
			Vx = temp_average(10);
			printf("Temp=%f\r\n",Vx);
        }
		else if( MY_String_Compare("output100",ReceBUFF1Pre)==0 )
        {
			output(100);
			printf("pass\r\n");
        }

//================== This is used for ADC Input 2 - AIN2P, AIN2N

		else if( MY_String_Compare("configure2",ReceBUFF1Pre)==0 )
		{
			output(100);
			register_read(0x00,buftabl,3);//
		
			buftabl[0] = 0x12;
			buftabl[1] = 0x34;
			buftabl[2] = 0x56;
			register_write(0x07,buftabl,3);
			register_read(0x07,buftabl,3);
			printf("receive0x07=%x%x%x%x\r\n",buftabl[0],buftabl[1],buftabl[2],buftabl[3]);
		
			buftabl[0] = 0x40;   //MUX1=1 ?????
			register_write(REG_SEQ,buftabl,1);
			register_read(REG_SEQ,buftabl,1);
			
			buftabl[0] = 0x00;
			register_write(REG_CTRL1,buftabl,1);
			register_read(REG_CTRL1,buftabl,1);
			
			buftabl[0] = 0x08;
			register_write(REG_CTRL2,buftabl,1);
			register_read(REG_CTRL2,buftabl,1);
			
			buftabl[0] = 0x04;	//OC3=1;
			register_write(REG_OC_DIRECT,buftabl,1);
			register_read(REG_OC_DIRECT,buftabl,1);
			
			SPI2_Send_One_Byte_CS(0xbe);//mode3     Data page 17 Rates 6400
			
			for(i=0;i<20;i++)
			{
				register_read(REG_DATA2,buftabl,3);
				Vx = data3ToInt(buftabl);
				printf("Data_From_ADC=%f\r\n",Vx);
			}
			printf("pass");

		}
//================== This is used for ADC Input 1 - AIN1P, AIN1N
		else if( MY_String_Compare("configure1",ReceBUFF1Pre)==0 )
		{
			output(100);
			register_read(0x00,buftabl,3);
		
			buftabl[0] = 0x12;
			buftabl[1] = 0x34;
			buftabl[2] = 0x56;
			register_write(0x07,buftabl,3);
			
			register_read(0x07,buftabl,3);
			printf("receive0x07=%x%x%x%x\r\n",buftabl[0],buftabl[1],buftabl[2],buftabl[3]);
		
			buftabl[0] = 0x20;
			register_write(REG_SEQ,buftabl,1);
			register_read(REG_SEQ,buftabl,1);
			
			buftabl[0] = 0x00;
			register_write(REG_CTRL1,buftabl,1);
			register_read(REG_CTRL1,buftabl,1);
			
			buftabl[0] = 0x08;
			register_write(REG_CTRL2,buftabl,1);
			register_read(REG_CTRL2,buftabl,1);
			
			buftabl[0] = 0x02;
			register_write(REG_OC_DIRECT,buftabl,1);
			register_read(REG_OC_DIRECT,buftabl,1);
			
			SPI2_Send_One_Byte_CS(0xbe);
			
			for(i=0;i<20;i++)
			{
				register_read(REG_DATA1,buftabl,3);
				Vx = data3ToInt(buftabl);
				printf("Data_From_ADC=%f\r\n",Vx);
			}
			printf("pass");

		}
//================== This is used for ADC Input 0 - AIN1P, AIN1N, PGA Enabled, Gain = 1,Sequencer Mode 1
		else if( MY_String_Compare("configure0",ReceBUFF1Pre)==0 )
		{
			output(100);
			register_read(0x00,buftabl,3);
			
			buftabl[0] = 0x12;
			buftabl[1] = 0x34;
			buftabl[2] = 0x56;
			register_write(0x07,buftabl,3);

			register_read(0x07,buftabl,3);
			printf("receive0x07=%x%x%x%x\r\n",buftabl[0],buftabl[1],buftabl[2],buftabl[3]);
		
			buftabl[0] = 0x00; //Seq mode 1
			register_write(REG_SEQ,buftabl,1);
			register_read(REG_SEQ,buftabl,1);
			
			buftabl[0] = 0x00;
			register_write(REG_CTRL1,buftabl,1);
			register_read(REG_CTRL1,buftabl,1);
			
			buftabl[0] = 0x08; // PGAEN=1  PGA 010
			register_write(REG_CTRL2,buftabl,1);
			register_read(REG_CTRL2,buftabl,1);
			
			buftabl[0] = 0x01;
			register_write(REG_OC_DIRECT,buftabl,1);
			register_read(REG_OC_DIRECT,buftabl,1);
			
			SPI2_Send_One_Byte_CS(0xbe);
			
			for(i=0;i<20;i++)
			{
				register_read(REG_DATA0,buftabl,3);
				Vx = data3ToInt(buftabl);
				printf("Data_From_ADC=%f\r\n",Vx);
			}
			printf("pass");

		}
//================== This is used for ADC Input 0 - AIN1P, AIN1N, PGA Disabled, Gain = 1, Mode= Sequencer Mode 3	
		else if( MY_String_Compare("configure0",ReceBUFF1Pre)==0 )
		{
			register_read(0x00,buftabl,3);
			
			buftabl[0] = 0x12;
			buftabl[1] = 0x34;
			buftabl[2] = 0x56;
			register_write(0x07,buftabl,3);
			register_read(0x07,buftabl,3);
			printf("receive0x07=%x%x%x%x\r\n",buftabl[0],buftabl[1],buftabl[2],buftabl[3]);
		
			buftabl[0] = 0x00;
			register_write(REG_SEQ,buftabl,1);
			register_read(REG_SEQ,buftabl,1);
			
			buftabl[0] = 0x00;
			register_write(REG_CTRL1,buftabl,1);
			register_read(REG_CTRL1,buftabl,1);
			
			buftabl[0] = 0x00;
			register_write(REG_CTRL2,buftabl,1);
			register_read(REG_CTRL2,buftabl,1);
			
			buftabl[0] = 0x01;
			register_write(REG_OC_DIRECT,buftabl,1);
			register_read(REG_OC_DIRECT,buftabl,1);
			
			SPI2_Send_One_Byte_CS(0xbe);
			
			for(i=0;i<20;i++)
			{
				register_read(REG_DATA0,buftabl,3);
				Vx = data3ToInt(buftabl);
				printf("Data_From_ADC=%f\r\n",Vx);
			}
			printf("pass");
			
		}
//==================================================================================		
		

		
		else if( MY_String_Compare("write3",ReceBUFF1Pre)==0 )
		{
			SPI2_CS_0();
			wrBuf[1] = 0x12;
			wrBuf[2] = 0x34;
			wrBuf[3] = 0x56;
			register_write(0x0f,wrBuf,3);
			SPI2_CS_1();
			printf("pass");
		}
		else if( MY_String_Compare("read3",ReceBUFF1Pre)==0 )
		{
			SPI2_CS_0();
			register_read(0x0f,wrBuf,3);
			printf("receive=%x%x%x%x\r\n",wrBuf[0],wrBuf[1],wrBuf[2],wrBuf[3]);
//			printf("receive=%x\r\n",Byte_Num);
			SPI2_CS_1();
		}
		

		
		else if( MY_String_Compare("output100",ReceBUFF1Pre)==0 )
        {
            output(100);
            printf("output100\r\n");
        }
		else if( MY_String_Compare("output1",ReceBUFF1Pre)==0 )
        {
            output(0);
            printf("output1\r\n");
        }

//================================================================================================
		else if( MY_String_Compare("spiw",ReceBUFF1Pre)==0 )
        {
			Decoder(ReceBUFF1Pre);
            printf("pass");
        }
		else if( MY_String_Compare("spir",ReceBUFF1Pre)==0 )
        {
			DecoderRead(ReceBUFF1Pre);
            printf("pass");
        }
		
		else if( MY_String_Compare("current",ReceBUFF1Pre)==0 )
        {
            Tem16 = getNunb(ReceBUFF1Pre);
            AD5660_Out(Tem16);
            printf("Current7=%dV\r\n",Tem16);
        }
		else if( MY_String_Compare("AD2",ReceBUFF1Pre)==0 )
        {
			Vx = Get_ADC_Reault_ADC_average(0x8043,1320);
            printf("Vx=%fV\r\n",Vx);
        }
		else if( MY_String_Compare("AD1",ReceBUFF1Pre)==0 )
        {
			Vx = Get_ADC_Reault_ADC_average(0x8001,1320);
            printf("Vx=%fV\r\n",Vx);
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
        printf("@_@\r\n");
    }

}

#include "config.h"

void SPI2_DUT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;  
	SPI_InitTypeDef SPI_InitStruct;  
	/*??GPIOB, SPI2??*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/*sck  miso  mosi*/  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15;  
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStruct);  
	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;  
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
//	GPIO_Init(GPIOB, &GPIO_InitStruct);  
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;  
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStruct);  
	GPIO_SetBits(GPIOB, GPIO_Pin_12);  
	
//	GPIO_SetBits(GPIOB, GPIO_Pin_14); 
	
	/*??cs?????*/  
//	SPI_SSOutputCmd(SPI1, ENABLE); 
	

//		
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;  
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStruct.SPI_CRCPolynomial = 7;  
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;  
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;  
//	SPI_InitStruct.SPI_NSS = SPI_NSS_Hard;  
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;/**/  
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;  
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; 
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;  
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	
//	SPI_SSOutputCmd(SPI2, DISABLE);
	SPI_Init(SPI2, &SPI_InitStruct);  
	
	SPI_Cmd(SPI2, ENABLE);

}

void SPI2_Send_One_Byte(uint8_t data)
{	
/* Loop while DR register in not emplty */ 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
	/* Send byte through the SPI1 peripheral */ 
//	printf("data=%x\r\n",data);
	SPI_I2S_SendData(SPI2, data); 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
	/* Return the byte read from the SPI bus */ 
	SPI_I2S_ReceiveData(SPI2);
}

uint8_t SPI2_Read_One_Byte(void)
{
	/* Loop while DR register in not emplty */ 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
	/* Send byte through the SPI1 peripheral */ 
//	printf("data=%x\r\n",data);
	SPI_I2S_SendData(SPI2, 0xff);
	Delay1us(1);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
	/* Return the byte read from the SPI bus */ 
	return SPI_I2S_ReceiveData(SPI2);	
}


void SPI2_Send_One_Byte_CS(uint8_t data)
{
	SPI2_CS_1();
	Delayus(2);
	SPI2_CS_0();
	SPI2_Send_One_Byte(data);
	SPI2_CS_0();
	Delayus(2);
	SPI2_CS_1();
}

void register_write(uint8_t Addr,uint8_t * data,uint8_t Byte_Num)
{
	uint8_t bytesNumber;
	uint8_t i;
	SPI2_CS_0();
//	Delay1us(20);
	Addr = (0x3<<6 | Addr<<1 | WRITE);
	SPI2_Send_One_Byte(Addr);
    for(i=0; i<Byte_Num; i++)
    {
        SPI2_Send_One_Byte(data[i]);
    }
//	Delay1us(20);
	SPI2_CS_1();
}
uint32_t register_read(uint8_t Addr,uint8_t * data,uint8_t Byte_Num)
{
	uint8_t i;
	uint8_t Data_Tem;
	uint32_t AD_Value;
	SPI2_CS_0();
	Data_Tem = (0x3<<6 | Addr<<1 | READ);

	SPI2_Send_One_Byte(Data_Tem);
	
    for(i=0; i<Byte_Num; i++)
    {
        data[i]=SPI2_Read_One_Byte(); 
//		printf("receivedata0=%x\r\n",data[i]);
    }

	SPI2_CS_1();
}



//==================================================================================
////======================================================================================

//void SPI2_DUT_Init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    RCC_APB2PeriphClockCmd(SPI2_GPIO_Port_RCC, ENABLE);
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    //SPI_CS
//    GPIO_InitStructure.GPIO_Pin = SPI2_CS_GPIO_Pin;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(SPI2_CS_GPIO_Port, &GPIO_InitStructure);
//    SPI2_CS_1();                                         //cs初始电平为高
//    //SPI_CLK
//    GPIO_InitStructure.GPIO_Pin = SPI2_CLK_GPIO_Pin;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(SPI2_CLK_GPIO_Port, &GPIO_InitStructure);
//    SPI2_CLK_0();                                        //CLK初始电平为低
//    //SPI_MISO
//    GPIO_InitStructure.GPIO_Pin = SPI2_MISO_GPIO_Pin;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(SPI2_MISO_GPIO_Port, &GPIO_InitStructure);
//    //SPI_MOSI
//    GPIO_InitStructure.GPIO_Pin = SPI2_MOSI_GPIO_Pin;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(SPI2_MOSI_GPIO_Port, &GPIO_InitStructure);
//    SPI2_MOSI_1();                                   //MOSI初始电平为高
//}


//void SPI2_Send_One_Byte(u8 data)
//{
//    u8 i=0;
////	printf("data=%x\r\n",data); 
//    for(i=0; i<8; i++)
//    {
//        SPI2_CLK_0();                    //初始状态时钟为高电平
//        SPI2_CLK_Delay();
//                       //CLK下降沿，数据线路可改变状态
//        if( data & 0x80 )           //MSB_First
//        {
//            SPI2_MOSI_1();
//        }
//        else
//        {
//            SPI2_MOSI_0();
//        }
//		SPI2_CLK_1();   
//        SPI2_CLK_Delay();
//        SPI2_CLK_0();                    //上升沿，数据传输给从机
//        data=data<<1;                   //待发送数据左移，以发送下一位数据
//    }
//	 SPI2_MOSI_0();
//    SPI2_CLK_Delay();            //一个字节数据发送完毕，做延时
//}
//u8 SPI2_Read_One_Byte(void)
//{
//    u8 i=0;
//    u8 dat=0;
////  if(Read_SPI_MISO())
//    for(i=0; i<8; i++)
//    {
//        dat=dat<<1;                     //数据缓冲变量左移，以准备接受下一bit数据
//        SPI2_CLK_0();                    //初始状态均为高电平
//        SPI2_CLK_Delay();
//        SPI2_CLK_1();                    //CLK下降沿，从机可以改变数据线路电平
//        SPI2_CLK_Delay();            //低电平保持
////        SPI2_CLK_1();                    //上升沿，数据传输给从机
//       SPI2_CLK_Delay();            //做延时后再读取数据
//        if( Read_SPI2_MISO() )   //若读取到数据为1
//        {
//            dat= dat|0x01;   //存储
//        }
//    }
////    SPI2_CLK_Delay();
//	SPI2_CLK_0();
//    return  dat;                        //返回接收到的结果数据
//}


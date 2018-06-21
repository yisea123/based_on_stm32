#include "config.h"
/********************************************************/
void TMP102_Init(void)
{
    IIC_Init1();
}
/********************************************************/
uint32_t TMP102_ReadTwoByte(uint16_t PointerAddr)
{
    uint32_t temp=0;
    uint16_t temp1,temp2;
    IIC_Start1();
    IIC_Send_Byte1(0x92);
    IIC_Wait_Ack1();
    IIC_Send_Byte1(PointerAddr); 
    IIC_Wait_Ack1();
    IIC_Stop1(); 
    Delayms(50);
    IIC_Start1();
    IIC_Send_Byte1(0x93);
    IIC_Wait_Ack1();
    temp1=IIC_Read_Byte1();
    IIC_Ack1();
    temp2=IIC_Read_Byte1();
    IIC_Ack1();
    temp1<<=8;
    temp=temp1|temp2;
    temp>>=4;
    IIC_Stop1(); 
    Delayms(10);
    return temp;
}
/********************************************************/
void TMP102_WriteTwoByte(uint16_t PointerAddr,uint16_t Data1Write,uint16_t Data2Write)
{
    IIC_Start1();
    IIC_Send_Byte1(0x92);
    IIC_Wait_Ack1();
    IIC_Send_Byte1(PointerAddr);
    IIC_Wait_Ack1();
    IIC_Send_Byte1(Data1Write); 
    IIC_Wait_Ack1();
    IIC_Send_Byte1(Data2Write); 
    IIC_Wait_Ack1();
    IIC_Stop1();
    Delayms(10);
}
uint32_t Complement(uint32_t data)
{
	uint32_t Tem;
	
	if((data&0x800) == 0x800)
	{
		Tem = data;
		data =~ Tem; 
		data = data+1;
	}
	return data;
}
float temp_average(uint8_t times)
{
	uint32_t temp;
	uint8_t i;
	float temperate=0;	
	TMP102_WriteTwoByte(0x01,0x78,0xA0);
	Delayms(10);
	for(i=0;i<times;i++)
	{
		temp = TMP102_ReadTwoByte(0x0);
		temp = Complement(temp);		
		temperate+=(float)temp*(0.0625);
	}
	temperate=temperate/times;
	return temperate ;

}


//=====================================================================================

////==========================================================


//void IIC_Init1(void)
//{					     
//	GPIO_InitTypeDef GPIO_InitStructure;
//	I2C_InitTypeDef I2C_InitStructure;
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE );
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;   //????=============GPIO_Mode_Out_OD 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 

//	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;//I2C_Mode_SMBusDevice//I2C_Mode_I2C
//	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_16_9;//I2C_DutyCycle_16_9//I2C_DutyCycle_2
//	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
//	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
//	I2C_InitStructure.I2C_ClockSpeed = 200000;
//	I2C_Init(I2C2,&I2C_InitStructure);
//	I2C_Cmd(I2C2,ENABLE);
//}




//void TMP102_WriteTwoByte(uint16_t PointerAddr,uint8_t Data1Write,uint8_t Data2Write)
//{  /* 1??*/
//	I2C_GenerateSTART(I2C2, ENABLE);
//  /* Test on EV5 and clear it */
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));  
//	/*2????/?*/
//	I2C_Send7bitAddress(I2C2, 0x90, I2C_Direction_Transmitter);//I2C_Direction_Transmitter I2C_Direction_Receiver
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//		I2C_SendData(I2C2, PointerAddr);

//  /* Test on EV8 and clear it */
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
////		I2C_Send7bitAddress(I2C2, PointerAddr, I2C_Direction_Transmitter);
////	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//      /*????/?*/
//	I2C_SendData(I2C2, Data1Write);

//  /* Test on EV8 and clear it */
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//  /* Send the byte to be written */
//	I2C_SendData(I2C2, Data2Write); 
// 
//  /* Test on EV8 and clear it */
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//  /* Send S
//  tion */
//	I2C_GenerateSTOP(I2C2, ENABLE);
//}

//////********************************************************/ 



// uint32_t TMP102_ReadTwoByte(uint16_t PointerAddr)
//{			uint32_t Date=0;
//	uint16_t Date1,Date2;
////   uint16_t *PDate;

//	I2C_GenerateSTART(I2C2, ENABLE);
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
//	I2C_Send7bitAddress(I2C2, 0x90, I2C_Direction_Transmitter);
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//	I2C_SendData(I2C2, PointerAddr);

//  /* Test on EV8 and clear it */
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
////	I2C_Send7bitAddress(I2C2, PointerAddr, I2C_Direction_Transmitter);
////	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//	I2C_GenerateSTOP(I2C2, ENABLE);
//  	Delayms(50);
//	I2C_GenerateSTART(I2C2, ENABLE);
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
//	I2C_Send7bitAddress(I2C2, 0x91, I2C_Direction_Receiver);
//	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//    I2C_AcknowledgeConfig(I2C2,ENABLE);
//	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_RXNE)==RESET);
//	Date1=I2C_ReceiveData(I2C2);

//	I2C_AcknowledgeConfig(I2C2,ENABLE);//	temp1=IIC_Read_Byte1();
//	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_RXNE)==RESET)
//	Date2=I2C_ReceiveData(I2C2);
//   	Date1<<=8;
//	Date=Date1|Date2;
//	Date>>=4;	
//	I2C_GenerateSTOP(I2C2, ENABLE);
//	Delayms(10);
//	return Date;
//}


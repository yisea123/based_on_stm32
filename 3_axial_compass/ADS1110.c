#include "main.h"


//#define u8 uchar
//#define u16 uint
//#define u32 long
#define SDA_PIN         ((PINA & (1<<(2))) ? 1 : 0)

#define ADS1110_WR_ADDRESS 0x92 // 1001 001 0 д
#define ADS1110_RD_ADDRESS 0x93 // 1001 001 1 ��
#define ADS1110_CONFIG_REG 0x8e // ����ת��ģʽ��16bit���ȣ�PGA=8


#define TWI_SCL_0 clrbit(PORTA, PA1)
#define TWI_SCL_1 setbit(PORTA, PA1)
#define TWI_SDA_0 clrbit(PORTA, PA2)
#define TWI_SDA_1 setbit(PORTA, PA2)
//#define TWI_SDA_STATE ((PINA & (1<<(2))) ? 1 : 0)

//#include "ADS1110.h"
#define TWI_NOP TWI_Delay()
#define I2CSendAck() (TWCR|=(1<<TWEA))

u16 AD_Result[13];
enum ENUM_TWI_REPLY
{
TWI_NACK=0
,TWI_ACK=1
};
enum ENUM_TWI_BUS_STATE
{
TWI_READY=0
,TWI_BUS_BUSY=1
,TWI_BUS_ERROR=2
};


/*******************************************************************************
* ��������: TWI_Delay
* ����: ��ʱ����
*
* ����: ��
* ���: ��
* ����: ��
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
void TWI_Delay(void)
{
u32 i=1;
while(i--);
}
/*******************************************************************************
* ��������:
TWI_Initialize
* ����:
I2C��ʼ������
*
* ����: ��
* ���: ��
* ����: ��
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
uchar TWI_SDA_STATE(void)
{
	DDRA &= ~(1<<PA2);
	TWI_NOP;
	//TWI_NOP;
	if((PINA & (1<<2)) != 0)
	{
		DDRA |= (1<<PA2);
		return 1;
	}
	else
	{
		DDRA |= (1<<PA2);
		return 0;
	}
	
}


/*******************************************************************************
-2-
F:\��Ƭ��\ARM\����\ADS1110.c 2013��1��8��21:19
* ��������: TWI_START
* ����: ��������
*
* ����: ��
* ���: ��
* ����: ��
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
u8 TWI_START(void)
{
TWI_SDA_1;
TWI_NOP;
TWI_SCL_1;
TWI_NOP;
//if(!TWI_SDA_STATE())
{
////DebugPrint("TWI_START:BUSY\n");
//return TWI_BUS_BUSY;
}
TWI_SDA_0;
TWI_NOP;
//if(TWI_SDA_STATE())
{
////DebugPrint("TWI_START:BUS ERROR\n");
//return TWI_BUS_ERROR;
}
TWI_SCL_0;
TWI_NOP;
return TWI_READY;
}
/*******************************************************************************
* ��������: TWI_STOP
* ����: ����ֹͣλ
*
* ����: ��
* ���: ��
* ����: ��
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
void TWI_STOP(void)
{
TWI_SCL_0;
TWI_NOP;
TWI_SDA_0;
TWI_NOP;

TWI_SCL_1;
TWI_NOP;
TWI_SDA_1;
TWI_NOP;
// TWI_SCL_0;
// TWI_NOP;
//////DebugPrint("TWI_STOP\n");
}
/*******************************************************************************
* ��������: TWI_SendACK
* ����:
�յ�����,����ACK
*
* ����: ��
* ���: ��
* ����: ��
* ����: ÷������
* �޸�����:
2012��10��20��
*******************************************************************************/
void TWI_SendACK(void)
{
TWI_SCL_0;
TWI_NOP;
TWI_SDA_0;
TWI_NOP;
TWI_SCL_1;
TWI_NOP;
TWI_SCL_0;
TWI_NOP;
//////DebugPrint("TWI_SendACK\n");
}
/*******************************************************************************
* ��������: TWI_SendNACK
* ����:
�յ�����,����NACK
*
* ����: ��
* ���: ��
* ����: ��
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
void TWI_SendNACK(void)
{
TWI_SCL_0;
TWI_NOP;
TWI_SCL_1;
TWI_NOP;

TWI_SDA_1;
TWI_NOP;
TWI_SCL_0;
TWI_NOP;
//////DebugPrint("TWI_SendNACK\n");
}
u8 TWI_WaitAck(void)
{
TWI_SCL_0;
TWI_NOP;
TWI_SDA_1;
TWI_NOP;
TWI_SCL_1;
TWI_NOP;
if(TWI_SDA_STATE())
{
TWI_SCL_0;
return 0x00;
}
TWI_SCL_0;
return 0x01;
}
/*******************************************************************************
* ��������: TWI_SendByte
* ����: ����һ���ֽ�
*
* ����:
Ҫ���͵����ݣ�1�ֽڣ�
* ���: ��
* ����:
TWI_ACK/TWI_NACK
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
u8 TWI_SendByte(u8 Data)
{
u8 i;
TWI_SCL_0;
for(i=0;i<8;i++)
{
TWI_SCL_0;
TWI_NOP;
//---------���ݽ���----------
if(Data&0x80)
{
TWI_SDA_1;
}
else
{

TWI_SDA_0;
}
Data<<=1;
TWI_NOP;
//---���ݽ�������һ����ʱ----
TWI_SCL_1;
TWI_NOP;
}
TWI_SCL_0;


if(TWI_SDA_STATE())
{
// TWI_SCL_0;
//////DebugPrint("TWI_NACK!\n");
return TWI_NACK;
}
else
{
// TWI_SCL_0;
//////DebugPrint("TWI_ACK!\n");
return TWI_ACK;
}

}
/*******************************************************************************
* ��������:
TWI_ReceiveByte
* ����: ����һ���ֽ�
*
* ����: ��
* ���: ��
* ����:
���ؽ��յ�������
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
u8 TWI_ReceiveByte(void)
{
u8 i,Dat;
TWI_SDA_1;
//TWI_SCL_0;
Dat=0;
for(i=0;i<8;i++)
{
//TWI_SCL_1;//����ʱ��������[������],�ôӻ�׼��������
//TWI_NOP;
Dat<<=1;
TWI_SCL_0;
TWI_NOP;
TWI_SCL_1;
TWI_NOP;
if(TWI_SDA_STATE()) //������״̬
{

Dat|=0x01;
}
TWI_SCL_0;//׼�����ٴν�������
//TWI_NOP;//�ȴ�����׼����
}
//////DebugPrint("TWI_Dat:%x\n",Dat);
return Dat;
}
/*******************************************************************************
* ��������:
ads1110Config
* ����:
��ADS1110��������
*
* ����: ��
* ���: ��
* ����: ��
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
/*
void ads1110Config(void)
{
TWI_START();
TWI_SendByte(ADS1110_WR_ADDRESS);
TWI_SendNACK();
TWI_SendByte(ADS1110_CONFIG_REG);
TWI_SendNACK();
TWI_STOP();
}
*/
/*******************************************************************************
* ��������: RD_ADS
* ����:
��ȡADS1100�����ӳ���
*
* ����: ��
* ���: ��
* ����: ת���������
* ����:
* �޸�����:
2012��10��20��
*******************************************************************************/
/*
u16 RD_ADS(void)
{
// u8 temp;
u16 W_B1byte_high, W_B1byte_low, W_B1_word;
TWI_START();
TWI_SendByte(ADS1110_RD_ADDRESS);
TWI_SendNACK();
//

// if(!temp)
// {
W_B1byte_high  = TWI_ReceiveByte();
TWI_SendACK();
W_B1byte_low = TWI_ReceiveByte();
TWI_SendACK();
W_B1_word = TWI_ReceiveByte();
TWI_STOP();
W_B1_word = (W_B1byte_high << 8)+ W_B1byte_low;
// if (W_B1_word > 0x7fff)
// W_B1_word = 0;
return W_B1_word;
// }
// else
// return 0x0000;
}
*/

void ads1110Config(void)
{
	TWI_Start();
	TWI_Write(ADS1110_WR_ADDRESS); 
	TWI_Write(ADS1110_CONFIG_REG); 	 
	TWI_Stop();
}
u16 RD_ADS(void)
{
	u16 W_B1byte_high, W_B1byte_low, W_B1_word;
	TWI_Start();
	TWI_Write(ADS1110_RD_ADDRESS);
	W_B1byte_high = i2c_read(1);
	//I2CSendAck();
	W_B1byte_low = i2c_read(1);
	//I2CSendAck();
	W_B1_word = i2c_read(1);
	//I2CSendAck();
	W_B1_word = (W_B1byte_high << 8)+ W_B1byte_low;
	return W_B1_word;
}
u16 get_ad_result(void)
{
	u8 i,j;
	u16 temp;
	for(i = 0; i < 13; i++) // ����ȡֵ13��
	{
		AD_Result[i] = RD_ADS();
		delayus(10);
	}
	for(i = 1; i< 13; i++) // ���뷨��ȡ��13��ֵ��������
	{
		temp = AD_Result[i]; // store the original sorted arrayin temp
		for(j=i ; j>0 && temp < AD_Result[j-1] ; j--) // compare the new array with temp
		{
			AD_Result[j] = AD_Result[j-1]; // all larger elements are moved onepot to the right
		}
		AD_Result[j] = temp;
	}
	return AD_Result[7];
}


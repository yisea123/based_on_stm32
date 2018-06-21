#include "main.h"


//#define u8 uchar
//#define u16 uint
//#define u32 long
#define SDA_PIN         ((PINA & (1<<(2))) ? 1 : 0)

#define ADS1110_WR_ADDRESS 0x92 // 1001 001 0 写
#define ADS1110_RD_ADDRESS 0x93 // 1001 001 1 读
#define ADS1110_CONFIG_REG 0x8e // 连续转换模式，16bit精度，PGA=8


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
* 函数名称: TWI_Delay
* 描述: 延时函数
*
* 输入: 无
* 输出: 无
* 返回: 无
* 作者:
* 修改日期:
2012年10月20日
*******************************************************************************/
void TWI_Delay(void)
{
u32 i=1;
while(i--);
}
/*******************************************************************************
* 函数名称:
TWI_Initialize
* 描述:
I2C初始化函数
*
* 输入: 无
* 输出: 无
* 返回: 无
* 作者:
* 修改日期:
2012年10月20日
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
F:\单片机\ARM\程序\ADS1110.c 2013年1月8日21:19
* 函数名称: TWI_START
* 描述: 发送启动
*
* 输入: 无
* 输出: 无
* 返回: 无
* 作者:
* 修改日期:
2012年10月20日
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
* 函数名称: TWI_STOP
* 描述: 发送停止位
*
* 输入: 无
* 输出: 无
* 返回: 无
* 作者:
* 修改日期:
2012年10月20日
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
* 函数名称: TWI_SendACK
* 描述:
收到数据,发送ACK
*
* 输入: 无
* 输出: 无
* 返回: 无
* 作者: 梅川酷子
* 修改日期:
2012年10月20日
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
* 函数名称: TWI_SendNACK
* 描述:
收到数据,发送NACK
*
* 输入: 无
* 输出: 无
* 返回: 无
* 作者:
* 修改日期:
2012年10月20日
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
* 函数名称: TWI_SendByte
* 描述: 发送一个字节
*
* 输入:
要发送的数据（1字节）
* 输出: 无
* 返回:
TWI_ACK/TWI_NACK
* 作者:
* 修改日期:
2012年10月20日
*******************************************************************************/
u8 TWI_SendByte(u8 Data)
{
u8 i;
TWI_SCL_0;
for(i=0;i<8;i++)
{
TWI_SCL_0;
TWI_NOP;
//---------数据建立----------
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
//---数据建立保持一定延时----
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
* 函数名称:
TWI_ReceiveByte
* 描述: 接收一个字节
*
* 输入: 无
* 输出: 无
* 返回:
返回接收到的数据
* 作者:
* 修改日期:
2012年10月20日
*******************************************************************************/
u8 TWI_ReceiveByte(void)
{
u8 i,Dat;
TWI_SDA_1;
//TWI_SCL_0;
Dat=0;
for(i=0;i<8;i++)
{
//TWI_SCL_1;//产生时钟上升沿[正脉冲],让从机准备好数据
//TWI_NOP;
Dat<<=1;
TWI_SCL_0;
TWI_NOP;
TWI_SCL_1;
TWI_NOP;
if(TWI_SDA_STATE()) //读引脚状态
{

Dat|=0x01;
}
TWI_SCL_0;//准备好再次接收数据
//TWI_NOP;//等待数据准备好
}
//////DebugPrint("TWI_Dat:%x\n",Dat);
return Dat;
}
/*******************************************************************************
* 函数名称:
ads1110Config
* 描述:
对ADS1110进行配置
*
* 输入: 无
* 输出: 无
* 返回: 无
* 作者:
* 修改日期:
2012年10月20日
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
* 函数名称: RD_ADS
* 描述:
读取ADS1100数据子程序
*
* 输入: 无
* 输出: 无
* 返回: 转换后的数据
* 作者:
* 修改日期:
2012年10月20日
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
	for(i = 0; i < 13; i++) // 连续取值13次
	{
		AD_Result[i] = RD_ADS();
		delayus(10);
	}
	for(i = 1; i< 13; i++) // 插入法对取的13个值进行排序
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


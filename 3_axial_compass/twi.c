
#include "main.h"

//#include "twi.h"
//-----------------------------------------
//				IIC初始化
//-----------------------------------------
void WaitACK(void)
{
 	uint ContACK;
	while(1)
	{
		if((TWCR & (1<<TWINT)))
		{
			ContACK = 0;
			_NOP();_NOP();
			//Transmit_arraydata("A");
			break;
		}
		else
		{
			ContACK ++;
			_NOP();_NOP();
			if(ContACK > 2000)
			{
				//Transmit_arraydata("time out\r\n");
				break;
			}
		}
	}
}
/*************************************************************************
** 函数名称: twi_init(void)
** 功能描述: i2c通信初始化
** 输　入:
** 输出	 :
** 全局变量: 无
** 调用模块:
** 说明：
** 注意：
**************************************************************************/
void TWI_init(void)
{
     TWCR= 0x00; //disable twi
     TWBR= (1<<6) | (1<<5) | (1<<2); //set bit rate
     TWSR= 0x00; //set prescale
     TWAR= 0x00; //set slave address
     TWCR= (1<<TWEN); //enable twi
}
/*************************************************************************
** 函数名称: i2cstart(void)
** 功能描述: i2c通信开始
** 输　入:
** 输出	 :
** 全局变量: 无
** 调用模块:
** 说明：
** 注意：
**************************************************************************/
void TWI_Start(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	_NOP();_NOP();
   	//while (!(TWCR & (1<<TWINT)));
   	WaitACK();
   	_NOP();_NOP();
}
/*************************************************************************
** 函数名称: unsigned char i2cwt(unsigned char data)
** 功能描述: i2c写数据,返回TWI状态
** 输　入:
** 输  出: TWI状态
** 全局变量: 无
** 调用模块:
** 说明：
** 注意：
**************************************************************************/
uchar TWI_Write(uchar data)
{

	TWDR = data;
   	TWCR = (1<<TWINT) | (1<<TWEN);
   	//while (!(TWCR & (1<<TWINT)));
    WaitACK();
	 _NOP();_NOP();
   	return(TWSR&0b11111000);

}
/*************************************************************************
** 函数名称: unsigned char i2crd(void)
** 功能描述: i2c读数据
** 输　入:
** 输出	 : 读取的数据
** 全局变量: 无
** 调用模块:
** 说明：
** 注意：
**************************************************************************/
uchar TWI_Read(void)
{
   	TWCR= (1<<TWINT)  | (1<<TWEN);//| (1<<TWEA)
   	//while (!(TWCR & (1<<TWINT)));
    WaitACK();
   	_NOP();
   	return(TWDR);

}
uchar i2c_read(uchar ack)   
{  
	uchar r;
	TWCR = (1<<TWINT) | (1<<TWEN) | (ack? (1<<TWEA) : 0);  
  	//waitTransmissionI2C();  
	_NOP();_NOP();
	WaitACK();
	r = TWDR;       //返回TWDR（数据寄存器）中的值  
	if (!ack) TWI_Stop();  
	return r;  
}  


/*************************************************************************
** 函数名称: i2cstop(void)
** 功能描述: i2c停止
** 输　入:
** 输出	 :
** 全局变量: 无
** 调用模块:
** 说明：
** 注意：
**************************************************************************/
void TWI_Stop(void)
{
   TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
  // printf("dfsdf");
}
//===============================================================



#include "main.h"

void delay_ms(unsigned char ms)	
// 延时子程序
{
	unsigned int i;
	while(ms--)
	{
		for(i = 0; i < 120; i++);
	}
}
void Init_AD5629(void)
{
 	SDA_out();
	SCL_out();
	CLR_out();
 	SDA_Hig();
	SCL_Hig();
	CLR_Hig();
	Write_AD5629(COMD8,0, 0,0);//复位
	Write_AD5629(COMD9,0, 0x0001,0);//设置为内部参考电压源
}
// 开始位	
void start_signed(void)
{
 	SDA_out();
	SDA_Hig();
	SCL_Hig();
	NOP();
	NOP();
	NOP();
	SDA_Low();
	NOP();
	NOP();
	NOP();
	NOP();
	SCL_Low();
}
// 停止位
void stop_signed(void)
{
 	SDA_out();
	SDA_Low();
	NOP();
	NOP();
	NOP();
	SCL_Hig();
	NOP();
	NOP();
	NOP();
	NOP();
	SDA_Hig();
}
void Write_byte(unsigned char write_data)
// 从MCU移出数据到AD5629
{
	unsigned char i;
	SDA_out();
	for(i = 0; i < 8; i++)		// 循环移入8个位
	{
		if((write_data & 0x80)==0x80) SDA_Hig();
		else SDA_Low();
		NOP();
		NOP();
		SCL_Hig();
		NOP();
		NOP();
		SCL_Low();
		write_data <<= 1;
	}
	SDA_in();
	SDA_Hig();
	NOP();
	NOP();
	SCL_Hig();
	NOP();
	NOP();
	NOP();
	NOP();
	SCL_Low();
	NOP();
}
/*说明：
功能：将电压值转换成DAC值
Voltage：要转化的电压值,单位位mV.AD5629不超过2.5V.
*/
unsigned short Volt2DAC(unsigned short Voltage)
{
   unsigned short V;
   V = Voltage;
   V = 4096*V/2500;
   return V;
}
/*
dat不能大于4095
Commad : 指令
CH ： 控制通道
dat ：参数
IS_V ：是否为电压控制。 1：是 0：否
*/
void Write_AD5629(unsigned char Commad,unsigned char CH, unsigned short dat,unsigned char IS_V)
{
    unsigned short data;
    unsigned char  CMD_CH;
    unsigned char  Hbyte;
    unsigned char  Lbyte;
	if(dat>4095) return;
	CMD_CH = Commad|CH;
	data = dat;
	if(IS_V) data <<= 4;
	Lbyte = (unsigned char)(data&0x00FF);
	Hbyte = (unsigned char)(data>>8);
	
 	start_signed(); 
	Write_byte(0XAE);  
	Write_byte(CMD_CH);
	Write_byte(Hbyte);
	Write_byte(Lbyte);
	stop_signed();
	delay_ms(50);    //延时10MS等待写操作完成
}
/*
unsigned char Read_AD5629(unsigned char Commad,unsigned char CH,unsigned char IS_V)
{
return 0;
}
*/
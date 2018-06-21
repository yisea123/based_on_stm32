//#include"INIT.H"
#ifndef _AD5629_h
#define _AD5629_h


#define   SDA_out()   	  setbit(DDRD,5)    //设置SDA为输出
#define   SDA_in()    	  clrbit(DDRD,5)    //设置SDA为输入
#define   SDA_Hig()   	  setbit(PORTD,5)   //设置SDA为高电平
#define   SDA_Low()   	  clrbit(PORTD,5)   //设置SDA为低电平
#define   SDA_action()    getbit(PIND,5)    //获取应答信号

#define   SCL_out()   	  setbit(DDRD,6)    //设置SCL为输出
#define   SCL_Hig()   	  setbit(PORTD,6)   //设置SCL为高电平
#define   SCL_Low()   	  clrbit(PORTD,6)   //设置SCL为低电平

#define   CLR_out()   	  setbit(DDRD,4)    //设置SDA为输出
#define   CLR_Hig()   	  setbit(PORTD,4)   //设置CLR为高电平
#define   CLR_Low()   	  clrbit(PORTD,4)   //设置CLR为高电平

//命令定义
#define	  COMD1	  0x00  //写入输入寄存器n
#define	  COMD2	  0x10	//更新DAC寄存器n
#define	  COMD3	  0x20	//写入输入寄存器n，更新全部(软件LDAC)
#define	  COMD4	  0x30	//写入并更新DAC通道n
#define	  COMD5	  0x40	//DAC掉电/上电Z
#define	  COMD6	  0x50	//加载清零编码寄存器
#define	  COMD7	  0x60	//加载LDAC寄存器
#define	  COMD8	  0x70	//复位(上电复位)
#define	  COMD9	  0x80	//设置内部REF寄存器
#define	  COMD10  0x90	//使能多字节模式
//输出通道定义
#define	  DAC_A	  0x00 	  
#define	  DAC_B	  0x01 	  
#define	  DAC_C	  0x02 	  
#define	  DAC_D	  0x03
#define	  DAC_E	  0x04 	  
#define	  DAC_F	  0x05
#define	  DAC_G	  0x06 	  
#define	  DAC_H	  0x07
#define	  DAC_All 0x0f

void Init_AD5629(void);
unsigned short Volt2DAC(unsigned short Voltage);
void Write_AD5629(unsigned char Commad,unsigned char CH, unsigned short dat,unsigned char IS_V);
unsigned char  Read_AD5629(unsigned char Commad,unsigned char CH,unsigned char IS_V);


#endif























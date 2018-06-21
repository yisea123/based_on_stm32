//模拟IIC
#ifndef _IIC_H
#define _IIC_H
//#include"stm32f10x.h"
//#include"lcd.h"
/********************************************************
 * 直接寄存器操作
 ********************************************************/
/******* PC12置1  *************************/
#define IIC_SCL_SET  GPIOB->BSRR=1<<10
/******* PC11置1  *************************/
#define IIC_SDA_SET  GPIOB->BSRR=1<<11
/******* PC12置0  *************************/
#define IIC_SCL_CLR  GPIOB->BRR=1<<10
/******* PC11置0  *************************/
#define IIC_SDA_CLR  GPIOB->BRR=1<<11
/******* SDA输入  *************************/
#define READ_SDA1   PBin(11)
/******* SDA输出 *************************/
#define IIC_SDA1    PBout(11)
/********************************************************
 * IO口的方向设置 （输入，输出）
 ********************************************************/
/*******  SDA输入方向 *************************/
//#define SDA_IN1()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<11;}
///*******  SDA输出方向 *************************/
//#define SDA_OUT1() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<11;}
/********************************************************
 * IIC所有操作函数
 ********************************************************/
/******* IIC的IO口初始化 *************************/
void IIC_Init1(void);
/******* 发送IIC开始信号  *************************/
void IIC_Start1(void);
/******* 发送IIC停止信号  *************************/
void IIC_Stop1(void);
/******* IIC发送一个字节  *************************/
void IIC_Send_Byte1(uint8_t txd);
/******* IIC读取一个字节  *************************/
uint8_t IIC_Read_Byte1(void);
/******* IIC等待ACK信号  *************************/
void IIC_Wait_Ack1(void);
/******* IIC发送ACK信号  *************************/
void IIC_Ack1(void);
/******* IIC不发送ACK信号  *************************/
void IIC_NAck(void);

#endif

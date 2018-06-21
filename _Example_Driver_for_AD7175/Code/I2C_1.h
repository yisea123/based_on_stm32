#ifndef __I2C_1
#define __I2C_1
//#include "sys.h"
//IO方向设置
//#define SDA_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=0X08000000;}
//#define SDA_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=0X03000000;}
//#define SDA_IN()  {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=0X00001000;}
//#define SDA_OUT() {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=0X00006000;}
//#define SDA_IN()  {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=0X00006000;}
//#define SDA_OUT() {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=0X00006000;}
//IO操作函数
#define IIC_SCL    PBout(5) //SCL
#define IIC_SDA    PBout(6) //SDA    
#define READ_SDA   PBin(6)  //输入SDA 
//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
void IIC_Start(void);               //发送IIC开始信号
void IIC_Stop(void);                //发送IIC停止信号
void IIC_Send_Byte(u8 txd);         //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void);              //IIC等待ACK信号
void IIC_Ack(void);                 //IIC发送ACK信号
void IIC_NAck(void);                //IIC不发送ACK信号
void IIC_Write_One_Byte(u8 daddr,u8 COMM,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);
u16 IIC_Read_TWO_Byte(u8 Address,u8 ReadAddr);
void IIC_ALS_intit(void);
u8 IIC_1_line_Dvice_find(u8 daddr);
#endif

//#include "GlobalDef.h"
//#define ulong unsigned long
#include "config.h"
//初始化IIC
void IIC_Init(void)
{
//    RCC->APB2ENR|=1<<3;//先使能外设IO PORTB时钟
//    GPIOB->CRL&=0XF00FFFFF;//PB6,7 推挽输出
//    GPIOB->CRL|=0X03300000;
//    GPIOB->ODR|=3<<5;     //PB6,7 输出高
//  GPIO_InitTypeDef  GPIO_InitStructure;
////  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);//???
    IIC_SCL=1;
    IIC_SDA=1;
}
void SDA_IN(void)
{
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SDA_OUT(void)
{
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//产生IIC起始信号
void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA=1;
    IIC_SCL=1;
    Delayus(50);
    IIC_SDA=0;//START:when CLK is high,DATA change form high to low
    Delayus(60);
    IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL=0;
    IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    Delayus(60);
    IIC_SCL=1;
    IIC_SDA=1;//发送I2C总线结束信号
    Delayus(60);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA_IN();      //SDA设置为输入
    IIC_SDA=1;
    Delayus(10);//实际10us(1);
    IIC_SCL=1;
    Delayus(10);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL=0;//时钟输出0
    return 0;
}
//产生ACK应答
void IIC_Ack(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=0;
    Delayus(20);
    IIC_SCL=1;
    Delayus(50);
    IIC_SCL=0;
}
//不产生ACK应答
void IIC_NAck(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=1;
    Delayus(20);
    IIC_SCL=1;
    Delayus(50);
    IIC_SCL=0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1;
        Delayus(20);   //对TEA5767这三个延时都是必须的
        IIC_SCL=1;
        Delayus(20);
        IIC_SCL=0;
        Delayus(20);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        IIC_SCL=0;
        Delayus(20);
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)
        {
            receive++;
        }
        Delayus(10);
    }
    if (!ack)
    {
        IIC_NAck();    //发送nACK
    }
    else
    {
        IIC_Ack();    //发送ACK
    }
    return receive;
}
//u8 IIC_Read_One_Byte(u8 Address,u8 ReadAddr)
//{
//      u8  temp;
//
//      IIC_Start();
//      IIC_Send_Byte((Address<<1)&0xfe);                   //发送器件地址0XA0,写数据
//      IIC_Send_Byte(Address);                   //发送器件地址0XA0,写数据
//      IIC_Wait_Ack();
//
//    IIC_Send_Byte(ReadAddr);               //发送低地址
//      IIC_Wait_Ack();
//      IIC_Start();
//      IIC_Send_Byte((Address<<1)|0x01);           //进入接收模式
//  //  IIC_Send_Byte(Address|0x01);           //进入接收模式
//      IIC_Wait_Ack();
//      temp=IIC_Read_Byte(0);
//  //  IIC_NAck();
//      IIC_Stop();
//      Delayus(10);                             //延时一段时间不然数据写不成功
//      return temp;
//}
u8 IIC_Read_One_Byte(u8 Address,u8 ReadAddr)
{
    u8  temp;
    IIC_Start();
    IIC_Send_Byte((Address<<1)&0xfe);                   //发送器件地址0XA0,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr);               //发送低地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((Address<<1)|0x01);           //进入接收模式
    IIC_Wait_Ack();
    temp=IIC_Read_Byte(0);
    IIC_Stop();
    Delayus(100);                            //延时一段时间不然数据写不成功
    return temp;
}
void IIC_Write_One_Byte(u8 daddr,u8 COMM,u8 data)
{
    IIC_Start();
    IIC_Send_Byte((daddr<<1)&0xfe);                   //发送器件地址0XA0,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(COMM);               //发送低地址
    IIC_Wait_Ack();
    IIC_Send_Byte(data);
    IIC_Wait_Ack();
    IIC_Stop();
    Delayus(100);                            //延时一段时间不然数据写不成功
}
u16 IIC_Read_TWO_Byte(u8 Address,u8 ReadAddr)
{
    u8  tempH,tempL;
    IIC_Start();
    IIC_Send_Byte((Address<<1)&0xfe);                   //发送器件地址0XA0,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr);               //发送低地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((Address<<1)|0x01);           //进入接收模式
    IIC_Wait_Ack();
    tempL=IIC_Read_Byte(1);
    tempH=IIC_Read_Byte(0);
    IIC_Stop();
    Delayus(100);                            //延时一段时间不然数据写不成功
    return (tempH<<8)+tempL;
}

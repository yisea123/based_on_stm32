//模拟IIC
//#include"IIC.h"
//#include "systickdelay.h"
#include "config.h"
/********************************************************
 * @brief  IIC_Init program.  //初始化IIC
 * @param  None
 * @retval : None
 ********************************************************/
void IIC_Init1(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SDA_IN1(void)
{
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SDA_OUT1(void)
{
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/********************************************************
 * @brief  IIC_Start program.  //产生IIC起始信号
 * @param  None
 * @retval : None
 ********************************************************/
void IIC_Start1(void)
{
    SDA_OUT1();     //sda线输�?
    IIC_SDA_SET;
    Delayus(4);
    IIC_SCL_SET;
    Delayus(4);
    IIC_SDA_CLR;//START:when CLK is high,DATA change form high to low
    Delayus(4);
    IIC_SCL_CLR;//钳住I2C总线，准备发送或接收数据
}
/********************************************************
 * @brief  IIC_Stop program. //产生IIC停止信号
 * @param  None
 * @retval : None
 ********************************************************/
void IIC_Stop1(void)
{
    SDA_OUT1();     //sda线输�?
    IIC_SDA_CLR;//STOP:when CLK is high DATA change form low to high
    Delayus(4);
    IIC_SCL_SET;
    Delayus(4);
    IIC_SDA_SET;//发送I2C总线结束信号
    Delayus(4);
}
/********************************************************
 * @brief  IIC_Wait_Ack program.  //等待从机的应答信号到�?
 * @param  None//在scl为高电平时slave将sda拉低，产生应答，但是scl也得拉低
 * @retval : None
 ********************************************************/
void IIC_Wait_Ack1(void)
{
    uint8_t i=0;
    SDA_IN1();     //sda线输�?
    IIC_SCL_SET;
    Delayus(4);
    while((IIC_SDA1==1)&&(i<250))
    {
        i++;
    }
    IIC_SCL_CLR;
    Delayus(4);
}
/********************************************************
 * @brief  IIC_Ack program.  //主机产生ACK应答
 * @param  None     //sda�?时将scl拉低，即产生主机应答
 * @retval : None
 ********************************************************/
void IIC_Ack1(void)
{
    SDA_OUT1(); //sda线输�?
    IIC_SDA_CLR;
    Delayus(4);
    IIC_SCL_SET;
    Delayus(4);
    IIC_SCL_CLR;
    Delayus(4);
}
/********************************************************
 * @brief  IIC_NAck program.  //主机不产生ACK应答
 * @param  None
 * @retval : None
 ********************************************************/
void IIC_NAck1(void)
{
    SDA_OUT1();   //sda线输�?
    IIC_SDA_SET;
    Delayus(4);
    IIC_SCL_SET;
    Delayus(4);
    IIC_SCL_CLR;
}
/********************************************************
 * @brief  IIC_Send_Byte program.  //IIC写一个字�?
 * @param  txd : 写字节参�?
 * @retval : None
 ********************************************************/
void IIC_Send_Byte1(uint8_t txd)
{
    uint8_t t;
    SDA_OUT1();        //sda线输�?
    IIC_SCL_CLR;   //拉低时钟开始数据传�?
    for(t=0; t<8; t++)
    {
        IIC_SDA1=(txd&0x80)>>7;
        txd<<=1;
        Delayus(4);
        IIC_SCL_SET;
        Delayus(4);
        IIC_SCL_CLR;
        Delayus(4);
    }
}
/********************************************************
 * @brief  IIC_Read_Byte program.  //�?个字�?
 * @param  None
 * @retval  receive  ：读出的一个字�?
 ********************************************************/
uint8_t IIC_Read_Byte1(void)
{
    uint8_t i,receive=0;
    SDA_IN1();    //SDA设置为输�?
    for(i=0; i<8; i++ )
    {
        IIC_SCL_CLR;
        Delayus(4);
        IIC_SDA_SET;
        for(i=0; i<8; i++)
        {
            IIC_SCL_SET;
            Delayus(4);
            receive=(receive<<1) | READ_SDA1;   //READ_SDA
            IIC_SCL_CLR;
            Delayus(4);
        }
    }
    return receive;
}

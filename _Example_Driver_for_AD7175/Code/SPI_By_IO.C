#include "config.h"
//#include "SPI_By_IO.h"
//#include "usart.h"
void SPI_By_IO_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(SPI_GPIO_Port_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //SPI_CS
    GPIO_InitStructure.GPIO_Pin = SPI_CS_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStructure);
    SPI_CS_1();                                         //cs初始电平为高
    //SPI_CLK
    GPIO_InitStructure.GPIO_Pin = SPI_CLK_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_CLK_GPIO_Port, &GPIO_InitStructure);
    SPI_CLK_0();                                        //CLK初始电平为低
    //SPI_MISO
    GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_MISO_GPIO_Port, &GPIO_InitStructure);
    //SPI_MOSI
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_MOSI_GPIO_Port, &GPIO_InitStructure);
    SPI_MOSI_1();                                   //MOSI初始电平为高
}
void SPI_Send_One_Byte(uint8_t data)
{
    u8 i=0;
    for(i=0; i<8; i++)
    {
        SPI_CLK_1();                    //初始状态时钟为高电平
        SPI_CLK_Delay();
        SPI_CLK_0();                    //CLK下降沿，数据线路可改变状态
        if( data & 0x80 )           //MSB_First
        {
            SPI_MOSI_1();
        }
        else
        {
            SPI_MOSI_0();
        }
        SPI_CLK_Delay();
        SPI_CLK_1();                    //上升沿，数据传输给从机
        data=data<<1;                   //待发送数据左移，以发送下一位数据
    }
    SPI_CLK_Delay();            //一个字节数据发送完毕，做延时
}
u8 SPI_Read_One_Byte(void)
{
    u8 i=0;
    u8 dat=0;
//  if(Read_SPI_MISO())
    for(i=0; i<8; i++)
    {
        dat=dat<<1;                     //数据缓冲变量左移，以准备接受下一bit数据
        SPI_CLK_1();                    //初始状态均为高电平
        SPI_CLK_Delay();
        SPI_CLK_0();                    //CLK下降沿，从机可以改变数据线路电平
        SPI_CLK_Delay();            //低电平保持
        SPI_CLK_1();                    //上升沿，数据传输给从机
        SPI_CLK_Delay();            //做延时后再读取数据
        if( Read_SPI_MISO() )   //若读取到数据为1
        {
            dat= dat|0x01;   //存储
        }
    }
    SPI_CLK_Delay();
    return  dat;                        //返回接收到的结果数据
}


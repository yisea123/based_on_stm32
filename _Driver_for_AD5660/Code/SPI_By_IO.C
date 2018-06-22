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
    SPI_CS_1();                                         //cs��ʼ��ƽΪ��
    //SPI_CLK
    GPIO_InitStructure.GPIO_Pin = SPI_CLK_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_CLK_GPIO_Port, &GPIO_InitStructure);
    SPI_CLK_0();                                        //CLK��ʼ��ƽΪ��
    //SPI_MISO
    GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_MISO_GPIO_Port, &GPIO_InitStructure);
    //SPI_MOSI
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_MOSI_GPIO_Port, &GPIO_InitStructure);
    SPI_MOSI_1();                                   //MOSI��ʼ��ƽΪ��
}
void SPI_Send_One_Byte(u8 data)
{
    u8 i=0;
    for(i=0; i<8; i++)
    {
        SPI_CLK_1();                    //��ʼ״̬ʱ��Ϊ�ߵ�ƽ
        SPI_CLK_Delay();
        SPI_CLK_0();                    //CLK�½��أ�������·�ɸı�״̬
        if( data & 0x80 )           //MSB_First
        {
            SPI_MOSI_1();
        }
        else
        {
            SPI_MOSI_0();
        }
        SPI_CLK_Delay();
        SPI_CLK_1();                    //�����أ����ݴ�����ӻ�
        data=data<<1;                   //�������������ƣ��Է�����һλ����
    }
    SPI_CLK_Delay();            //һ���ֽ����ݷ�����ϣ�����ʱ
}
u8 SPI_Read_One_Byte(void)
{
    u8 i=0;
    u8 dat=0;
//	if(Read_SPI_MISO())
    for(i=0; i<8; i++)
    {
        dat=dat<<1;                     //���ݻ���������ƣ���׼��������һbit����
        SPI_CLK_1();                    //��ʼ״̬��Ϊ�ߵ�ƽ
        SPI_CLK_Delay();
        SPI_CLK_0();                    //CLK�½��أ��ӻ����Ըı�������·��ƽ
        SPI_CLK_Delay();            //�͵�ƽ����
        SPI_CLK_1();                    //�����أ����ݴ�����ӻ�
        SPI_CLK_Delay();            //����ʱ���ٶ�ȡ����
        if( Read_SPI_MISO() )   //����ȡ������Ϊ1
        {
            dat= dat|0x01;   //�洢
        }
    }
    SPI_CLK_Delay();
    return  dat;                        //���ؽ��յ��Ľ������
}

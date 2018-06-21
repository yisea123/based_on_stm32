//#define ADS8320_GLOBALS
//#include "GlobalDef.h"
//extern GPIO_InitTypeDef GPIO_InitStructure;

#include "main.h"
/************************
����:spi�ӿڳ�ʼ��
�汾:V1.00   ����:13.07.05
�޸�:xiaodong.liu
����:value
 ************************/
void ADS8320_Config(void)
{
/*
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIOB->CRH &=0XCFFFFFFF;
    GPIOB->CRH |=0X20000000;
    // GPIOC Periph clock enable /
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // PC output pushpull mode
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;//
    GPIO_Init(GPIOB, &GPIO_InitStructure);
*/
}
/************************
??:?ADS8320???,??SPI??
??:V1.00   ??:09.08.17
??:
??:value
//#define SDA_OUT   (GPIOB->CRH |=0X02000000,))
//#define SDA_IN    ((GPIOB->CRH &=0XFCFFFFFF,))
 ************************/

uint ADC_read(void)
{
    uchar i=0;
    uint readdata=0;
    //SDA_OUT;
//    GPIOB->CRH &=0X0FFFFFFF;//output PB15
//    GPIOB->CRH |=0X60000000;//output PB15
	DDRA |= (1<<PA2);
    PORTA |= (1<<PA2);
    delayus(1);
    CS_AD8320;
    delayus(1);
    SCL_H;
    SDA_H;
    delayus(1);
    for (i = 0; i <5; i++)
    {
        SCL_L;
        delayus(1);
        SCL_H;
        delayus(1);
    }
    //SDA_IN;
//    GPIOB->CRH &=0X0FFFFFFF;//PB15 INPUT
//    GPIOB->CRH |=0X40000000;//PB15 INPUT
	DDRA &= ~(1<<PA2);
    PORTA |= (1<<PA2);
    delayus(1);
    for (i = 0; i < 16; i++)
    {
        SDA_H;
        SCL_L;
        delayus(1);
        //if((GPIOB->IDR & GPIO_Pin_15)==0)
		if(TestSDA == 0)
        {
			USART_Transmit_byte(0x13); 
            readdata <<= 1;
            readdata = (readdata & 0xfffe);
            SCL_H;
        }
        else
        {
            readdata <<= 1;
            readdata = (readdata | 0x0001);
            delayus(1);
            SCL_H;
        }
    }
    for (i = 0; i < 5; i++)
    {
        SCL_L;
        delayus(1);
        SCL_H;
        delayus(1);
    }
    delayus(1);
    SCL_L;
    CS_DISABLEALL;
 //   delayus(1);
    //GPIOB->CRH |=0X06000000;//output PB14
//    delayus(1);
//   UartSend1Byte(USART1,((readdata>>8)&0xFF));
//   UartSend1Byte(USART1,((readdata>>0)&0xFF));
    return (readdata);
}
/************************
����:ѭ����ȡADS8320�����ݣ�
�汾:V1.00   ����:09.08.17
�޸�:xiaodong.liu
����:value
ѭ����ȡADS8320��ֵ
count:ѭ����ȡ����
�����ֵ,��Сֵ����ƽ��ֵ
countҪС��readdata[200]
 ************************/
float ReadADCData(float count)
{
    uint i;
    float adc = 0;
    float MaxData = 0;//TRANadc=0;
    float MinData = 0;
    float readdata[200];
    for (i = 0; i < count; i++) //ѭ����ȡ����
    {
        readdata[i] = ADC_read(); //AD��ȡ;
		NOP();NOP();NOP();
        //__NOP();
        //__NOP();
        //__NOP();
    }
    for (i = 1; i < count; i++)
    {
        //MaxData = max(MaxData, readdata[i]); //�����ֵ
        //MinData = min(MinData, readdata[i]); //����Сֵ
        adc += (ulong)readdata[i]; //���ܺ�
    }
    //adc = (adc - (ulong)MinData - (ulong)MaxData) / (count - 2); //�����ֵ,��Сֵ����ƽ��ֵ
	adc = adc/count;
/*	
    if(Openmark==Open)
    {
        ;
    }
    else
    {
        //if((TestPar.Type=='D')&&(adc>21845))Openmark=Open;//����ǲ��ѹ����5V���ǳ���
        if(adc>34952)
        {
            Openmark=Open;    //����ǲ��ѹ����8V���ǳ���
        }
        else
        {
            Openmark=NotOpen;
        }
    }
*/	
    return (adc);//return mv
}

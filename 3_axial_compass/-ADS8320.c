//#define ADS8320_GLOBALS
//#include "GlobalDef.h"
//extern GPIO_InitTypeDef GPIO_InitStructure;

#include "main.h"
/************************
功能:spi接口初始化
版本:V1.00   日期:13.07.05
修改:xiaodong.liu
作者:value
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
功能:循环读取ADS8320的数据，
版本:V1.00   日期:09.08.17
修改:xiaodong.liu
作者:value
循环读取ADS8320的值
count:循环读取次数
减最大值,最小值再求平均值
count要小于readdata[200]
 ************************/
float ReadADCData(float count)
{
    uint i;
    float adc = 0;
    float MaxData = 0;//TRANadc=0;
    float MinData = 0;
    float readdata[200];
    for (i = 0; i < count; i++) //循环读取数据
    {
        readdata[i] = ADC_read(); //AD读取;
		NOP();NOP();NOP();
        //__NOP();
        //__NOP();
        //__NOP();
    }
    for (i = 1; i < count; i++)
    {
        //MaxData = max(MaxData, readdata[i]); //求最大值
        //MinData = min(MinData, readdata[i]); //求最小值
        adc += (ulong)readdata[i]; //求总和
    }
    //adc = (adc - (ulong)MinData - (ulong)MaxData) / (count - 2); //减最大值,最小值再求平均值
	adc = adc/count;
/*	
    if(Openmark==Open)
    {
        ;
    }
    else
    {
        //if((TestPar.Type=='D')&&(adc>21845))Openmark=Open;//如果是测电压大于5V便是超载
        if(adc>34952)
        {
            Openmark=Open;    //如果是测电压大于8V便是超载
        }
        else
        {
            Openmark=NotOpen;
        }
    }
*/	
    return (adc);//return mv
}

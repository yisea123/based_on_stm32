//#define AD9833_GLOBALS
//#include "GlobalDef.h"
#include "config.h"
extern GPIO_InitTypeDef GPIO_InitStructure;
/************************
功能:读AD9833的数据，SPI方式
版本:V1.00   日期:13.09.07
修改:xiaodong.liu
作者:value
//参考产品：http://blog.sina.com.cn/s/blog_6d637a3f0100qfnd.html
************************/
void Write_word(unsigned int DDSdata)
{
    unsigned char data_num=0;
    unsigned int DDSdata_temp;
//    /********************增加配置PB13、PB15为浮空输出*********/
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    /********************2012/12/10************************/
    DDSdata_temp=DDSdata;
    SCL_H;
    Delayus(4);
    AD9833_H;
    Delayus(4);
    AD9833_L;
    Delayus(4);
    for(data_num=0; data_num<16; data_num++)
    {
        if(DDSdata_temp&0x8000)
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }
        Delayus(5);
        SCL_L;
        Delayus(5);
        SCL_H;
        DDSdata_temp=DDSdata_temp<<1;
    }
    AD9833_H;
}

/************************
功能:读AD9833 init
版本:V1.00   日期:13.09.07
修改:xiaodong.liu
作者:value
 ************************/
void init_ad9833(void)
{
    Write_word(0x2100); //28???,????0,??0,RESET=1
    //Write_word(0x2000);   //28???,????0,??0,RESET=1
    Write_word(0x4000); //???0???????LSB
    Write_word(0x4000); //???0???????MSB
    Write_word(0x2900); //28???,????1,??0,RESET=1
    Write_word(0x8000); //???1???????LSB
    Write_word(0x8000); //???1???????MSB
    Write_word(0xC000); //???0???????LSB
    Write_word(0xF000); //???0???????MSB
    Write_word(0x2000); //28???,????0,??0,RESET=0
//  CS_DISABLEALL;
}
/************************
功能:AD9833 输出指定频率的正旋波
版本:V1.00   日期:13.09.07
修改:xiaodong.liu
作者:value
fout=(fMCLK/2^28)*FREQREG
FREQREG=fout*2^28/fMCLK
 ************************/
void output(uint32_t freq_value)//单位 1HZ
{
    uint32_t dds;
    uint16_t dds_l,dds_h;
    dds= freq_value *26.8435456; //268435456/ FMCLK
    dds_l=(dds&0x3fff)|0x4000;  //???(??0)
    dds_h=((dds>>14)&0x3fff)|0x4000;//???(??0)
    //Write_word(0x2000);   //28???,????0,??0,RESET=0
    //Write_word(0xC400);   //???0???????LSB
    //Write_word(0xF100);   //???0???????MSB
    Write_word(0x2000); //28???,????0,??0,RESET=0
    Write_word(dds_l);
    Write_word(dds_h);
//  CS_DISABLEALL;
}

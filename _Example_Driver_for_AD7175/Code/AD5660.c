#include "config.h"
//void SPI_AD5660_Init(void)
//{
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5| GPIO_Pin_6|\
//                                  GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//}
void SPI_AD5660_Init(void)
{
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_10| GPIO_Pin_11|\
                                  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    SET_SCLK();
    SET_SYNC();
    SET_DIN();
}
void Wr_AD5660(unsigned char *Buf,unsigned char count)
{
    unsigned    char    ValueToWrite = 0;
    unsigned    char    i = 0;
    unsigned    char    j = 0;
    SET_SCLK();
    CLR_SYNC();
    Delayms(1);
    for ( i=count; i>0; i-- )
    {
        ValueToWrite =  *(Buf+i-1);
        //printf("wr_buf=%x\t",ValueToWrite);
        for (j=0; j<8; j++)
        {
            SET_SCLK();
            Delayus(30);
            if(0x80 == (ValueToWrite & 0x80))
            {
                SET_DIN();
            }
            else
            {
                CLR_DIN();
            }
            Delayus(30);
            CLR_SCLK();
            Delayus(30);
            ValueToWrite <<= 1;
        }
    }
    SET_SCLK();
    Delayus(30);
    SET_SYNC();
    Delayus(30);
    SET_DIN();
    Delayms(1);
}
//void Wr_AD5660(unsigned char *Buf,unsigned char count)
//{
//  unsigned    char    ValueToWrite = 0;
//    unsigned  char    i = 0;
//  unsigned    char    j = 0;
//
//  SET_SCLK();
//  SET_SYNC();
//  Delayms(1);
//  CLR_SYNC();
//  Delayms(1);
//  for ( i=count;i>0;i-- )
//  {
//      ValueToWrite =  *(Buf+i-1);
////        printf("wr_buf=%x\r\n",ValueToWrite);
//      for (j=0; j<8; j++)
//      {
//          SET_SCLK();
//          Delayms(1);
//          if(0x80 == (ValueToWrite & 0x80))
//          {
//              SET_DIN();
//          }
//          else
//          {
//              CLR_DIN();
//          }
//
//          Delayms(1);
//          CLR_SCLK();
//          Delayms(1);
//          ValueToWrite <<= 1;
//      }
//  }
//  SET_SYNC();
//  Delayms(2);
//}
//void AD5660_Out(uint16_t Current)
//{
//  uint8_t ad5660_wr_buf[3];
//  uint16_t DA_Value;
//  DA_Value = (65535*Current)/5000;
//  ad5660_wr_buf[2]=0;
//  ad5660_wr_buf[1]=DA_Value>>8;
//  ad5660_wr_buf[0]=DA_Value;
//
//  Wr_AD5660(ad5660_wr_buf,3);
////    printf("ad5660_wr_buf=%s",ad5660_wr_buf);
//}
void AD5660_Out(uint16_t Current)
{
    uint8_t ad5660_wr_buf[3];
    uint16_t DA_Value;
    DA_Value = (65535 * Current)/(uint16_t)(2 * 2.5 * 1000);
    ad5660_wr_buf[2]=0x00;
    ad5660_wr_buf[0]=DA_Value;
    ad5660_wr_buf[1]=DA_Value>>8;
    Wr_AD5660(ad5660_wr_buf,3);
    printf("AD5660 Set Voltage is: %d\r\n" ,Current);
//    printf("AD5660 Buff[0] is:  %02X\r\n" ,ad5660_wr_buf[0]);
//    printf("AD5660 Buff[1] is: %02X\r\n" ,ad5660_wr_buf[1]);
//    printf("AD5660 Buff[2] is:  %02X\r\n" ,ad5660_wr_buf[2]);
}

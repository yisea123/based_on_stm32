
#include "main.h"

void ADS8320_init(void)
{
 	ADS8320_DDR|=(1<<ADS8320_CS)|(1<<ADS8320_CLOCK);
	ADS8320_DDR&=~(1<<ADS8320_DOUT);
	ADS8320_PORT|=(1<<ADS8320_CS)|(1<<ADS8320_CLOCK)|(1<<ADS8320_DOUT);
}

uint Read_ADS8320(void)
{
 	 uchar i;
	 uint data=0;
	 ADS8320_CLOCK_L;
	 ADS8320_CS_L;
	 while(1)	  	 									 		//ADS8320 ǰ5��ʱ��Ϊģ��������
	 {
	  	ADS8320_CLOCK_H;
		NOP();NOP();	
		if((ADS8320_Dat(PA2))==0)										//DOUT �͵�ƽ����������־
		{
			ADS8320_CLOCK_L;
			NOP();NOP();
			break;
		}
		else if(++i==10)										//����10��ʱ�ӳ�ʼ�������ʧ��
		{
		 	 i=0;
			 Transmit_arraydata("ADS8320 Failed to initialize\r\n");
			 return 0;
		}
		ADS8320_CLOCK_L;
		NOP();NOP();
	 }
	 for(i=0;i<16;i++)								   	  		//��ʼ������
	 {
	  	ADS8320_CLOCK_H;
		NOP();NOP();
		data=(data<<1)|(ADS8320_Dat(ADS8320_DOUT));								//ADS8320_Dat(ADS8320_DOUT);
		ADS8320_CLOCK_L;
		NOP();NOP();
	 }
	 ADS8320_CS_H;
	 return (data);
}

uint ADS8320_Smoothing(uchar time,uint ms)
{
	  long AD_sum=0;
	  uint AD_max=0,AD_min=0xffff,AD_data=0;
	  uchar i;
	 for(i=0;i<time;i++)
	 {
	  	AD_data=Read_ADS8320();
		if(AD_data > AD_max) AD_max = AD_data;
	    if(AD_data < AD_min) AD_min = AD_data;
	    AD_sum += AD_data;
		delayms(ms);
	 }
	 return ((AD_sum - AD_max - AD_min)/(time - 2));
}

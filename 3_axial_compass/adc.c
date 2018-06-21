//=============================2013-10-29  更新

#include "main.h"

extern void delayms(unsigned int ms);
/*
void ADC_init(unsigned char ADC_Channel)
{

	if(ADC_Channel==0)
	{
		ADMUX=0x00;
	}
	if(ADC_Channel==1)
	{
		ADMUX=0x01;
	}	
	ADMUX=0x00;
    ADCSRA = 0x00; 
    DDRF&=~(BIT(0)|BIT(1));
    ACSR  = 0x80;	//模拟比较器状态.
    ADCSRB = 0x00;
    ADCSRA = (1<<(ADEN))|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
*/
void ADC_init(void)
{
DDRF&=~(BIT(0)|BIT(1));
//ADMUX=0xc0;//1100 0000
ADMUX = 0xC0;
ADCSRA = 0x00; 
ACSR  = 0x80;	//模拟比较器状态.
ADCSRB = 0x00;
//ADMUX | = (1<<REFS1)|(1<<REFS0);2.56
//ADCSRA = (1<<(ADEN))|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
unsigned int adc_converter(void)
{
	unsigned int V_voltage=0;
	unsigned int ADC_result=0; 
    ADCSRA|=(1<<(ADSC));//使能ADC并开始转换
	//ADMUX | = (1<<REFS1)|(1<<REFS0);2.56
    while(!(ADCSRA&(1<<(ADIF))));//查询转换完成标志位
	ADCSRA|=(1<<ADIF);
    ADC_result=ADCL;
    ADC_result|=(unsigned int)ADCH<<8;
    V_voltage=ADC_result*(2560.0/1024.0);//2560 基准电压需要根据实际测量
	return V_voltage;
}

unsigned int adc_converter_multiple(void)
{
	unsigned int a[10]={0};
	unsigned int xx=0;
	unsigned char i=0,j=0;
	unsigned int yy=0;
	unsigned int ADC_average=0;
ADCSRA = (1<<(ADEN))|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	for(i=0;i<10;i++)
	{
		a[i]=adc_converter();
		delayms(100);
	}
	ADCSRA = 0;
	/*
	*******From max to min: a[0]=max.....a[9]=min
	*/
	for(i=0;i<10;i++)
	{
		for(j=9;j>i;j--)
		{
			if(a[j]>a[j-1])
			{
				xx=a[j-1];
				a[j-1]=a[j];
				a[j]=xx;
			}
		}
	}
	/*
	********Delete max and min, other 8 nums add and average
	*/
	for(i=1;i<9;i++)
	{
		yy+=a[i];
	}
	ADC_average=(yy+0.00)/8.0;
	
	return ADC_average;
	
	
	/*
	*******Display 10 num from a[i] arrays ,from max to min
	*/
	/*for(i=0;i<10;i++)
	{
    	adc_measure_data[0]=a[i]/1000+'0';
        adc_measure_data[1]='.';
        adc_measure_data[2]=a[i]%1000/100+'0';
        adc_measure_data[3]=a[i]%1000%100/10+'0';
        adc_measure_data[4]=a[i]%10+'0';
        adc_measure_data[5]='v';
        adc_measure_data[6]='\r';
        send_uart1_string(adc_measure_data);
	}
	uart1_send_data('\r');
    uart1_send_data('\n');
	*/
}
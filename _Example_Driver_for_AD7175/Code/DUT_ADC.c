#include "config.h"
/********************************************************/
uint8_t Buf_AD[8];
uint8_t BufRead[4];
uint8_t BufRead_100[300];

//=====================================================================
void DecoderRead(uint8_t * string1)
{
	char a[1];
	uint8_t i;
	uint8_t TemValu;
	char *p[20];
	uint8_t BufTab[8];
	uint8_t j;
	uint16_t temp;
	uint8_t in=0; 
	char *buf = ReceBUFF1Pre;
	
	for(i=0;i<ReceLen1-3;i++)
	{
		ReceBUFF1Pre[i] = ReceBUFF1Pre[5+i];
	}
	while ((p[in] = strtok(buf, " ,")) != NULL)  
	{  
		in++;  
		buf = NULL;  
	}         
	SPI2_CS_0();
	TemValu = strtol(p[0],NULL,16); 
	SPI2_Send_One_Byte(TemValu);
	TemValu = strtol(p[1],NULL,16);
	SPI2_CS_1();
	Delayus(10);
	SPI2_CS_0();
	for(i=0;i<TemValu;i++)
	{
		j = SPI2_Read_One_Byte();
		printf("data=%x\r\n",j);
	}	
	SPI2_CS_1();
}

void Decoder(uint8_t * string1)
{
	char a[1];
	uint8_t i;
	char *p[20];
	uint8_t BufTab[8];
	uint8_t j;
	uint16_t temp;
	uint8_t in=0; 
	char *buf = ReceBUFF1Pre;
	
//	printf("buf=%s\r\n",buf);
	for(i=0;i<ReceLen1-3;i++)
	{
		ReceBUFF1Pre[i] = ReceBUFF1Pre[5+i];
	}
	while ((p[in] = strtok(buf, " ,")) != NULL)  
	{  
		in++;  
		buf = NULL;  
	}  
	for (j=0; j<in; j++)  
	{         
		SPI2_CS_0();
		a[0] = strtol(p[j],NULL,16); 
		SPI2_Send_One_Byte(a[0]);
		printf("data=%x\r\n",a[0]);
		SPI2_CS_1();
	}	
}
//====================================================================

uint32_t Complement_ADC(uint32_t data)
{
	uint32_t Tem;
	
//	if((data&0x80000000) == 0x80000000)
	data = (data&0x00ffffff);
	if(data> 0x7fffff)
	{
		
		Tem = data;
		data =~ Tem; 
		data = data+1;
	}
	data = (data&0x00ffffff);
	return data;
}
//======================================================================
float data3ToInt(uint8_t *data)
{
	uint32_t Data32a,Data32;
	float Vx;
	
	Data32a = data[0];
	Data32a = Data32a<<8; 
	Data32a = Data32a|data[1];
	Data32a = Data32a<<8; 
	Data32a = Data32a|data[2];
	Data32a = (Data32a&0x00ffffff);
				
	Data32 = Complement_ADC(Data32a);

//	printf("receive2=%x\r\n",Data32);	
	if(Data32 > 0x007FFFFF)
	{
		Vx = -1*(float)(Data32*3)/8388608;
	}
	else
	{
		Vx = (float)(Data32*3)/8388608;
//	printf("Data_From_ADC=%f\r\n",Vx);
	}

	return Vx;
}


//==========================================================================



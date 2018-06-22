
#include "SPI_By_IO.h"
//#include "usart.h"
#include "config.h"

void SPI_By_IO_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(SPI_GPIO_Port_RCC, ENABLE);	

    //SPI_CS
    GPIO_InitStructure.GPIO_Pin = SPI_CS_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStructure);
    SPI_CS_1();											

    //SPI_CLK	
    GPIO_InitStructure.GPIO_Pin = SPI_CLK_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Init(SPI_CLK_GPIO_Port, &GPIO_InitStructure);
    SPI_CLK_1();									

    //SPI_MISO	
    GPIO_InitStructure.GPIO_Pin = SPI_MISO_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_MISO_GPIO_Port, &GPIO_InitStructure);

    //SPI_MOSI	
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Init(SPI_MOSI_GPIO_Port, &GPIO_InitStructure);
    SPI_MOSI_1();								
		
		//AD_SYNC	
//    GPIO_InitStructure.GPIO_Pin = AD7175_SYNC_Pin;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(AD7175_SYNC_Port, &GPIO_InitStructure);
//		//AD_SYNC_0();								
//		AD_SYNC_1();		
}
void Reset64CLK(void)
{
		u8 i=0;
		for(i=0;i<65;i++)
	{
		SPI_CLK_0();				
		SPI_CLK_Delay();
		SPI_CLK_1();				
		SPI_CLK_Delay();
	}
}
void SPI_Send_One_Byte(u8 data)
{		u8 i=0;
    SPI_CS_0();
    SPI_CLK_Delay();
		for(i=0;i<8;i++)
		{
			SPI_CLK_1();					
			SPI_CLK_Delay();
			
			SPI_CLK_0();				
			if( data & 0x80 )			//MSB_First
				{	SPI_MOSI_1();}
			else {	SPI_MOSI_0();}
			SPI_CLK_Delay();

			SPI_CLK_1();				
			data=data<<1;				
		}
    SPI_CLK_Delay();			
    SPI_CS_0();
    SPI_CLK_Delay();
}

u8 SPI_Read_One_Byte(void)
{
    SPI_CS_0();
    SPI_CLK_Delay();
		u8 i=0;
		u8 dat=0;

		for(i=0;i<8;i++)
		{
			dat=dat<<1;					
			SPI_CLK_1();				
			SPI_CLK_Delay();
			
			SPI_CLK_0();				
			SPI_CLK_Delay();			
			
			SPI_CLK_1();				
			SPI_CLK_Delay();			
			if( Read_SPI_MISO()	)	
				{	dat= dat|0x01;}		
		}
			SPI_CLK_Delay();
        SPI_CS_1();
		return	dat;						
}



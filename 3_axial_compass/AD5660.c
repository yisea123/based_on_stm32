//2015-1-13,glp

#include "main.h"

unsigned char ad5660_wr_buf[3];
///////////////////////////////////////////////
//#define setbit(var, bits) ((var) |= (1 << (bits)))       
//#define clrbit(var, bits) ((var) &= ~(1 << (bits)))    
///////////////////////////////////////////////
#define   SET_DIN()   setbit(PORTF,PF2);
#define   CLR_DIN()   clrbit(PORTF,PF2);
#define   SET_SCLK()  setbit(PORTF,PF3);
#define   CLR_SCLK()  clrbit(PORTF,PF3);
#define   SET_SYNC()  setbit(PORTF,PF4);
#define   CLR_SYNC()  clrbit(PORTF,PF4);
///////////////////////////////////////////////
#define SPI_DIN_PIN     		BIT(2)
#define SPI_DIN_PORTx   		PORTF
#define SPI_DIN_DDRx    		DDRF
#define SPI_DIN_PINx    		PINF

#define SPI_SCLK_PIN     BIT(3)
#define SPI_SCLK_PORTx   PORTF
#define SPI_SCLK_DDRx    DDRF
#define SPI_SCLK_PINx    PINF

#define SPI_SYNC_PIN     BIT(4)
#define SPI_SYNC_PORTx   PORTF
#define SPI_SYNC_DDRx    DDRF
#define SPI_SYNC_PINx    PINF



//#define 	SET_SYNC()		    	SPI_SYNC_PORTx |= SPI_SYNC_PIN
//#define 	CLR_SYNC()		   	 SPI_SYNC_PORTx &=~SPI_SYNC_PIN

//#define 	SET_DIN()	    		SPI_DIN_PORTx |= SPI_DIN_PIN
//#define 	CLR_DIN()	   	 	SPI_DIN_PORTx &=~SPI_DIN_PIN

//#define 	SET_SCLK()	    		SPI_SCLK_PORTx |= SPI_SYNC_PIN
//#define 	CLR_SCLK()	    		SPI_SCLK_PORTx &=~SPI_SYNC_PIN
///////////////////////////////////////////////

void delay (int length)
{
	while (length >0)
    	length--;
}

void AD5660Init()
{
	SPI_SYNC_DDRx|=SPI_SYNC_PIN;
	SPI_DIN_DDRx|=SPI_DIN_PIN;
	SPI_SCLK_DDRx|=SPI_SCLK_PIN;
}

void Wr_AD5660(unsigned char *Buf,unsigned char count)
{

	unsigned	char	ValueToWrite = 0;
    unsigned	char	i = 0;
	unsigned	char	j = 0;
	
	SET_SCLK();
	SET_SYNC();
	delay(1);
	CLR_SYNC();

	for ( i=count;i>0;i-- )
 	{
	 	ValueToWrite =	*(Buf+i-1);
		for (j=0; j<8; j++)		
		{
			SET_SCLK();
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_DIN();	   
			}
			else
			{
				CLR_DIN();	   
			}
										 
			delay(1);
			CLR_SCLK();
			delay(1);
			ValueToWrite <<= 1;	
		}

	}
	SET_SYNC();
	delay(20);
}

#ifndef _ADS8320_H_
#define _ADS8320_H_
#include "main.h"

#define ADS8320_CS 						PA3
#define ADS8320_CLOCK					PA1
#define ADS8320_DOUT					PA2

#define ADS8320_DDR 					DDRA
//#define ADS8320_DDRB					DDRB


#define ADS8320_PORT					PORTA
//#define ADS8320_PORTB					PORTB

#define ADS8320_CS_H					setbit(PORTA, PA3)
#define ADS8320_CS_L					clrbit(PORTA, PA3)

#define ADS8320_CLOCK_H				    setbit(PORTA, PA1)
#define ADS8320_CLOCK_L				    clrbit(PORTA, PA1)

//#define ADS8320_Dat					 	(PINB&BIT(PB7))    

#define Read_PortDIO_Bit(x)         ((PINA & (1<<(x))) ? 1 : 0)
#define ADS8320_Dat(_x)   			Read_PortDIO_Bit(_x)

void ADS8320_init(void);
uint Read_ADS8320(void);
uint ADS8320_Smoothing(uchar time,uint ms);






#endif
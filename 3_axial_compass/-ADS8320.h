#ifndef __ADS8320_H_
#define __ADS8320_H_


#define   SCL_H 		    setbit(PORTA,PA1)  
#define   SCL_L 		    clrbit(PORTA,PA1)
#define   SDA_H 		    setbit(PORTA,PA2)  
#define   SDA_L 		    clrbit(PORTA,PA2)
#define   TestSDA 		   	testbit(PINA,PA2)

#define   CS_AD8320 		setbit(PORTA,PA3)  
#define   CS_DISABLEALL 	clrbit(PORTA,PA3)



/*变量定义*/
/*函数*/
//void ADS8320_Config(void);
//uint16_t ADC_read(void);
//float ReadADCData(float count);
//#else
/*变量声明*/
/*函数声明*/
//extern void ADS8320_Config(void);
//extern uint16_t ADC_read(void);
//extern float ReadADCData(float count);
//#endif
#endif

/************************************************************************************************
* AD9834_SDA: data input, 16bit data
* AD9834_SCLK:  clock input, bit input AD9834 through every negtive trigger
* AD9834_FSYNC: command port, when AD9834_FSYNC voltage change to low, chip receive AD9834_SDATA's data
* Pin Define:
	1. PF1 -> FSYNC
	2. PF3 -> SCLK
	3. PF5 -> SDA

write protical: 
	1. write cotrol data
	2. write LSB | register address
	3. write MLB | register address

AD9834 Init Statues:
	1. Keep bit 14 and bit 15 is 0.
	2. disable output, set reset bit to 1
	3. select soft control model
	4. select to refresh all 28 bit data mode
	InitRegister = ((1 << 13) | (1 << 8)) = 0x2100
************************************************************************************************/


#ifndef __AD9834_H
#define __AD9834_H
#include "stm32f10x_conf.h"
//#include "math.h"
//#include "spi.h"
#include "delay.h"
#include "string.h"

/************************** sine wave ************************/
//1KHz words: 00068DC
#define _1KHz_MSB					(uint16_t)(0x0001 | 0x4000)
#define _1KHz_LSB					(uint16_t)(0x28dc | 0x4000)

//10KHZ words: 0041893
#define _10KHz_MSB					(uint16_t)(0x0010 | 0x4000)
#define _10KHz_LSB					(uint16_t)(0x1893 | 0x4000)

//500Hz words: 000346E
#define _500Hz_MSB					(uint16_t)(0x0000 | 0x4000)
#define _500Hz_LSB					(uint16_t)(0x346e | 0x4000)

#define AD9834_SYSTEM_COLCK 		10000000UL

#define GPIO_AD9834					GPIOE

#define FSYNC 						GPIO_Pin_1
#define SCLK 						GPIO_Pin_3
#define SDA 						GPIO_Pin_5

#define FreqRegister_0				0
#define FreqRegister_1				1
#define PhaseRegister_0				2
#define PhaseRegister_1				3

#define Wave_Sine					10
#define Wave_Triangular				11
#define Wave_Square					12


#define	SET_FSYNC()		    GPIO_WriteBit(GPIO_AD9834, FSYNC, Bit_SET)
#define CLR_FSYNC()         GPIO_WriteBit(GPIO_AD9834, FSYNC, Bit_RESET)

#define SET_SCLK()			GPIO_WriteBit(GPIO_AD9834, SCLK, Bit_SET)
#define CLR_SCLK()          GPIO_WriteBit(GPIO_AD9834, SCLK, Bit_RESET)

#define SET_SDA()			GPIO_WriteBit(GPIO_AD9834, SDA, Bit_SET)
#define CLR_SDA()			GPIO_WriteBit(GPIO_AD9834, SDA, Bit_RESET)

//AD9834_InitAddress: set reset register to 1, set output wave is sine.
extern uint16_t AD9834_InitAddress;			

void AD9834_PortInit(void);
void AD9834_Init(void);
void AD9834_WriteBytes(uint16_t bytes);
void AD9834_WaveSet(unsigned int WaveKind);
void AD9834_WaveFrequecySet(uint8_t FreqRegNum, uint32_t Frequency);
void OutputFreCalc(uint32_t Frequency);
void OutputPhaseCalc(uint16_t Phase);
void AD9834_WavePhaseSet(uint8_t PhaseRegNum, uint16_t Phase);
void AD9834_OutputSet(uint8_t FreqRegNum, uint32_t Frequency, uint16_t Phase);
void AD9834_OutputEnable(void);


#endif


/****************************** END LINE *********************************/


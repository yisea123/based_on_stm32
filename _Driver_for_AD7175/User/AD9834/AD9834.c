/*************************************************************************
*AD9834 chip description: 

Control step: 
			1. set AD9834_WaveSet
			2. AD9834_SetFrequecy
*************************************************************************/

#include "AD9834.h"

uint16_t AD9834_InitAddress			= (uint16_t)0x2100;
//uint16_t AD9834_BaseAddress			= (uint16_t)0x0000;									
//uint16_t AD9834_FreqReg0 			=  (uint16_t)(1<< 14);
//uint16_t AD9834_FreqReg1			= ((uint16_t)(1<< 15));			
//uint16_t AD9834_DispSet 			= ((uint16_t)0x0000);									//bit13; only need to refresh MSB or LSB
//uint16_t AD9834_AllSet 				= (uint16_t)(1<< 13);			//bit13; need to refresh all 28 bit data									//bit12; DB28 = 0
//uint16_t AD9834_LowDensity 			= (uint16_t)(1<< 12);			//bit12; DB28 = 1
////uint16_t AD9834_HLB					= ((uint16_t)0x0000);								//company with AD9834_LowDensity									//bit11; 	
//uint16_t AD9834_FSLE1				= (uint16_t)(1 << 11);			//bit11;									//bit10;
//uint16_t AD9834_PSEL				= (uint16_t)(1 << 10); 		//bit10;
//uint16_t AD9834_SW 					= (uint16_t)(1 << 9); 			//bit9; PIN/SW, choose to SW(software control)
//uint16_t AD9834_RESET_SET			= (uint16_t)(1<< 8); 			//bit8; set RESET model, init set
//uint16_t AD9834_RESET_CLR			= (uint16_t)0x0000;									//bit8; cancle Reset model, ready to output
//uint16_t AD9834_SLEEP_SET			= (uint16_t)(1<< 7);		//bit7; cancle SLEEP fuction
//uint16_t AD9834_SLEEP12_CLR			= (uint16_t)0x0000;
//uint16_t AD9834_Triggerave			= (uint16_t)(1<< 4);						//bit4;
//uint16_t AD9834_MODE_1				= (uint16_t)(1<< 2);						//bit2; ?????


uint16_t FreqLSB = 0;
uint16_t FreqMSB = 0;
uint16_t PhaseWords = 0;

/******************************************************************************
* Function  : Init all pins,
* Input     : NULL
* Output    : NULL
*******************************************************************************/
void AD9834_PortInit(void)
{
		GPIO_InitTypeDef GPIO_InitStruction;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

        GPIO_InitStruction.GPIO_Pin   = FSYNC | SCLK | SDA;
        GPIO_InitStruction.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIO_AD9834, &GPIO_InitStruction);
}

/******************************************************************************
* Function  : Init all set, set AD9834 at Standby model.
* Input     : NULL
* Output    : NULL
*******************************************************************************/
void AD9834_Init(void)
{
        AD9834_PortInit();
        delay_ms(500);
		SET_FSYNC();
        SET_SCLK();
        SET_SDA();
		delay_ms(50);
        AD9834_WriteBytes(AD9834_InitAddress);
}

/***************************************************************************
* Function  : send 16 bit data to AD9834, date type must be unsigned 16bit data.
* Input     : Data which need to be sent.
* Return    : NULL
****************************************************************************/
void AD9834_WriteBytes(uint16_t bytes)
{
	SET_SCLK();
	CLR_FSYNC();
	for(uint8_t count = 0; count < 16; count++) 
	{
        SET_SCLK();
		if(bytes & 0x8000)
			{
                SET_SDA();
            }
		else
			{
                CLR_SDA();
            }

		CLR_SCLK();
		bytes <<= 1;
	}
	SET_FSYNC();
    delay_ms(10);
}

/***********************************************************************************
* Function  : choose Output wave kinds.
* Input     :    
        1. Wave_Sine
        2. Wave_Triangular
        3. Wave_Square

* return    : NULL
************************************************************************************/
void AD9834_WaveSet(unsigned int WaveKind)
{
    ;
}

/********************************************************************************
* Function: calculate the frequency register words which need to write to chip AD9834.
* Input: frequency which need to be change to frequency register word.
* Output: NULL
* changed words are stored in FreqLSB and FreqMSB.
*********************************************************************************/
void OutputFreCalc(uint32_t Frequency)
{
    uint32_t calcuTemp = 0;
    uint32_t temp = 0;
    calcuTemp = (((float)Frequency * 268435456UL) / AD9834_SYSTEM_COLCK + 0.5);     //+0.5 让小数点后有余数大于0.5可以进1
    temp = calcuTemp;
    FreqLSB = (uint16_t)(temp & 0x3fff);
    temp = calcuTemp;
    FreqMSB = (uint16_t)((temp >> 14) & 0x3fff);
}

/****************************************************************
* Function: 	Set output by frequency.
* parameter:  	1.address of register need to write.  	//refere to FREQ_0 or FREQ_1
				2. value of output frequency.			//refere Formula: Freq_value(value)=Freq_data(data)*FCLK/2^28
* return value: NULL
*****************************************************************/
void AD9834_WaveFrequecySet(uint8_t FreqRegNum, uint32_t Frequency)
{
    if (FreqRegNum == FreqRegister_0)
    {
        OutputFreCalc(Frequency);
        delay_ms(10);
        AD9834_WriteBytes(AD9834_InitAddress);      //control register
        AD9834_WriteBytes(FreqLSB | 0x4000);        //frequency LSB
        AD9834_WriteBytes(FreqMSB | 0x4000);        //frequency MSB
        //AD9834_OutputEnable();                      //Exit and reset, set reset to 0
    }
    else
    {
        OutputFreCalc(Frequency);
        AD9834_WriteBytes(AD9834_InitAddress);      //control register
        delay_ms(10);
        AD9834_WriteBytes(FreqLSB | 0x8000);        //frequency LSB
        AD9834_WriteBytes(FreqMSB | 0x8000);        //frequency MSB
        //AD9834_OutputEnable();                      //Exit and reset, set reset to 0
    }
}

/********************************************************************************
* Function: calculate the register phase words which need to write to chip AD9834.
* Input: phase which need to be change to phase register word.
* Output: NULL
* changed words are stored in PhaseWords.
*********************************************************************************/
void OutputPhaseCalc(uint16_t Phase)
{
    PhaseWords = ((Phase * 4096) / (uint16_t)(2 * 3.1415926));
    PhaseWords &= 0x0fff;

}


void AD9834_WavePhaseSet(uint8_t PhaseRegNum, uint16_t Phase)
{
    if (PhaseRegNum == PhaseRegister_0)
    {
        OutputPhaseCalc(Phase);
        AD9834_WriteBytes(0xc000 | PhaseWords);                  //set phase0 register , set DB15,DB14,DB13 to 110
    }
}

//A signal appears at the output of the DAC seven MCLK cycles after RESET is set to 0.
void AD9834_OutputEnable(void)
{
    AD9834_WriteBytes(0x2000);
}


void AD9834_OutputSet(uint8_t FreqRegNum, uint32_t Frequency, uint16_t Phase)
{
    AD9834_WaveFrequecySet(FreqRegNum, Frequency);
    AD9834_WavePhaseSet(PhaseRegister_0, Phase);
    AD9834_OutputEnable();
}

/******************************************* END LINE ***********************************************/


/**
  * -----------------------------------------------------------------------------
  * @file    adc.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file provides all the adc functions.
  * ----------------------------------------------------------------------------
  */
#include "adc.h"

/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address		((uint32_t)0x4001244C)
uint32_t ADC_ConvertedValue;

#ifdef AC200_DEBUG
void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	ADC_DeInit(ADC1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //| RCC_APB2Periph_AFIO

	/* Configure PA1 (ADC Channel1), PA2 (ADC Channel2) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/** 
 * @brief: ADC configuration
 * 
*/
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* ADCCLK = PCLK2/4 -------------------------------------------------*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	ADC_DeInit(ADC1);
	ADC_GPIO_Config();
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent; //Single (one channel) mode
	ADC_InitStructure.ADC_ScanConvMode       = DISABLE;	  // scan mode
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //normal mode
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel       = 2;
	ADC_Init(ADC1, &ADC_InitStructure);

	/** because NbrOfChannel not equal 1. set Sequence channel */
	/* ADC1 regular channel1 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	/* ADC1 regular channel2 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5);
	/*--------------------------------------------------------------------------------*/
	/** ADC NVIC Configuration -------------------------------------------------------*/
	// NVIC_InitStruction.NVIC_IRQChannel                   = ADC1_2_IRQn;
	// NVIC_InitStruction.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStruction.NVIC_IRQChannelSubPriority        = 2;
	// NVIC_InitStruction.NVIC_IRQChannelCmd                = ENABLE;
	// NVIC_Init(&NVIC_InitStruction);

	/* Enable ADC1 DMA */
	//	ADC_DMACmd(ADC1, ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while (ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC1));
	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	DC_MOTOR_RunStatus.DC_MOTOR_CurrentProtection = OFF;
}
/** DMA way ---------------------------------------------------------*/

// void ADC_GPIO_Config(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;

// 	ADC_DeInit(ADC1);
// 	/* ADCCLK = PCLK2/4 -------------------------------------------------*/
// 	// RCC_ADCCLKConfig(RCC_PCLK2_Div4);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); //| RCC_APB2Periph_AFIO

// 	/* Configure PA1 (ADC Channel1), PA2 (ADC Channel2) as analog input -------------------------*/
// 	GPIO_InitStructure.GPIO_Pin   = ADC_5V_Voltage | ADC_MOTOR_CURRENT;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
// 	GPIO_Init(PORTA, &GPIO_InitStructure);
// }

// /** 
//  * @brief: ADC configuration
//  * 
// */
// void ADC_Configuration(void)
// {
// 	ADC_InitTypeDef 	ADC_InitStructure;
//     DMA_InitTypeDef     DMA_InitStructure;
    
// 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
// 	ADC_GPIO_Config();

// 	/* DMA channel1 configuration ----------------------------------------------*/
// 	DMA_DeInit(DMA1_Channel1);
// 	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
// 	DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)&ADC_ConvertedValue;
// 	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
// 	DMA_InitStructure.DMA_BufferSize         = 1;
// 	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
// 	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
// 	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
// 	DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
// 	DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
// 	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
// 	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
// 	/* Enable DMA1 channel1 */
// 	DMA_Cmd(DMA1_Channel1, ENABLE);

// 	/* ADC1 configuration ------------------------------------------------------*/
// 	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent; //Single (one channel) mode
// 	ADC_InitStructure.ADC_ScanConvMode       = DISABLE;	  // si mode
// 	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //normal mode
// 	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
// 	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
// 	ADC_InitStructure.ADC_NbrOfChannel       = 2;


// 	/** because NbrOfChannel not equal 1. set Sequence channel */
// 	/* ADC1 regular channel1 configuration */
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
// 	/* ADC1 regular channel2 configuration */
// 	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5);
// 	/*--------------------------------------------------------------------------------*/

// 	ADC_Cmd(ADC1, ENABLE);
// 	/* Enable ADC1 reset calibaration register */
// 	ADC_ResetCalibration(ADC1);
// 	/* Check the end of ADC1 reset calibration register */
// 	while (ADC_GetResetCalibrationStatus(ADC1));
// 	/* Start ADC1 calibaration */
// 	ADC_StartCalibration(ADC1);
// 	/* Check the end of ADC1 calibration */
// 	while (ADC_GetCalibrationStatus(ADC1));
// 	/* Start ADC1 Software Conversion */
// 	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
// }
/** DMA way ---------------------------------------------------------*/
#endif
/*----------------------------------------------------------------*/

#ifdef AC200
void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;

	ADC_DeInit(ADC1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);       //| RCC_APB2Periph_AFIO

	/* Configure PA1 (ADC Channel1), PA2 (ADC Channel2) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void ADC_DMA_Config(void)
{
	DMA_InitTypeDef 	DMA_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	DMA_DeInit(DMA1_Channel1);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA channel1 configuration ----------------------------------------------*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // datasheet page:286
	DMA_InitStructure.DMA_BufferSize = 16;			   // for ADC 16-bit value
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; // memory increase disable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // normal mode
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
/** 
 * @brief: ADC configuration
 * 
*/
void ADC_Configuration(void)
{
	ADC_InitTypeDef 	ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  	/* ADCCLK = PCLK2/4 -------------------------------------------------*/
  	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    ADC_DeInit(ADC1);
	ADC_GPIO_Config();
//	ADC_DMA_Config();
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;    //Single (one channel) mode
	ADC_InitStructure.ADC_ScanConvMode       = DISABLE;				    // scan mode
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					//normal mode
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel       = 2;
	ADC_Init(ADC1, &ADC_InitStructure);

	/** because NbrOfChannel not equal 1. set Sequence channel */
	/* ADC1 regular channel1 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	/* ADC1 regular channel2 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5);
	/*--------------------------------------------------------------------------------*/
	/** ADC NVIC Configuration -------------------------------------------------------*/
    // NVIC_InitStruction.NVIC_IRQChannel                   = ADC1_2_IRQn;
    // NVIC_InitStruction.NVIC_IRQChannelPreemptionPriority = 1;
    // NVIC_InitStruction.NVIC_IRQChannelSubPriority        = 2;
    // NVIC_InitStruction.NVIC_IRQChannelCmd                = ENABLE;
    // NVIC_Init(&NVIC_InitStruction);

	/* Enable ADC1 DMA */
//	ADC_DMACmd(ADC1, ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while (ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC1));
	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	DC_MOTOR_CurrentProtection = OFF;
}
#endif
/**
 * @brief: Get specify ADC channel converted value and return it.
 * @refer datasheet P224: ADC samples the input voltage for a number of ADC_CLK cycles which can be modified using
	the SMP[2:0] bits in the ADC_SMPR1 and ADC_SMPR2 registers. Each channel can be
	sampled with a different sample time.
	The total conversion time is calculated as follows:
	Tconv = Sampling time + 12.5 cycles
	Example:
	With an ADCCLK = 14 MHz and a sampling time of 1.5 cycles:
	Tconv = 1.5 + 12.5 = 14 cycles = 1 μs
 */
uint16_t Get_ADC_Value(uint8_t ADC_Channel)
{
	/* set specification  channel ,rank and sample time -------------------*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5 );  //ADC1,ADC Channel, ADC sample time239.5T
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);										//Enable ADC1 software  convert function
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); 							//Wait covert finished
	return ADC_GetConversionValue(ADC1);  										//return covert value
}
/**
 * @brief: Get average value frome ADC channel
 * 
 */
uint16_t Get_Adc_Average(uint8_t ADC_Channel, uint8_t times)
{
	uint32_t temp_val = 0;
	uint8_t t;
	for (t = 0; t < times; t++)
	{
		temp_val += Get_ADC_Value(ADC_Channel);
	}
	return temp_val / times;
}

/* Get STM32 Chip Temperature ---------------------------------*/

/**
 *    Get chip inside temperature
 *    @param  times [times for average calculate]
 *    @return       [return chip inside temperature average]
 */
float GetChipInsideTempAve(uint8_t times)
{
	uint32_t chipTempValue = 0;
	float 	 valueTemp     = 0;
	float 	 chipTemp      = 0;
	uint8_t counter        = 0;
	counter                = times;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);
	ADC_TempSensorVrefintCmd(ENABLE);                               	// enable TSVREFE bit to read inside temperature
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	/* times Temperature value sum ------------------*/
	for (; counter > 0; counter--)
	{
		while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
		chipTempValue += ADC_GetConversionValue(ADC1);
	}
	chipTempValue = chipTempValue / times;					//Temperature average
	valueTemp = (float)chipTempValue * (3.3 / 4096);		//value transfer
	chipTemp = (((1.43 - valueTemp) / 0.0043) + 25);        //Temperature = {(V25 - Vsense)/Avg_slope} + 25
	// ADC_TempSensorVrefintCmd(DISABLE);
	return chipTemp;
}

/* Get STM32 Chip inside AD Vref ---------------------------------*/
/**
 *    Get chip inside AD Vref voltage
 *    @param  		times for get average, it must equal or great than 1
 *    @return       [average chip inside AD vref voltage]
 */
float GetChipVerf_VOL_AVE(uint8_t times)
{
	uint32_t ADC_VrefValueTemp  = 0;
	float 	ADC_vrefValue       = 0;

	/* Enable the temperature sensor and Verfer channel*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_239Cycles5);
	ADC_TempSensorVrefintCmd(ENABLE);  					// enable TSVREFE bit
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	/* times voltage value sum --------------------*/
	for (uint8_t counter = 1; counter < times; counter++)
	{
        DelayMs(100);
		while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
		ADC_VrefValueTemp += ADC_GetConversionValue(ADC1);
	}
	ADC_VrefValueTemp = ADC_VrefValueTemp / (times - 1);						//AD Vref average
	ADC_vrefValue = (float)ADC_VrefValueTemp * (3.3 / 4096);			//value transfer
	// ADC_TempSensorVrefintCmd(DISABLE);
    #ifdef AC200_DEBUG
        printf("\r\n STM32 chip Vref is: %2.3f\r\n", ADC_vrefValue);
	#endif
	return ADC_vrefValue;
}
/**
 * @brief: 		Get motor current value
 * @param: 		times for get average, it must equal or great than 1
 * @return: 	return got value
 */
float GetMotorCurrentOriginal(uint8_t times)
{
	uint32_t ADC_MotorCurrentValueTemp = 0;
	float ADC_MotorCurrentValue        = 0;

	/* Enable the temperature sensor and Verfer channel*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_71Cycles5);
	ADC_TempSensorVrefintCmd(ENABLE); 												// enable TSVREFE bit
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	/* times voltage value sum --------------------*/
	for (uint8_t counter = 1; counter < times; counter++)
	{
		while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
		ADC_MotorCurrentValueTemp += ADC_GetConversionValue(ADC1);
        DelayMs(50);
	}
	ADC_MotorCurrentValueTemp = ADC_MotorCurrentValueTemp / (times - 1);					//AD Vref average
	ADC_MotorCurrentValue     = (float)ADC_MotorCurrentValueTemp * (3.3 / 4096); 	//value transfer
	#ifdef AC200_DEBUG
		printf("\r\n Motor current Original is: %2.3f\r\n", ADC_MotorCurrentValue);
	#endif
	return (ADC_MotorCurrentValue);										            // hardware define.
}
/**
 * @brief: 
 * NOT GOOD!!!
 */
float GetMotorCurrentAVE(uint8_t times)
{
	uint32_t ADC_MotorCurrentValueTemp = 0;
    uint32_t ADC_Motor_AVE_CurrentValue = 0;
	float ADC_MotorCurrentValue        = 0;
    uint8_t counter = 0;

    if(times == 0)
    {
        times = 1;
    }

    /* Enable the temperature sensor and Verfer channel*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5);
    ADC_TempSensorVrefintCmd(ENABLE); 												// enable TSVREFE bit
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    /* times voltage value sum --------------------*/
    for(uint8_t counter1 = 1; counter1 < times; counter1++)
    {
        for (; counter < 5; counter++)
        {
            while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
            ADC_MotorCurrentValueTemp += ADC_GetConversionValue(ADC1);	
            DelayMs(50);
        }
        ADC_MotorCurrentValueTemp = (ADC_MotorCurrentValueTemp / counter);
        ADC_Motor_AVE_CurrentValue += ADC_MotorCurrentValueTemp;
    }
    
    ADC_Motor_AVE_CurrentValue = ADC_Motor_AVE_CurrentValue / (times - 1);					//AD Vref average
    ADC_MotorCurrentValue     = (float)ADC_Motor_AVE_CurrentValue * (3.3 / 4096); 	//value transfer
    #ifdef AC200_DEBUG
        printf("\r\n Motor current AVE is: %2.3f\r\n", ADC_MotorCurrentValue);
    #endif

	return (ADC_MotorCurrentValue);										            // hardware define.
}
/**
 * @brief: 		Get VCC Voltage value
 * @param: 		times for get average, it must equal or great than 1
 * @return: 	return got value
 */
float GetMotor_VCC_5V_Vlotage(uint8_t times)
{
	uint32_t ADC_VCC_ValueTemp = 0;
	float ADC_VCC_Value        = 0;

	/* Enable the temperature sensor and Verfer channel*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
	ADC_TempSensorVrefintCmd(ENABLE); // enable TSVREFE bit
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	/* times voltage value sum --------------------*/
	for (uint8_t counter = 1; counter < times; counter++)
	{
		while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        DelayMs(100);
		ADC_VCC_ValueTemp += ADC_GetConversionValue(ADC1);
	}
	ADC_VCC_ValueTemp = ADC_VCC_ValueTemp / (times - 1);			 		//AD Vref average
	ADC_VCC_Value     = (((float)ADC_VCC_ValueTemp * (3.3 / 4096) )* 2); 	//value transfer to float Decimal
	#ifdef AC200_DEBUG
		printf("\r\n 5V power voltage is: %2.3f\r\n", ADC_VCC_Value);
	#endif
	return ADC_VCC_Value;									 	// hardware define
}
/*---------------------------------- END LINE -------------------------------------*/

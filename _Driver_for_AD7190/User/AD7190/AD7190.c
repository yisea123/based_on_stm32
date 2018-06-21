#include "AD7190.h"
/////////////////////////////////////////////////////////////////////////////////////////
/***************************************************************************//**
 *   @file   AD7190.c
 *   @brief  Implementation of AD7190 Driver.
 *   @author DNechita (Dan.Nechita@analog.com)
********************************************************************************
 * Copyright 2012(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
 *   SVN Revision: 903
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
// #include "AD7190.h"     // AD7190 definitions.
// #include "TIME.h"       // TIME definitions.

/***************************************************************************//**
 * @brief Writes data into a register.
 *
 * @param registerAddress - Address of the register.
 * @param registerValue - Data value to write.
 * @param bytesNumber - Number of bytes to be written.
 * @param modifyCS - Allows Chip Select to be modified.
 *
 * @return none.
*******************************************************************************/
void AD7190_SetRegisterValue(unsigned char registerAddress,
                             unsigned long registerValue,
                             unsigned char bytesNumber,
                             unsigned char modifyCS)
{
    unsigned char writeCommand[5] = {0, 0, 0, 0, 0};
    unsigned char* dataPointer    = (unsigned char*)&registerValue;
    unsigned char bytesNr         = bytesNumber;
    
    writeCommand[0] = AD7190_COMM_WRITE |
                      AD7190_COMM_ADDR(registerAddress);
    while(bytesNr > 0)
    {
        writeCommand[bytesNr] = *dataPointer;
        dataPointer ++;
        bytesNr --;
    }
    SPI_Write(AD7190_SLAVE_ID * modifyCS, writeCommand, bytesNumber + 1);
}

/***************************************************************************//**
 * @brief Reads the value of a register.
 *
 * @param registerAddress - Address of the register.
 * @param bytesNumber - Number of bytes that will be read.
 * @param modifyCS    - Allows Chip Select to be modified.
 *
 * @return buffer - Value of the register.
*******************************************************************************/
unsigned long AD7190_GetRegisterValue(unsigned char registerAddress,
                                      unsigned char bytesNumber,
                                      unsigned char modifyCS)
{
    unsigned char registerWord[5] = {0, 0, 0, 0, 0}; 
    unsigned long buffer          = 0x0;
    unsigned char i               = 0;
    
    registerWord[0] = AD7190_COMM_READ |
                      AD7190_COMM_ADDR(registerAddress);
    SPI_Read(AD7190_SLAVE_ID * modifyCS, registerWord, bytesNumber + 1);
    for(i = 1; i < bytesNumber + 1; i++) 
    {
        buffer = (buffer << 8) + registerWord[i];
    }
    
    return buffer;
}

/***************************************************************************//**
 * @brief Checks if the AD7190 part is present.
 *
 * @return status - Indicates if the part is present or not.
*******************************************************************************/
unsigned char AD7190_Init(void)
{
    unsigned char status = 1;
    unsigned char regVal = 0;
    
    AD7190_Reset();
    /* Allow at least 500 us before accessing any of the on-chip registers. */
    delay_ms(1);
    regVal = AD7190_GetRegisterValue(AD7190_REG_ID, 1, 1);
    if( (regVal & AD7190_ID_MASK) != ID_AD7190)
    {
        status = 0;
    }
    return status ;
}

/***************************************************************************//**
 * @brief Resets the device.
 *
 * @return none.
*******************************************************************************/
void AD7190_Reset(void)
{
    unsigned char registerWord[7];
    
    registerWord[0] = 0x01;
    registerWord[1] = 0xFF;
    registerWord[2] = 0xFF;
    registerWord[3] = 0xFF;
    registerWord[4] = 0xFF;
    registerWord[5] = 0xFF;
    registerWord[6] = 0xFF;
    SPI_Write(AD7190_SLAVE_ID, registerWord, 7);
}

/***************************************************************************//**
 * @brief Set device to idle or power-down.
 *
 * @param pwrMode - Selects idle mode or power-down mode.
 *                  Example: 0 - power-down
 *                           1 - idle
 *
 * @return none.
*******************************************************************************/
void AD7190_SetPower(unsigned char pwrMode)
{
     unsigned long oldPwrMode = 0x0;
     unsigned long newPwrMode = 0x0; 
 
     oldPwrMode = AD7190_GetRegisterValue(AD7190_REG_MODE, 3, 1);
     oldPwrMode &= ~(AD7190_MODE_SEL(0x7));
     newPwrMode = oldPwrMode | 
                  AD7190_MODE_SEL((pwrMode * (AD7190_MODE_IDLE)) |
                                  (!pwrMode * (AD7190_MODE_PWRDN)));
     AD7190_SetRegisterValue(AD7190_REG_MODE, newPwrMode, 3, 1);
}

///***************************************************************************//**
// * @brief Waits for RDY pin to go low.
// *
// * @return none.
//*******************************************************************************/
//void AD7190_WaitRdyGoLow(void)
//{
//    unsigned long timeOutCnt = 0xFFFFF;
//    
//    while(AD7190_RDY_STATE && timeOutCnt--)
//    {
//        ;
//    }
//}

/***************************************************************************//**
 * @brief Selects the channel to be enabled.
 *
 * @param channel - Selects a channel.
 *  
 * @return none.
*******************************************************************************/
void AD7190_ChannelSelect(unsigned short channel)
{
    unsigned long oldRegValue = 0x0;
    unsigned long newRegValue = 0x0;   
     
    oldRegValue = AD7190_GetRegisterValue(AD7190_REG_CONF, 3, 1);
    oldRegValue &= ~(AD7190_CONF_CHAN(0xFF));
    newRegValue = oldRegValue | AD7190_CONF_CHAN(1 << channel);   
    AD7190_SetRegisterValue(AD7190_REG_CONF, newRegValue, 3, 1);
}

/***************************************************************************//**
 * @brief Performs the given calibration to the specified channel.
 *
 * @param mode - Calibration type.
 * @param channel - Channel to be calibrated.
 *
 * @return none.
*******************************************************************************/
void AD7190_Calibrate(unsigned char mode, unsigned char channel)
{
    unsigned long oldRegValue = 0x0;
    unsigned long newRegValue = 0x0;
    
    AD7190_ChannelSelect(channel);
    oldRegValue = AD7190_GetRegisterValue(AD7190_REG_MODE, 3, 1);
    oldRegValue &= ~AD7190_MODE_SEL(0x7);
    newRegValue = oldRegValue | AD7190_MODE_SEL(mode);
    _CS_L(); 
    AD7190_SetRegisterValue(AD7190_REG_MODE, newRegValue, 3, 0); // CS is not modified.
//    AD7190_WaitRdyGoLow();
    delay_ms(10);
    _CS_H();
}

/***************************************************************************//**
 * @brief Selects the polarity of the conversion and the ADC input range.
 *
 * @param polarity - Polarity select bit. 
                     Example: 0 - bipolar operation is selected.
                              1 - unipolar operation is selected.
* @param range - Gain select bits. These bits are written by the user to select 
                 the ADC input range.     
 *
 * @return none.
*******************************************************************************/
void AD7190_RangeSetup(unsigned char polarity, unsigned char range)
{
    unsigned long oldRegValue = 0x0;
    unsigned long newRegValue = 0x0;
    
    oldRegValue = AD7190_GetRegisterValue(AD7190_REG_CONF,3, 1);
    oldRegValue &= ~(AD7190_CONF_UNIPOLAR |
                     AD7190_CONF_GAIN(0x7));
    newRegValue = oldRegValue | 
                  (polarity * AD7190_CONF_UNIPOLAR) |
                  AD7190_CONF_GAIN(range); 
    AD7190_SetRegisterValue(AD7190_REG_CONF, newRegValue, 3, 1);
}

/***************************************************************************//**
 * @brief Returns the result of a single conversion.
 *
 * @return regData - Result of a single analog-to-digital conversion.
*******************************************************************************/
unsigned long AD7190_SingleConversion(void)
{
    unsigned long command = 0x0;
    unsigned long regData = 0x0;
 
    command = AD7190_MODE_SEL(AD7190_MODE_SINGLE) | 
              AD7190_MODE_CLKSRC(AD7190_CLK_INT) |
              AD7190_MODE_RATE(0x060);    
    _CS_L();
    AD7190_SetRegisterValue(AD7190_REG_MODE, command, 3, 0); // CS is not modified.
    //AD7190_WaitRdyGoLow();
    delay_ms(10);
    regData = AD7190_GetRegisterValue(AD7190_REG_DATA, 3, 0);
    _CS_H();
    
    return regData;
}

/***************************************************************************//**
 * @brief Returns the average of several conversion results.
 *
 * @return samplesAverage - The average of the conversion results.
*******************************************************************************/
unsigned long AD7190_ContinuousReadAvg(unsigned char sampleNumber)
{
    unsigned long samplesAverage = 0x0;
    unsigned char count = 0x0;
    unsigned long command = 0x0;
    
    command = AD7190_MODE_SEL(AD7190_MODE_CONT) | 
              AD7190_MODE_CLKSRC(AD7190_CLK_INT) |
              AD7190_MODE_RATE(0x060);
    _CS_L();
    AD7190_SetRegisterValue(AD7190_REG_MODE, command, 3, 0); // CS is not modified.
    for(count = 0;count < sampleNumber;count ++)
    {
        //AD7190_WaitRdyGoLow();
        delay_ms(10);
        samplesAverage += AD7190_GetRegisterValue(AD7190_REG_DATA, 3, 0); // CS is not modified.
    }
    _CS_H();
    samplesAverage = samplesAverage / sampleNumber;
    
    return samplesAverage ;
}

/***************************************************************************//**
 * @brief Read data from temperature sensor and converts it to Celsius degrees.
 *
 * @return temperature - Celsius degrees.
*******************************************************************************/
unsigned long AD7190_TemperatureRead(void)
{
    unsigned char temperature = 0x0;
    unsigned long dataReg = 0x0;
    
    AD7190_RangeSetup(0, AD7190_CONF_GAIN_1);
    AD7190_ChannelSelect(AD7190_CH_TEMP_SENSOR);
    dataReg = AD7190_SingleConversion();
    dataReg -= 0x800000;
    dataReg /= 2815;   // Kelvin Temperature
    dataReg -= 273;    //Celsius Temperature
    temperature = (unsigned long) dataReg;
    
    return temperature;
}


///////////////////////////////////////////////////////////////////////////////////////////////




























//  AD7190_CommunicationReg_Init   AD7190_CommunicationReg;
//  AD7190_StatusRegInit			AD7190_StatusReg;

// void AD7190_ModeConfig(void)
// {
// 	AD7190_CommunicationReg.AD7190_WriteMode = 	(uint8_t)(1 << 6);
// 	AD7190_CommunicationReg.AD7190_ReadMode  = 	(uint8_t)(1 << 7);
// 	AD7190_CommunicationReg.AD7190_ModeReg   = 	(uint8_t)(1 << 3);
// 	AD7190_CommunicationReg.AD7190_StatusReg =	(uint8_t)(1 << 4);
// 	AD7190_CommunicationReg.AD7190_Mode_ID   =	(uint8_t)(1 << 5);

// 	SPI1_SendByte(AD7190_CommunicationReg.AD7190_WriteMode);		//default mode
// }

// void AD7190_StatusConfig(void)
// {

// }

// void AD7190_init(void)
// {
// 	for (uint8_t i = 0; i < 7; i++)
// 	{
// 		SPI1_SendByte(0xff);
// 	}
// }















/************************************* END LINE ************************************************/

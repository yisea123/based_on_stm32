/***************************************************************************//**
 *   @file   Communication.c
 *   @brief  Implementation of Communication Driver for RENESAS RL78G13
 *           Processor.
 *   @author DBogdan (dragos.bogdan@analog.com)
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
 *   SVN Revision: 750
*******************************************************************************/

/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "Communication.h"

/******************************************************************************/
/* Variables Declarations                                                     */
/******************************************************************************/
unsigned char nineClocksFlag = 0;

/***************************************************************************//**
 * @brief INTIIC01 interrupt service routine.
 *
 * @return None.
*******************************************************************************/
#pragma vector = INTIIC01_vect
__interrupt static void IIC01_Interrupt(void)
{
    nineClocksFlag = 1;
}

/***************************************************************************//**
 * @brief Initializes the I2C communication peripheral.
 *
 * @param clockFreq - I2C clock frequency (Hz).
 *                    Example: 100000 - SPI clock frequency is 100 kHz.
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful;
 *                           0 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char I2C_Init(unsigned long clockFreq)
{
    unsigned long fckFrequency   = 16000000;
    unsigned short  regClockFreq = 0;
    
    SAU0EN = 1U;    // supply SAU0 clock
    SPS0 = 0x0001;
    ST0 |= 0x0002;   // disable IIC01
    IICMK01 = 1U;    // disable INTIIC01 interrupt
    IICIF01 = 0U;    // clear INTIIC01 interrupt flag
    SMR01 = 0x0024; // Simplified I2C operation mode.
    SCR01 = 0x0017;
     /* Set transfer rate. */
    regClockFreq = (unsigned short) ((fckFrequency / clockFreq) /  2) - 1;
    if(regClockFreq != 0)
    {
        SDR01 = regClockFreq << 9;
    }
    SO0 |= 0x0202;
    /* Set SCL01, SDA01 pin */
    P7 |= 0x30U;
    PM7 &= 0xCFU;
    
    return 1;
}

/***************************************************************************//**
 * @brief Writes data to a slave device.
 *
 * @param slaveAddress - Adress of the slave device.
 * @param dataBuffer - Pointer to a buffer storing the transmission data.
 * @param bytesNumber - Number of bytes to write.
 * @param stopBit - Stop condition control.
 *                  Example: 0 - A stop condition will not be sent;
 *                           1 - A stop condition will be sent.
 *
 * @return status - Number of written bytes.
*******************************************************************************/
unsigned char I2C_Write(unsigned char slaveAddress,
                        unsigned char* dataBuffer,
                        unsigned char bytesNumber,
                        unsigned char stopBit)
{
    unsigned char wait = 0;
    unsigned char byte = 0;

    ST0 |= 0x0002;  // Stop the communication operation for manipulate the CKOmn.
    SOE0 &= ~0x0002;// Disable serial output.
    SO0 |= 0x0200;  // Set SCLK.
    SO0 |= 0x0002;  // Set SDA.
    for(wait = 0; wait < 10; wait++)
    {
         __no_operation();
    }
    SO0 &= ~0x0002; // Clear SDA.
    ST0 &= ~0x0002; // Start the communication operation.
    SS0 |= 0x0002;  // Enable the operation.

    SOE0 |= 0x0002;     // Enable serial output.
    SCR01 &= ~0xC000;
    SCR01 |= 0x8000;    // Set the operation mode to transmission only.
    IICIF01 = 0;        // Clear the interrupt flag.
    IICMK01 = 0;        // Interrupt servicing enabled.
    
    nineClocksFlag = 0;
    SIO01 = (slaveAddress << 1);
    while(nineClocksFlag == 0);
    for(byte = 0; byte < bytesNumber; byte++)
    {
        nineClocksFlag = 0;
        SIO01 = *dataBuffer;
        while(nineClocksFlag == 0);
        dataBuffer++;
    }
    
    if(stopBit)
    {
        ST0 |= 0x0002;  // Stop the communication operation for manipulate the CKOmn.
        SOE0 &= ~0x0002;// Disable serial output.
        SO0 &= ~0x0002; // Clear SDA.
        SO0 |= 0x0200;  // Set SCLK.
        for(wait = 0; wait < 10; wait++)
        {
             __no_operation();
        }
        SO0 |= 0x0002;  // Set SDA.
    }
    
    return bytesNumber;
}

/***************************************************************************//**
 * @brief Reads data from a slave device.
 *
 * @param slaveAddress - Adress of the slave device.
 * @param dataBuffer - Pointer to a buffer that will store the received data.
 * @param bytesNumber - Number of bytes to read.
 * @param stopBit - Stop condition control.
 *                  Example: 0 - A stop condition will not be sent;
 *                           1 - A stop condition will be sent.
 *
 * @return status - Number of read bytes.
*******************************************************************************/
unsigned char I2C_Read(unsigned char slaveAddress,
                       unsigned char* dataBuffer,
                       unsigned char bytesNumber,
                       unsigned char stopBit)
{
    unsigned char wait = 0;
    unsigned char byte = 0;

    ST0 |= 0x0002;  // Stop the communication operation for manipulate the CKOmn.
    SOE0 &= ~0x0002;// Disable serial output.
    SO0 |= 0x0200;  // Set SCLK.
    SO0 |= 0x0002;  // Set SDA.
    for(wait = 0; wait < 10; wait++)
    {
        __no_operation();
    }
    SO0 &= ~0x0002; // Clear SDA.
    ST0 &= ~0x0002; // Start the communication operation.
    SS0 |= 0x0002;  // Enable the operation.

    SOE0 |= 0x0002;     // Enable serial output.
    SCR01 &= ~0xC000;
    SCR01 |= 0x8000;    // Set the operation mode to transmission only.
    IICIF01 = 0;        // Clear the interrupt flag.
    IICMK01 = 0;        // Interrupt servicing enabled.
    
    nineClocksFlag = 0;
    SIO01 = (slaveAddress << 1) + 1;
    while(nineClocksFlag == 0);
    
    SCR01 &= ~0xC000;
    SCR01 |= 0x4000;    // Set the operation mode to reception only.

    for(byte = 0; byte < bytesNumber; byte++)
    {
        if(byte == (bytesNumber - 1))   // Last byte.
        {
            SOE0 &= ~0x0002;    // Disable serial output.
        }
        nineClocksFlag = 0;
        SIO01 = 0xFFU;
        while(nineClocksFlag == 0);
        *dataBuffer = SIO01;
        dataBuffer++;        
    }
    if(stopBit)
    {
        ST0 |= 0x0002;  // Stop the communication operation for manipulate the CKOmn.
        SOE0 &= ~0x0002;// Disable serial output.
        SO0 &= ~0x0002; // Clear SDA.
        SO0 |= 0x0200;  // Set SCLK.
        for(wait = 0; wait < 10; wait++)
        {
             __no_operation();
        }
        SO0 |= 0x0002;  // Set SDA.
    }
    
    return bytesNumber;
}

/***************************************************************************//**
 * @brief Initializes the SPI communication peripheral.
 *
 * @param lsbFirst - Transfer format (0 or 1).
 *                   Example: 0x0 - MSB first.
 *                            0x1 - LSB first.
 * @param clockFreq - SPI clock frequency (Hz).
 *                    Example: 1000 - SPI clock frequency is 1 kHz.
 * @param clockPol - SPI clock polarity (0 or 1).
 *                   Example: 0x0 - Idle state for clock is a low level; active
 *                                  state is a high level;
 *	                      0x1 - Idle state for clock is a high level; active
 *                                  state is a low level.
 * @param clockEdg - SPI clock edge (0 or 1).
 *                   Example: 0x0 - Serial output data changes on transition
 *                                  from idle clock state to active clock state;
 *                            0x1 - Serial output data changes on transition
 *                                  from active clock state to idle clock state.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful;
 *                           0 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char SPI_Init(unsigned char lsbFirst,
                       unsigned long clockFreq,
                       unsigned char clockPol,
                       unsigned char clockEdg)
{
    unsigned long mckFreq  = 16000000;
    unsigned char sdrValue = 0;
    
    PMOD1_CS_OUT;
    PMOD1_CS_HIGH;
    ST7579_CS_PIN_OUT;
    ST7579_CS_HIGH;
    
    /* Enable input clock supply. */
    SAU0EN = 1;
    /* After setting the SAUmEN bit to 1, be sure to set serial clock select
       register m (SPSm) after 4 or more fCLK clocks have elapsed. */
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    SPS0 = 0x0001;
    
    SMR02 = 0x0020;     // CSI mode.
    clockPol = 1 - clockPol;
    SCR02 = (clockEdg << 13) |
            (clockPol << 12) |
            0xC000 |            // Operation mode: Transmission/reception.
            0x0007;             // 8-bit data length (stored in bits 0 to 7 of
                                // the SDRmn register).
    /* clockFreq =  mckFreq / (sdrValue * 2 + 2)*/
    sdrValue = (unsigned char)(mckFreq / (2 * clockFreq) - 1);
    SDR02 = sdrValue << 9;
    
    /* Enable output for serial communication operation. */
    SOE0 |= 0x0004;
    
    /*  Configure SCLK and MOSI pins as output. */
    PMC0 &= 0xFB; 
    PM0 &= ~0x14;
    P0 |= 0x14;
    /*  Configure the MISO pin as input. */
    PMC0 &= 0xF7;
    PM0 |= 0x08;
    /* Set the clock and data initial level. */
    clockPol = 1 - clockPol;
    SO0 &= ~0x0202;
    SO0 |= (clockPol << 9) |
           (clockPol << 1);  
    /* Set the SEmn bit to 1 and enter the communication wait status */
    SS0 |= 0x0004;
    
    return 1;
}

/***************************************************************************//**
 * @brief Reads data from SPI.
 *
 * @param slaveDeviceId - The ID of the selected slave device.
 * @param data - Data represents the write buffer as an input parameter and the
 *               read buffer as an output parameter.
 * @param bytesNumber - Number of bytes to read.
 *
 * @return Number of read bytes.
*******************************************************************************/
unsigned char SPI_Read(unsigned char slaveDeviceId,
                       unsigned char* data,
                       unsigned char bytesNumber)
{
    unsigned char byte = 0;
    
    if(slaveDeviceId == 1)
    {
        PMOD1_CS_LOW;
    }
    if(slaveDeviceId == 2)
    {  
        ST7579_CS_LOW;
    }
    for(byte = 0; byte < bytesNumber; byte++)
    {
        SIO10 = data[byte];
        __no_operation();
        __no_operation();
        while(SSR02 & 0x0040);
        data[byte] = SIO10;
    }
    if(slaveDeviceId == 1)
    {
        PMOD1_CS_HIGH;
    }
    if(slaveDeviceId == 2)
    {
        ST7579_CS_HIGH;
    }
    
    return bytesNumber;
}

/***************************************************************************//**
 * @brief Writes data to SPI.
 *
 * @param slaveDeviceId - The ID of the selected slave device.
 * @param data - Data represents the write buffer.
 * @param bytesNumber - Number of bytes to write.
 *
 * @return Number of written bytes.
*******************************************************************************/
unsigned char SPI_Write(unsigned char slaveDeviceId,
                        unsigned char* data,
                        unsigned char bytesNumber)
{
    unsigned char byte = 0;
    unsigned char read = 0;
    
    if(slaveDeviceId == 1)
    {
        PMOD1_CS_LOW;
    }
    if(slaveDeviceId == 2)
    {
        ST7579_CS_LOW;
    }
    for(byte = 0; byte < bytesNumber; byte++)
    {
        SIO10 = data[byte];
        __no_operation();
        __no_operation();
        while(SSR02 & 0x0040);
        read = SIO10;
    }
    if(slaveDeviceId == 1)
    {
        PMOD1_CS_HIGH;
    }
    if(slaveDeviceId == 2)
    {
        ST7579_CS_HIGH;
    }
    
    return bytesNumber;
}

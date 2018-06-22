/**************************************************************************//**
*   @file   spi.c
*   @brief  SPI functions implementations.
*   @author acozma (andrei.cozma@analog.com)
*
*******************************************************************************
* Copyright 2011(c) Analog Devices, Inc.
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
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
* THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT, MERCHANTABILITY
* AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************
*   SVN Revision: 750
******************************************************************************/

/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/
#include "spi.h"
#include "Communication.h"

/**************************************************************************//**
* @brief Initializes the communication with the Microblaze SPI peripheral.
*
* @param spiBaseAddr - Microblaze SPI peripheral AXI base address.
* @param lsbFirst - Set to 1 if the data is transmitted LSB first.
* @param cpha - Set to 1 if CPHA mode is used.
* @param cpol - Set to 1 if CPOL mode is used.
* @return Returns 0 for success or negative error code
******************************************************************************/
uint32_t Init_SPI(uint32_t spiBaseAddr, 
                  uint8_t lsbFirst, 
                  uint8_t cpha, 
                  uint8_t cpol)
{
    //SPI_Init(lsbFirst, 8000000, 1, 0);
    
    return 0;
}

/**************************************************************************//**
* @brief Transfers data to and from a SPI slave.
*
* @param spiBaseAddr - Microblaze SPI peripheral AXI base address.
* @param txSize - Number of bytes to transmit to the SPI slave.
* @param txBuffer - Buffer which holds the data to be transmitted to the SPI slave.
* @param rxSize - Number of bytes to receive from the SPI slave.
* @param txBuffer - Buffer to store the data read to the SPI slave.
* @param ssNo - Slave select line on which the slave is connected.
* @return Returns 0 for success or negative error code.
******************************************************************************/
uint32_t SPI_TransferData(uint32_t spiBaseAddr, 
                          uint8_t txSize, uint8_t* txBuf, 
                          uint8_t rxSize, uint8_t* rxBuf, 
                          uint8_t ssNo)
{
    uint8_t spiSlaveAddr = 1;
    uint8_t byteIndex    = 0;
    uint8_t buffer[10] = {0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0};
    

    for(byteIndex = 0; byteIndex < txSize; byteIndex++)
    {
        buffer[byteIndex] = txBuf[byteIndex];
    }
    SPI_Read(spiSlaveAddr, buffer, txSize + rxSize);
    for(byteIndex = 0; byteIndex < rxSize; byteIndex++)
    {
        rxBuf[byteIndex] = buffer[byteIndex + txSize];
    }
    
    return 0;
}

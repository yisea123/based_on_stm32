/*! ****************************************************************************
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
 *
 * @spi model     - configurat SPI in model 3.
*******************************************************************************/ 

#include "spi.h"

#define SPI_HARDWARE    true
//#define SPI_HARDWARE    false

void SPI1_gpioConfig(void)
{
    GPIO_InitTypeDef SPI1_GPIO_InitStructure;
        
#if SPI_HARDWARE == true
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    SPI1_GPIO_InitStructure.GPIO_Pin = SPI1_SCK | SPI1_MOSI | SPI1_MISO;
    SPI1_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                            //Init IO port to AF output
    SPI1_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_PORT, &SPI1_GPIO_InitStructure);
    
    SPI1_GPIO_InitStructure.GPIO_Pin = SPI1_NSS;
    SPI1_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    SPI1_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_PORT, &SPI1_GPIO_InitStructure);

#endif

#if SPI_HARDWARE == false
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    SPI1_GPIO_InitStructure.GPIO_Pin = SPI1_SCLK | SPI1_MOSI;
    SPI1_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    SPI1_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_PORT, &SPI1_GPIO_InitStructure);

    SPI1_GPIO_InitStructure.GPIO_Pin = SPI1_MISO;
    SPI1_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    SPI1_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_PORT, &SPI1_GPIO_InitStructure);

    GPIO_SetBits(SPI1_PORT, SPI1_SCK | SPI1_MOSI | SPI1_MISO);               //default mode
    
    _SCLK_H();
    _MOSI_L();

#endif

    SPI1_GPIO_InitStructure.GPIO_Pin = SPI1_NSS;
    SPI1_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    SPI1_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_PORT, &SPI1_GPIO_InitStructure);

    _CS_H();
}
/**************************************************************************
* configurat SPI in model 3.
**************************************************************************/
void SPI1_Config(void)
{

#if SPI_HARDWARE == true
    SPI_InitTypeDef SPI_InitStruction;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitStruction.SPI_Direction = SPI_Direction_2Lines_FullDuplex;              
    SPI_InitStruction.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruction.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruction.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruction.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruction.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruction.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStruction.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruction.SPI_CRCPolynomial = 7;                            //CRC check
    
    SPI_Init(SPI1, &SPI_InitStruction);

/*************************************** need to consider **************************   
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    
    NVIC_Init(&NVIC_InitStructure);
***********************************************************************************/
#endif

#if SPI_HARDWARE == false
    return 0;
#endif

}
/**************************************************************************
* Init SPI1

**************************************************************************/
void SPI1_Init(void)
{
    SPI1_gpioConfig();
    SPI1_Config();

    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);
    
    SPI_Cmd(SPI1, ENABLE);                                              // finished configuration and open spi1
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

static uint8_t _SPI_ReadWrite(unsigned char* data, unsigned char bytesNumber)
{       
    uint8_t i = 0;
    uint8_t retry = 0;

    #if SPI_HARDWARE == true
        _CS_L();
        uint8_t retry = 0;
        for(i = 0; i < bytesNumber; i++) {
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
            {
                retry++;
                if(retry > 200)
                {
                    printf("SPI1 Send Failed.\r\n");
                    return 0;
                }
            }
            SPI_I2S_SendData(SPI1, data[i]);
            retry = 0;
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
            {
               retry++;
                if(retry > 200)
                {
                    printf("SPI1 Receive Failed.\r\n");
                    return 0;
                }
            }
            data[i] = SPI_I2S_ReceiveData(SPI1);
        }

        _CS_H();
        return bytesNumber;

    #endif

    #if SPI_HARDWARE == false
        uint8_t valueToWrite, readData = 0, readBit, byte;

        _CS_L();
        for(byte = 0; byte < bytesNumber; byte++) 
        {
            valueToWrite = *(data + byte);
            for(i = 0; i < 8; i++) 
            {
                _SCLK_L();

                if(valueToWrite & 0x80) 
                {
                    _MOSI_H();    //Send one to MOSI pin
                } 
                else 
                {
                    _MOSI_L();    //Send zero to MOSI pin
                }
                valueToWrite <<= 1;   //Rotate and save data

                delay_us(10);

                readData <<= 0x01;
                readBit = _Read_MISO();
                if (readBit) 
                    readData |= 0x01;

                _SCLK_H();
                delay_us(10);
            }

            *(data + byte) = readData;
        }

        _CS_H();
        return bytesNumber;

    #endif
}
/*************************************** need to consider **************************
uint8_t SPI1_ReadByte(void)         
{        
    uint8_t retry=0;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    {
        retry++;
        if(retry>200)return 0;
    }
    retry = 0;                                                            
    return SPI_I2S_ReceiveData(SPI1);                  
}



void SPI1_SendByte(uint8_t byte)                 
{
    uint8_t retry=0;                                         
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 
    {
        retry++;
        if(retry>200) return;
    }   
    retry = 0;                        
    SPI_I2S_SendData(SPI1, byte); 
}

//spi1 interrupt handler
void SPI1_IRQHandler(void)
{         
    uint8_t buff;
    if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == SET)
    {         
        buff = SPI_I2S_ReceiveData(SPI1);
    }

}
***********************************************************************************/

/************************************* END LINE ************************************************/

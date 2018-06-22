/***************************************************************************//**
 *   @file   Main.c
 *   @brief  Implementation of the program's main function.
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
/***************************** Option Bytes ***********************************/
/******************************************************************************/
#pragma location = "OPTBYTE"
__root const unsigned char opbyte0 = 0x00; // Watchdog timer settings.
#pragma location = "OPTBYTE"
__root const unsigned char opbyte1 = 0xFD; // Low voltage detector settings.
#pragma location = "OPTBYTE"
__root const unsigned char opbyte2 = 0xF8; // Flash op. mode and osc. settings.
#pragma location = "OPTBYTE"
__root const unsigned char opbyte3 = 0x04; // On-chip Debug Option Byte.

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "YRDKRL78G13.h"    // YRDKRL78G13 definitions.
#include "ST7579.h"         // ST7579 definitions.
#include "TIME.h"           // TIME definitions.
#include "AD7175.h"         // AD7175 definitions.


void ADI_Component(unsigned char* string)
{
    const unsigned char adiComponent[7] =
    {
        0xFE, 0xFE, 0x82, 0xC6, 0xEE, 0xFE, 0xFE 
    };
    ST7579_Image(0, 0, adiComponent, 7, 8);
    ST7579_String(0, 9, string, 0);
}

void main(void)
{
    long ret      = 0;
    long timeout  = 0x00FFFFFF;
    long sample   = 0;
    float voltage = 0;
    
    /* Initialize YRDKRL78G13. */
    YRDKRL78G13_Init();
    
    /* Enable interrupts. */
    __enable_interrupt();
    
    /* Init timer. */
    TIME_Init();
        
    /* Initialize the ST7579 Display. */
    ST7579_Init();
    
    /* Initialize AD7175 device. */
    ret = AD7175_Setup();
    if(ret < 0)
    {
        ADI_Component("AD7175 Err");
    }
    else
    {
        ADI_Component("AD7175 OK");
    } 
    ST7579_String(2, 0, "CHANNEL1:", 0);
    ST7579_String(3, 0, "        (Raw)", 0);
    ST7579_String(4, 0, "CHANNEL1:", 0);
    ST7579_String(5, 0, "        (Volts)", 0);
    while(1)
    {
        ret = AD7175_WaitForReady(timeout);
        if(ret < 0)
        {
            ST7579_String(6, 0, "       ", 0);
            ST7579_String(6, 0, "Timeout", 0);
        }
        ret = AD7175_ReadData(&sample);
        if(ret < 0)
        {
            ST7579_String(7, 0, "        ", 0);
            ST7579_String(7, 0, "Read Err", 0);
        }
        ST7579_HexNumber(3, 0, sample, 0);
        voltage = ((float)sample * 5.0) / ((long)1 << 24);
        ST7579_FloatNumber(5, 0, voltage, 3, 0);
        TIME_DelayMs(300);
   }
}

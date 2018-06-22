/**************************************************************************//**
*   @file   AD7175_regs.h
*   @brief  AD7175 Registers Definitions.
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
*   SVN Revision: 0
******************************************************************************/

#ifndef __AD7175_REGS_H__
#define __AD7175_REGS_H__

/*! AD7175 register info */
typedef struct _st_reg 
{
    unsigned long addr;
    unsigned long value;
    unsigned long size;
}st_reg;

/*! AD7175 registers list*/
enum AD7175_registers
{
	Status_Register 			= 	0x00,	

	ADC_Mode_Register,						
	Interface_Mode_Register,			
	REGCHECK,											
	Data_Register,								
	GPIOCON,											
	ID_st_reg,										
	CH_Map_1,											
	CH_Map_2,											
	CH_Map_3,
	CH_Map_4,
	Setup_Config_1,
	Setup_Config_2,
	Setup_Config_3,
	Setup_Config_4,
	Filter_Config_1,
	Filter_Config_2,
	Filter_Config_3,
	Filter_Config_4,
	Offset_1,
	Offset_2,
	Offset_3,
	Offset_4,
	Gain_1,
	Gain_2,
	Gain_3,
	Gain_4,
	Communications_Register,
	AD7175_REG_NO
};

#ifdef AD7175_INIT
/*! Array holding the info for the AD7175 registers - address, initial value, size */
st_reg AD7175_regs[] = 
{
	{0x00, 0x00,   		1},			//Status_Register
	{0x01, 0x8000, 		2},			//ADC_Mode_Register
	{0x02, 0x0000, 		2},			//Interface_Mode_Register
	{0X03, 0x0000, 		3},			//REGCHECK
	{0x04, 0x0000, 		3},			//Data_Register
	{0x06, 0x0800, 		2},			//GPIOCON

	{0x07, 0x0000, 		2},			//ID_st_reg

	{0x10, 0x8001, 		2},			//CH_Map_1
	{0x11, 0x0001, 		2},			//CH_Map_2
	{0x12, 0x0001, 		2},			//CH_Map_3
	{0x13, 0x0001, 		2},			//CH_Map_4

	{0x20, 0x0320, 		2},			//Setup_Config_1
	{0x21, 0x0320, 		2},			//Setup_Config_2
	{0x22, 0x0320, 		2},			//Setup_Config_3
	{0x23, 0x0320, 		2},			//Setup_Config_4

	{0x28, 0x0500, 		2},			//Filter_Config_1
	{0x29, 0x0500, 		2},			//Filter_Config_2
	{0x2a, 0x0500, 		2},			//Filter_Config_3
	{0x2b, 0x0500, 		2},			//Filter_Config_4

	{0x30, 0x800000, 	3},			//Offset_1
	{0x31, 0x800000, 	3},			//Offset_2
	{0x32, 0x800000, 	3},			//Offset_3
	{0x33, 0x800000, 	3},			//Offset_4

	{0x38, 0x500000, 	3},			//Gain_1
	{0x39, 0x500000, 	3},			//Gain_2
	{0x3a, 0x500000, 	3},			//Gain_3
	{0x3b, 0x500000, 	3},			//Gain_4
	
	{0xFF, 0x500000, 	1}				//Communications_Register
};

#else
extern st_reg AD7175_regs[AD7175_REG_NO];
#endif

#define AD7175_SLAVE_ID    1

/* Communication Register bits */
#define COMM_REG_WEN    				(0 << 7)        //set to start communication with AD7175
#define COMM_REG_WR     				(0 << 6)        //set communication mode is write
#define COMM_REG_RD     				(1 << 6)        //set communication mode is read

/* Status Register bits */
#define STATUS_REG_RDY      			(1 << 7)        //read data from AD7175 is not used
#define STATUS_REG_ADC_ERR  			(1 << 6)        //ADC error
#define STATUS_REG_CRC_ERR  			(1 << 5)        //CRC error
#define STATUS_REG_REG_ERR  			(1 << 4)        //register error
#define STATUS_REG_CH(x)    			((x) & 0x03)    //read status about AD channel, set rang is 0x00, 0x01, 0x02, 0x03

/* ADC Mode Register */
#define ADC_MODE_REG_REF_EN         	(1 << 15)                   //enable AD7175 inside RefVoltage, Vref = 2.5V
#define ADC_MODE_REG_DELAY(x)       	(((x) & 0x7) << 8)          //set program time delay                    //datasheet page P51
#define ADC_MODE_REG_MODE(x)        	(((x) & 0x7) << 4)          //AD7175 work mode set                      //datasheet page P51
#define ADC_MODE_REG_CLKSEL(x)     		(((x) & 0x3) << 2)          //AD7175 work clock source choose           //datasheet page P51

/* Interface Mode Register bits */                              //Interface mode refer to datasheet page P52
#define INTF_MODE_REG_DOUT_RESET    	(1 << 8)
#define INTF_MODE_REG_CONT_READ     	(1 << 7)                    //连续转换模式开启
#define INTF_MODE_REG_DATA_STAT     	(1 << 6)                    //使能状态信息和通达数据一起传输。
#define INTF_MODE_REG_CRC_EN        	(0x02 << 2)
#define INTF_MODE_REG_CRC_STAT(x)   	(((x) & INTF_MODE_REG_CRC_EN) == INTF_MODE_REG_CRC_EN)          //CRC function set

/* GPIO Configuration Register */                               //Don't use GPIO Configuration register
#define GPIO_CONF_REG_MUX_IO        	(1 << 12)
#define GPIO_CONF_REG_SYNC_EN       	(1 << 11)
#define GPIO_CONF_REG_ERR_EN(x)     	(((x) & 0x3) << 9)
#define GPIO_CONF_REG_ERR_DAT       	(1 << 8)
#define GPIO_CONF_REG_IP_EN1        	(1 << 5)
#define GPIO_CONF_REG_IP_EN0        	(1 << 4)
#define GPIO_CONF_REG_OP_EN1        	(1 << 3)
#define GPIO_CONF_REG_OP_EN0        	(1 << 2)
#define GPIO_CONF_REG_DATA1         	(1 << 1)
#define GPIO_CONF_REG_DATA0         	(1 << 0)

/* ID Register */                                       
#define ID_REG_PRODUCT_ID(x)        	(((x) & 0xFF) << 8)         //AD7175 ID register, only can read, static value

/* Channel Map Register 1-4 */
#define CH_MAP_REG_CHEN         		(1 << 15)                       // channel enable
#define CH_MAP_REG_SETUP(x)     		(((x) & 0x7) << 12)             // channel function set register, value is 00, 01, 10, 11.  //datasheet page P55
#define CH_MAP_REG_AINPOS(x)    		(((x) & 0x1F) << 5)             // choose which channel is positive voltage input channel   //datasheet page P55
#define CH_MAP_REG_AINNEG(x)    		(((x) & 0x1F) << 0)             // choose which channel is positive voltage input channel   //datasheet page P56

/* Setup Configuration Register 1-4 */                          // datasheet page P57       //init value 0x1320
#define SETUP_CONF_REG_CHOP_MD(x)       (((x) & 0x3) << 14)     // set x = 0
#define SETUP_CONF_REG_BI_UNIPOLAR      (1 << 12)               // polarity set, bipolar polarity
#define SETUP_CONF_REG_UNI_POLAR        (0 << 12)               // polarity set, unipolarity polarity
#define SETUP_CONF_REG_REF_BUF_P        (1 << 11)               // don't care
#define SETUP_CONF_REG_REF_BUF_N        (1 << 10)               // don't care
#define SETUP_CONF_REG_AIN_BUF_P        (1 << 9)                // don't care
#define SETUP_CONF_REG_AIN_BUF_N        (1 << 8)                // don't care
#define SETUP_CONF_REG_BRNOUT_EN        (1 << 7)                // don't care
#define SETUP_CONF_REG_REF_SEL(x)       (((x) & 0x3) << 4)      // choose AD7175 refere voltage source, (00) for outside refer voltage; (10) for inside RefVoltage, Vref = 2.5V

/* Filter Configuration Register 1-4 */                         // datasheet page P58       //init value 0x0500
#define FILT_CONF_REG_EXTCLKFREQ(x)     (((x) & 0x3) << 13)     // datasheet page P58
#define FILT_CONF_REG_ENHFILTEN         (1 << 11)               // Filter enable
#define FILT_CONF_REG_ENHFILTSEL(x)     (((x) & 0x7) << 8)
#define FILT_CONF_REG_ORDER(x)          (((x) & 0x7) << 5)      // ADC Filter control        // datasheet page P58
#define FILT_CONF_REG_ODR(x)            (((x) & 0x1F) << 0)     // ADC output speed control  // datasheet page P58


#endif //__AD7175_REGS_H__

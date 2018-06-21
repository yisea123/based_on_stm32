#ifndef _MAIN_H_
#define _MAIN_H_

#define setbit(var, bits) ((var) |= (1 << (bits)))
#define clrbit(var, bits) ((var) &= ~(1 << (bits)))
#define testbit(var,bits) ((var)&(1<<(bits)))

#define ulong           unsigned long
#define ushort 			unsigned short
#define uchar 			unsigned char
#define uint 			unsigned int
#include "main.h"
#include<stdlib.h>
#include <iom64v.h>
#include <macros.h>
#include <string.h>
//#include <stdio.h>
#include <stdio.h>
#include "INIT.H"
#include"UART.H"
#include"string.h"
#include"adc.h"
//#include"ACSOURE.H"
//#include "AD5629.h"
#include "AD5660.h"
#include "ak8963.h"
#include <math.h>
#include "twi.h"
#include "Magnetic_Field.H"
#include "ADS8320.h"
#include "ADS1110.h"
#include "Eeprom.h"
#include "AT24C01.h"
#include "uAk8963.h"



#endif

#ifndef __AD9833_H_
#define __AD9833_H_
#include "GlobalDef.h"
#ifdef AD9833_GLOBALS
/*变量定义*/
/*函数*/
void Write_word(uint DDSdata);
void init_ad9833(void);
void output(uint32_t freq_value);//单位 1HZ
#else
/*变量声明*/
/*函数声明*/
extern void Write_word(uint DDSdata);
extern void init_ad9833(void);
extern void output(uint32_t freq_value);//单位 1HZ
#endif
#endif

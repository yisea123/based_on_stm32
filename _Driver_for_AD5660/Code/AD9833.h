#ifndef __AD9833_H_
#define __AD9833_H_
#include "GlobalDef.h"
#ifdef AD9833_GLOBALS
/*��������*/
/*����*/
void Write_word(uint DDSdata);
void init_ad9833(void);
void output(uint32_t freq_value);//��λ 1HZ
#else
/*��������*/
/*��������*/
extern void Write_word(uint DDSdata);
extern void init_ad9833(void);
extern void output(uint32_t freq_value);//��λ 1HZ
#endif
#endif

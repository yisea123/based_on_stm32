#ifndef __AD9833_H_
#define __AD9833_H_
//#ifdef AD9833_GLOBALS
/*变量定义*/
/*函数*/
#define SCL_L       GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define SCL_H       GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define SDA_L       GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define SDA_H       GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define AD9833_L    GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define AD9833_H    GPIO_SetBits(GPIOB, GPIO_Pin_5)
void Write_word(unsigned int DDSdata);
void init_ad9833(void);
void output(unsigned int freq_value);//单位 1HZ
#else
/*变量声明*/
/*函数声明*/
extern void Write_word(uint DDSdata);
extern void init_ad9833(void);
extern void output(uint32_t freq_value);//单位 1HZ
#endif

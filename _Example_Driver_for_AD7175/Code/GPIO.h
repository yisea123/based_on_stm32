/************************************************
文件：config.h
用途：系统配置函数
************************************************/
#ifndef __GPIO_H__
#define __GPIO_H__
#define Power1V8_On     GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define Power1V8_Off    GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define Power3V3_On     GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define Power3V3_Off    GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define DUT_Detecte_On  GPIO_SetBits(GPIOB,GPIO_Pin_2)
#define DUT_Detecte_Off GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define PGA_25_3V3      GPIO_ResetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define PGA_50_3V3      GPIO_ResetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define PGA_100_3V3     GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define PGA_200_3V3     GPIO_SetBits(GPIOA,GPIO_Pin_12);GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define PGA_25_1V8      GPIO_ResetBits(GPIOA,GPIO_Pin_8);GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define PGA_50_1V8      GPIO_ResetBits(GPIOA,GPIO_Pin_8);GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define PGA_100_1V8     GPIO_SetBits(GPIOA,GPIO_Pin_8);GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define PGA_200_1V8     GPIO_SetBits(GPIOA,GPIO_Pin_8);GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define I_SW1       GPIO_ResetBits(GPIOB,GPIO_Pin_1);GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define I_SW2       GPIO_ResetBits(GPIOB,GPIO_Pin_1);GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define I_SW3       GPIO_SetBits(GPIOB,GPIO_Pin_1);GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define I_SW4       GPIO_SetBits(GPIOB,GPIO_Pin_1);GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define SW_CH1       GPIO_ResetBits(GPIOA,GPIO_Pin_2);GPIO_ResetBits(GPIOA,GPIO_Pin_1);GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define SW_CH2       GPIO_ResetBits(GPIOA,GPIO_Pin_2);GPIO_ResetBits(GPIOA,GPIO_Pin_1);GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define SW_CH3       GPIO_ResetBits(GPIOA,GPIO_Pin_2);GPIO_SetBits(GPIOA,GPIO_Pin_1);GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define SW_CH4       GPIO_SetBits(GPIOA,GPIO_Pin_2);GPIO_ResetBits(GPIOA,GPIO_Pin_1);GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define SW_CH5       GPIO_SetBits(GPIOA,GPIO_Pin_2);GPIO_SetBits(GPIOA,GPIO_Pin_1);GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define SW_CH6       GPIO_SetBits(GPIOA,GPIO_Pin_2);GPIO_ResetBits(GPIOA,GPIO_Pin_1);GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define SW_CH7       GPIO_SetBits(GPIOA,GPIO_Pin_2);GPIO_SetBits(GPIOA,GPIO_Pin_1);GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define SW_CH8       GPIO_SetBits(GPIOA,GPIO_Pin_2);GPIO_SetBits(GPIOA,GPIO_Pin_1);GPIO_SetBits(GPIOA,GPIO_Pin_0)



void GPIO_Configuration(void);
void NVIC_Configuration(void);
extern void GPIO_Configuration(void);
extern void NVIC_Configuration(void);
extern float CurrentTest(uint8_t CH,uint8_t I_PGA);
#endif

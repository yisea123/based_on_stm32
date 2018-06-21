#ifndef __DELAY_H__
#define __DELAY_H__
//#include "stm32f10x.h"
//#include "GlobalDef.h"
//uint Delay1msCount=0;
//uint Delay1ms=0;
//uint Delay10ms=0;
//uint ItemDelay1ms=0;
//int OverTime;
void Delayus(u16 delayus);//实际1us
void Delayms(ulong delay);
extern uint Delay1msCount;
extern uint Delay1ms;
extern uint Delay10ms;
extern uint ItemDelay1ms;
extern int OverTime;
extern void Delayus(u16 delayus);//实际1us
extern void Delayms(ulong delay);
extern void Delay1us(u16 delayus);
#endif

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x_conf.h"
#include "usart.h"
#include "blink.h"
#include "adc.h"
#include "motor_driver.h"
#include "can.h"
#include "stdlib.h"
#include "math.h"
#include "io_operation.h"
#include "wwdg.h"
#include "current_protect.h"
#include "stm32f10x_it.h"
#include "rs485.h"
// #include "motor_trigger.h"


void PeriphDeviceInit(void);
ErrorStatus SelfCheck(void);
ErrorStatus ADC_SelfCheck(void);
ErrorStatus CAN_SelfCheck(void);
void SystemBoot(void);
#endif

/*----------------------------- END LINE ---------------------------*/

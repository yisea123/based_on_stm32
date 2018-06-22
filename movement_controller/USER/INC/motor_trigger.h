#ifndef __MOTOR_TRIGGER_H
#define __MOTOR_TRIGGER_H
#include "stm32f10x_conf.h"
#include "command.h"
#include "motor_driver.h"
#include "io_operation.h"

#define MOTOR_TRIGGER_High      GPIO_Pin_13
#define MOTOR_TRIGGER_Low       GPIO_Pin_2

typedef struct
{
    DC_MOTOR_Status PositonTrigger_High;
    DC_MOTOR_Status PositonTrigger_Low;

} DC_MOTOR_PositionTypeDef;

extern DC_MOTOR_PositionTypeDef DC_MOTOR_Position;

void MotorTriggerConfig(void);

#endif




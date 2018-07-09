#ifndef __BSP_BLE_H
#define __BSP_bLE_H

#include "stm32f4xx_hal_conf.h"
#include "usart.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"

typedef struct _buff{
	unsigned char *recBuff;
	unsigned char *sendBuff;
}bsp_ble_buff;

typedef enum{
	on = 1,
	off
}ble_status;

#define bsp_ble_port		GPIOG
#define bsp_ble_on_pin		GPIO_PIN_13


#define bsp_ble_on() 	HAL_GPIO_WritePin(bsp_ble_port, bsp_ble_on_pin, GPIO_PIN_SET);
#define bsp_ble_off() 	HAL_GPIO_WritePin(bsp_ble_port, bsp_ble_on_pin, GPIO_PIN_SET);







static void bsp_ble_gpio_init(void);
void bsp_ble_init(void);
int bsp_ble_sendByte(uint8_t byte);
int bsp_ble_sendString(uint8_t *str);
uint8_t bsp_ble_recByte(void);
char * bsp_ble_recStr(void);


#endif

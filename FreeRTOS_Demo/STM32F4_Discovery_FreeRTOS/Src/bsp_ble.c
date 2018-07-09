/*
 * bsp_ble.c
 *
 *  Created on: Jul 7, 2018
 *      Author: JiangQuan
 */

#include "bsp_ble.h"

GPIO_InitTypeDef GPIO_InitStruct;
static void bsp_ble_gpio_init(void)
{
	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOG_CLK_ENABLE();

//	  GPIO_InitStruct.Pin = ;
//	  GPIO_InitStruct.Mode = ;
//	  GPIO_InitStruct.Speed = ;
//	  GPIO_InitStruct.Pull = ;
//	  GPIO_InitStruct.Alternate = ;


}
void bsp_ble_init(void)
{
	bsp_ble_gpio_init();
}
int bsp_ble_sendByte(uint8_t byte)
{
	return 1;
}
int bsp_ble_sendString(uint8_t *str)
{
	return 1;
}
uint8_t bsp_ble_recByte(void)
{
	return 0x00;
}
char * bsp_ble_recStr(void)
{
	return NULL;
}


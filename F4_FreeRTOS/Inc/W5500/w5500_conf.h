//#ifndef _W5500_CONF_H_
//#define _W5500_CONF_H_
//
//#include "stm32f1xx_hal.h"
//#include "stdio.h"
//#include "Ethernet/W5500/types.h"
//#define __GNUC__
//
//typedef  void (*pFunction)(void);
//
////typedef  void (*pFunction)(void);
//
//extern uint8    remote_ip[4];                            	  /* Զ��IP��ַ                   */
//extern uint16   remote_port;                            	  /* Զ�˶˿ں�                   */
//extern uint16   local_port;                             	  /* ���屾�ض˿�                 */
//extern uint8    use_dhcp;                              	    /* �Ƿ�ʹ��DHCP��ȡIP           */
//extern uint8    use_eeprom;                             	  /* �Ƿ�ʹ��EEPROM�е�IP������Ϣ */
//
//
///*����SPI��ΪW5500��Ӳ���ӿ�*/
//#define WIZ_SPIx                         SPI3                   	     /* ����W5500���õ�SPI�ӿ�       */
//#define WIZ_SPIx_RCC_CLK_ENABLE()        __HAL_RCC_SPI3_CLK_ENABLE()   /* ����W5500���õ�SPI�ӿ�ʱ��   */
//
//#define WIZ_SPI_GPIO_ClK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE() /* GPIO�˿�ʱ��                 */
//#define WIZ_SPIx_GPIO_PORT               GPIOB									  	  /* GPIO�˿�                     */
//#define WIZ_SPIx_SCLK_PIN                GPIO_PIN_3						   	    /* ����W5500��ʱ�ӹܽ�          */
//#define WIZ_SPIx_MISO_PIN                GPIO_PIN_4						   	    /* ����W5500��MISO�ܽ�          */
//#define WIZ_SPIx_MOSI_PIN                GPIO_PIN_5						   	    /* ����W5500��MOSI�ܽ�          */
//
//#define WIZ_SPIx_SCS_PIN                 GPIO_PIN_11						   	  /* ����W5500��Ƭѡ�ܽ�          */
//#define WIZ_SPIx_SCS_PORT                GPIOF									   	  /* GPIO�˿�                     */
//#define WIZ_SPIx_SCS_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()	/* GPIO�˿�ʱ��                 */
//
//#define WIZ_INT_PIN                      GPIO_PIN_9									  /* ����W5500��INT�ܽ�           */
//#define WIZ_INT_PORT                     GPIOF									  	  /* GPIO�˿�                     */
//#define WIZ_INT_CLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE() /* GPIO�˿�ʱ��                 */
//
//#define WIZ_RESET_PIN                   GPIO_PIN_8									  /* ����W5500��INT�ܽ�           */
//#define WIZ_RESET_PORT                  GPIOF									  	    /* GPIO�˿�                     */
//#define WIZ_RESET_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE() /* GPIO�˿�ʱ��                 */
//
//#define FW_VER_HIGH  						1
//#define FW_VER_LOW    					0
//#define ON	                 		1
//#define OFF	                 		0
//#define HIGH	           	 			1
//#define LOW		             			0
//
//#define MAX_BUF_SIZE		 				1460       			            /*����ÿ�����ݰ��Ĵ�С*/
//#define KEEP_ALIVE_TIME	     		30	// 30sec
//#define TX_RX_MAX_BUF_SIZE      2048
//#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)
//
//#define IP_FROM_DEFINE	        0       			              /*ʹ�ó�ʼ�����IP��Ϣ*/
//#define IP_FROM_DHCP	          1       			              /*ʹ��DHCP��ȡIP��Ϣ*/
//#define IP_FROM_EEPROM	        2       			              /*ʹ��EEPROM�����IP��Ϣ*/
//extern uint8	ip_from;											                /*ѡ��IP��Ϣ����Դ*/
//#pragma pack(1)
///*�˽ṹ�嶨����W5500�ɹ����õ���Ҫ����*/
//typedef struct _CONFIG_MSG
//{
//  uint8 mac[6];																							/*MAC��ַ*/
//  uint8 lip[4];																							/*local IP����IP��ַ*/
//  uint8 sub[4];																							/*��������*/
//  uint8 gw[4];																							/*����*/
//  uint8 dns[4];																							/*DNS��������ַ*/
//  uint8 rip[4];																							/*remote IPԶ��IP��ַ*/
//	uint8 sw_ver[2];																					/*����汾��*/
//
//}CONFIG_MSG;
//#pragma pack()
//
//#pragma pack(1)
///*�˽ṹ�嶨����eepromд��ļ����������ɰ����޸�*/
//typedef struct _EEPROM_MSG
//{
//	uint8 mac[6];																							/*MAC��ַ*/
//  uint8 lip[4];																							/*local IP����IP��ַ*/
//  uint8 sub[4];																							/*��������*/
//  uint8 gw[4];																							/*����*/
//}EEPROM_MSG_STR;
//#pragma pack()
//
//extern EEPROM_MSG_STR EEPROM_MSG;
//extern CONFIG_MSG  	ConfigMsg;
//extern uint8 dhcp_ok;																				/*DHCP��ȡ�ɹ�*/
//extern uint32	dhcp_time;																		/*DHCP���м���*/
//extern __IO uint8_t	ntptimer;																				/*NPT�����*/
//
///*MCU������غ���*/
//void gpio_for_w5500_config(void);														/*SPI�ӿ�reset ���ж�����*/
//void timer2_init(void);																			/*STM32��ʱ��2��ʼ��*/
//void timer2_isr(void);																			/*��ʱ���ж�ִ�еĺ���*/
//void reboot(void);																					/*STM32��λ*/
//void write_config_to_eeprom(void);													/*д������Ϣ��EEPROM��*/
//void read_config_from_eeprom(void);													/*��EEPROM�ж�����Ϣ*/
//
///*W5500SPI��غ���*/
//void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*д��һ��8λ���ݵ�W5500*/
//uint8 IINCHIP_READ(uint32 addrbsb);													/*��W5500����һ��8λ����*/
//uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*��W5500д��len�ֽ�����*/
//uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*��W5500����len�ֽ�����*/
//
///*W5500����������غ���*/
//void reset_w5500(void);																			/*Ӳ��λW5500*/
//void set_w5500_mac(void);																		/*����W5500��MAC��ַ*/
//void set_w5500_ip(void);																		/*����W5500��IP��ַ*/
//
///*��Ҫ�ö�ʱ�ĵ�Ӧ�ú���*/
//void dhcp_timer_init(void);																	/*dhcp�õ��Ķ�ʱ����ʼ��*/
//void ntp_timer_init(void);																	/*npt�õ��Ķ�ʱ����ʼ��*/
//
//
//#endif

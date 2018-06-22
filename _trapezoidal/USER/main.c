/*******************************************************************************
* �ļ���          : main.c
* ����            : �����
* ʱ��			  : 13/06/2015
* �ļ�����        : ������
********************************************************************************/

/* ͷ�ļ�    ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "stdio.h"

/* �Զ���ͬ��ؼ���    --------------------------------------------------------*/ 


#define NAME  1          

#define OFF  1
#define ON   0

 
#define TIM2_CR1              ( *( (volatile  u32 *) (0x40000000+0x00) ) )
#define TIM2_CR2              ( *( (volatile  u32 *) (0x40000000+0x04) ) )
#define TIM2_SMCR             ( *( (volatile  u32 *) (0x40000000+0x08) ) )
#define TIM2_SR               ( *( (volatile  u32 *) (0x40000000+0x10) ) )
#define TIM2_CNT              ( *( (volatile  u32 *) (0x40000000+0x24) ) )
#define TIM2_ARR              ( *( (volatile  u32 *) (0x40000000+0x2c) ) )


#define RD_X_TIM   (TIM2->CR1&0x01)
#define START_X_TIM   (TIM2->CR1|=0x01)
#define STOP_X_TIM   (TIM2->CR1&=0xFFFE)


/* �Զ��庯����        --------------------------------------------------------*/ 
#define  TIM_CRYSTALL      36000000L
/* �Զ������          --------------------------------------------------------*/ 

//    ��ʼ�ٶ�       ����ٶ�
u32  start_speed, high_speed;
//   ��ǰ������   ����������     �������壬    Ԥ���������         ���ٴ�����   Ԥ����ٴ���
u32  pulse,   pulse_total, change_pulse, set_change_pulse[50], add_times,   set_add_times;
u32   add_pulse_sum[50];
u32   uniform_velocity_pulse;		//����·��
u16   tim_value[50];
u32   add_inc_status;


/* �Զ��庯������      --------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration(void);
void USART_Configuration(void);
void Systick_Configuration(void);
void  start_motor(u32 start_sp,  u32 high_sp,  u8 add_ms, u32 pulse_total, u8 dir );

void delay_ms(u16 n)
{
    u16 tmp;
	while(n--)
	{
	    tmp=8030;
		while(tmp--);
	}
}
int fputc(int ch, FILE *f)		//�ع� print
{
	USART_SendData(USART1, (u8)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}

/*******************************************************************************
* ������		 : main
* ��������       : Main ����
* �������       : ��
* ������       : ��
* ����ֵ         : ��
*******************************************************************************/
int main(void)
{          
  	int i;
	RCC_Configuration();    /* ����ϵͳʱ�� */
  	GPIO_Configuration();   /* ����GPIO�˿� */ 
    NVIC_Configuration();   /* �����жϵȼ� */       
    USART_Configuration();  /* ���ô������� */         	    
    //Systick_Configuration();   /*�δ�ʱ��*/	
    TIM_Configuration();    /* ��ʱ����ʼ�� */  
	
	printf("\r\n");
	printf("reset\r\n");
	printf("��Ȼ����ʾ���ģ� �棬�����������\r\n");
	while(1)
	{
		delay_ms(500);
		//GPIO_SetBits(GPIOA, GPIO_Pin_0); //OUT1
		//GPIO_ResetBits(GPIOA, GPIO_Pin_11); 
		//  ��ʼ�ٶ�   ����ٶ�    ����ʱ��      ��������    �˶�����
		start_motor(4000,60000,60,20000,0);
		printf("start\r\n");
		while(RD_X_TIM);
		printf("over\r\n");
		delay_ms(500);
		//GPIO_ResetBits(GPIOA, GPIO_Pin_0);  //OUT1
		//GPIO_SetBits(GPIOA, GPIO_Pin_11); 
	}
}


//  ��ʼ�ٶ�   ����ٶ�    ����ʱ��      ��������    �˶�����
void  start_motor(u32 start_sp,  u32 high_sp,  u8 add_ms, u32 pulse_total, u8 dir )
{
	float  set_ms;
	u32    i,    add_speed;
	if(start_sp<TIM_CRYSTALL/65000)	
	{
		start_sp =  TIM_CRYSTALL/65000;
	}
	
	if(high_sp>TIM_CRYSTALL/500)
	{ 
			high_sp = TIM_CRYSTALL/500;
	}
	start_speed  = start_sp;
	high_speed   = high_sp;
	
	add_speed =  (high_sp-start_sp)/50;			//����ÿ�μ��ٵķ��ȣ�ͨ��50��С���ȼ������ﵽ����ٶ�
	set_ms  = add_ms/50;						//����ÿ��С���ȼ��ٵļ��ʱ��    ��λ������
	for(i=0; i<50; i++)
	{
		tim_value[i]  =  TIM_CRYSTALL/start_speed;
		change_pulse  =  start_speed*set_ms/1000;
		set_change_pulse[i]=change_pulse;
		start_speed   =  start_speed+add_speed;
		if(i==0)	add_pulse_sum[i] = change_pulse;
		else 		add_pulse_sum[i] = add_pulse_sum[i-1]+change_pulse;
	}
	set_add_times =50;
	
	while(set_add_times)
	{
		if( add_pulse_sum[set_add_times] < pulse_total/2)
		{
			break;
		}
		else
		{
			set_add_times--;
		}
	}
	
	if(set_add_times)
	{
		uniform_velocity_pulse  = pulse_total -  add_pulse_sum[set_add_times-1]*2;
		add_inc_status=1;
		add_times=0;
		change_pulse = set_change_pulse[add_times];
		
		add_times=0;
		pulse =0;
		TIM2_ARR = tim_value[add_times] ; 	
		TIM2_CNT = 0xfffe;
		START_X_TIM;
	}
	else
	{
		change_pulse = pulse_total;
		add_inc_status=4;
		add_times=0;
		pulse =0;
		TIM2_ARR = tim_value[add_times] ; 	
		TIM2_CNT = 0xfffe;
		START_X_TIM;
	}
}





/*******************************************************************************
* ������	: RCC_Configuration
* ��������  : ����ϵͳ������ʱ��
* �������  : ��
* ������  : ��
* ����ֵ    : ��
*******************************************************************************/
void RCC_Configuration(void)
{	
	ErrorStatus HSEStartUpStatus;                   /* ����ö�����ͱ��� HSEStartUpStatus */
    RCC_DeInit();                                   /* ��λϵͳʱ������*/
  	RCC_HSEConfig(RCC_HSE_ON);     	                /* ����HSE*/
  	HSEStartUpStatus = RCC_WaitForHSEStartUp();     /* �ȴ�HSE�����ȶ�*/
  	if(HSEStartUpStatus == SUCCESS)                 /* �ж�HSE���Ƿ���ɹ����������if()�ڲ� */
  	{    	
    	RCC_HCLKConfig(RCC_SYSCLK_Div1);            /* ѡ��HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ */    	
    	RCC_PCLK2Config(RCC_HCLK_Div1);             /* ѡ��PCLK2ʱ��ԴΪ HCLK��AHB�� 1��Ƶ */    	
    	RCC_PCLK1Config(RCC_HCLK_Div2);             /* ѡ��PCLK1ʱ��ԴΪ HCLK��AHB�� 2��Ƶ */ 
    	FLASH_SetLatency(FLASH_Latency_2);          /* ����FLASH��ʱ������Ϊ2 */
       	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   /* ʹ��FLASHԤȡ���� */
          	
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);    /* ѡ�����໷��PLL��ʱ��ԴΪHSE 1��Ƶ����Ƶ��Ϊ9����PLL���Ƶ��Ϊ 8MHz * 9 = 72MHz */   	
    	RCC_PLLCmd(ENABLE);                                     /* ʹ��PLL */     	
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);     /* �ȴ�PLL����ȶ� */    	
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              /* ѡ��SYSCLKʱ��ԴΪPLL */    	
    	while(RCC_GetSYSCLKSource() != 0x08);                   /* �ȴ�PLL��ΪSYSCLKʱ��Դ */
  	}   	
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|
                            RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1  | RCC_APB2Periph_TIM8  | RCC_APB2Periph_USART1, ENABLE);
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR |RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|
                           RCC_APB1Periph_TIM4, ENABLE);    
}



/*******************************************************************************
* ������  		: GPIO_Configuration
* ��������    	: ���ø�GPIO�˿ڹ���
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/

void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;   
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE); // ʧ�� JTAG �� SWJ        
    //GPIO_DeInit(GPIOA);
    //GPIO_AFIODeInit();
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    
    /* ����PD    ���תƵ��Ϊ50MHz*/    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD , &GPIO_InitStructure);         
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//����  out  1- 8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);         
   
    //  RXD TXD
    /* ����USART1��Tx�ţ�PA.9��Ϊ�ڶ������������ģʽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);    
    /* ����USART1��Rx�ţ�PA.10��Ϊ��������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
}

// /*******************************************************************************
// * ������  		: TIM_Configuration
// * ��������    	: ���ø�TIM�˿ڹ���
// * �������      : ��
// * ������      : ��
// * ����ֵ        : ��
// *******************************************************************************/

void TIM_Configuration(void)
{
	/* ���� TIM_TimeBase ��ʼ���ṹ�� TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;			
//tim2
	TIM_TimeBaseStructure.TIM_Period = 520;   ///������ʼֵ��������������ʱҪ���¸�ֵ��
	TIM_TimeBaseStructure.TIM_Prescaler = 1;	//	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);        

	
    TIM_ARRPreloadConfig(TIM2,ENABLE);				//  ������
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	  	//ʹ���ж�         DISABLE
	TIM_ClearFlag(TIM2, TIM_IT_Update);
    /* ���� TIM ���� */    
	TIM_Cmd(TIM2 , DISABLE);	
	
}
// /*******************************************************************************
// * ������  		: NVIC_Configuration
// * ��������    	: ���ø�nvic�˿ڹ���
// * �������      : ��
// * ������      : ��
// * ����ֵ        : ��
// *******************************************************************************/

void 	NVIC_Configuration(void)
{
/* ���� NVIC ��ʼ���ṹ�� */
	NVIC_InitTypeDef NVIC_InitStructure;
	
		/* #ifdef...#else...#endif�ṹ�������Ǹ���Ԥ�������������ж���������ʼ��ַ*/   
#ifdef  VECT_TAB_RAM  
  		/* �ж���������ʼ��ַ�� 0x20000000 ��ʼ */ 
 		NVIC_SetVectorTable(NVIC_VectTab_RAM , 0x0); 
#else 	/* VECT_TAB_FLASH */
  		/* �ж���������ʼ��ַ�� 0x80000000 ��ʼ */ 
  		NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0x0);   
#endif

	/* ѡ�����ȼ�����0 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* ���� TIM2 �ж�, 1����ռ���ȼ���0����ռ���ȼ� */	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ENABLE;  DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	/* ���� TIM3 �ж�, 1����ռ���ȼ���0����ռ���ȼ� */	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    /* ���� TIM4 �ж�, 1����ռ���ȼ���0����ռ���ȼ� */	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);    
	
	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
		
}



/*******************************************************************************
* ������  		: USART_Configuration
* ��������    	: ����USART1
* �������      : None
* ������      : None
* ����ֵ        : None
*******************************************************************************/
    /*	
    *	������Ϊ9600bps
    *	8λ���ݳ���
    *	1��ֹͣλ����У��
    *	����Ӳ��������
    *	��ֹUSARTʱ��
    *	ʱ�Ӽ��Ե�
    *	�ڵ�2�����ز�������
    *	���һλ���ݵ�ʱ�����岻�� SCLK ���
    */ 

void USART_Configuration(void)
{
	USART_InitTypeDef 	USART_InitStructure;
  	USART_ClockInitTypeDef  USART_ClockInitStructure;

	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1 , &USART_ClockInitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//8λ�ֳ���ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;									//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;						//���ͣ�����ģʽ
	USART_Init(USART1, &USART_InitStructure);											//��ʼ������
	

	USART_ClearFlag(USART1, USART_FLAG_RXNE); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);										//ʹ�ܽ����ж�
	
	USART_Cmd(USART1, ENABLE);														    //ʹ�ܴ���
}





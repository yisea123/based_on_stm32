/*******************************************************************************
* 文件名          : main.c
* 作者            : 陈昊昱
* 时间			  : 13/06/2015
* 文件描述        : 主函数
********************************************************************************/

/* 头文件    ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "stdio.h"

/* 自定义同义关键字    --------------------------------------------------------*/ 


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


/* 自定义函数宏        --------------------------------------------------------*/ 
#define  TIM_CRYSTALL      36000000L
/* 自定义变量          --------------------------------------------------------*/ 

//    开始速度       最高速度
u32  start_speed, high_speed;
//   当前脉冲数   脉冲总数，     变速脉冲，    预设加速脉冲         加速次数，   预设加速次数
u32  pulse,   pulse_total, change_pulse, set_change_pulse[50], add_times,   set_add_times;
u32   add_pulse_sum[50];
u32   uniform_velocity_pulse;		//匀速路程
u16   tim_value[50];
u32   add_inc_status;


/* 自定义函数声明      --------------------------------------------------------*/
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
int fputc(int ch, FILE *f)		//重构 print
{
	USART_SendData(USART1, (u8)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}

/*******************************************************************************
* 函数名		 : main
* 函数描述       : Main 函数
* 输入参数       : 无
* 输出结果       : 无
* 返回值         : 无
*******************************************************************************/
int main(void)
{          
  	int i;
	RCC_Configuration();    /* 设置系统时钟 */
  	GPIO_Configuration();   /* 设置GPIO端口 */ 
    NVIC_Configuration();   /* 设置中断等级 */       
    USART_Configuration();  /* 设置串口配置 */         	    
    //Systick_Configuration();   /*滴答定时器*/	
    TIM_Configuration();    /* 定时器初始化 */  
	
	printf("\r\n");
	printf("reset\r\n");
	printf("竟然能显示中文？ 噫，还有这操作？\r\n");
	while(1)
	{
		delay_ms(500);
		//GPIO_SetBits(GPIOA, GPIO_Pin_0); //OUT1
		//GPIO_ResetBits(GPIOA, GPIO_Pin_11); 
		//  初始速度   最高速度    加速时间      脉冲总数    运动方向
		start_motor(4000,60000,60,20000,0);
		printf("start\r\n");
		while(RD_X_TIM);
		printf("over\r\n");
		delay_ms(500);
		//GPIO_ResetBits(GPIOA, GPIO_Pin_0);  //OUT1
		//GPIO_SetBits(GPIOA, GPIO_Pin_11); 
	}
}


//  初始速度   最高速度    加速时间      脉冲总数    运动方向
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
	
	add_speed =  (high_sp-start_sp)/50;			//计算每次加速的幅度，通过50次小幅度加速来达到最高速度
	set_ms  = add_ms/50;						//加速每次小幅度加速的间隔时间    单位：毫秒
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
* 函数名	: RCC_Configuration
* 函数描述  : 设置系统各部分时钟
* 输入参数  : 无
* 输出结果  : 无
* 返回值    : 无
*******************************************************************************/
void RCC_Configuration(void)
{	
	ErrorStatus HSEStartUpStatus;                   /* 定义枚举类型变量 HSEStartUpStatus */
    RCC_DeInit();                                   /* 复位系统时钟设置*/
  	RCC_HSEConfig(RCC_HSE_ON);     	                /* 开启HSE*/
  	HSEStartUpStatus = RCC_WaitForHSEStartUp();     /* 等待HSE起振并稳定*/
  	if(HSEStartUpStatus == SUCCESS)                 /* 判断HSE起是否振成功，是则进入if()内部 */
  	{    	
    	RCC_HCLKConfig(RCC_SYSCLK_Div1);            /* 选择HCLK（AHB）时钟源为SYSCLK 1分频 */    	
    	RCC_PCLK2Config(RCC_HCLK_Div1);             /* 选择PCLK2时钟源为 HCLK（AHB） 1分频 */    	
    	RCC_PCLK1Config(RCC_HCLK_Div2);             /* 选择PCLK1时钟源为 HCLK（AHB） 2分频 */ 
    	FLASH_SetLatency(FLASH_Latency_2);          /* 设置FLASH延时周期数为2 */
       	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   /* 使能FLASH预取缓存 */
          	
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);    /* 选择锁相环（PLL）时钟源为HSE 1分频，倍频数为9，则PLL输出频率为 8MHz * 9 = 72MHz */   	
    	RCC_PLLCmd(ENABLE);                                     /* 使能PLL */     	
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);     /* 等待PLL输出稳定 */    	
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              /* 选择SYSCLK时钟源为PLL */    	
    	while(RCC_GetSYSCLKSource() != 0x08);                   /* 等待PLL成为SYSCLK时钟源 */
  	}   	
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|
                            RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1  | RCC_APB2Periph_TIM8  | RCC_APB2Periph_USART1, ENABLE);
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR |RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|
                           RCC_APB1Periph_TIM4, ENABLE);    
}



/*******************************************************************************
* 函数名  		: GPIO_Configuration
* 函数描述    	: 设置各GPIO端口功能
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/

void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;   
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE); // 失能 JTAG 和 SWJ        
    //GPIO_DeInit(GPIOA);
    //GPIO_AFIODeInit();
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    
    /* 设置PD    最大翻转频率为50MHz*/    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD , &GPIO_InitStructure);         
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽  out  1- 8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);         
   
    //  RXD TXD
    /* 设置USART1的Tx脚（PA.9）为第二功能推挽输出模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);    
    /* 设置USART1的Rx脚（PA.10）为浮空输入脚 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
}

// /*******************************************************************************
// * 函数名  		: TIM_Configuration
// * 函数描述    	: 设置各TIM端口功能
// * 输入参数      : 无
// * 输出结果      : 无
// * 返回值        : 无
// *******************************************************************************/

void TIM_Configuration(void)
{
	/* 定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;			
//tim2
	TIM_TimeBaseStructure.TIM_Period = 520;   ///随便给初始值，反正待会启动时要重新赋值的
	TIM_TimeBaseStructure.TIM_Prescaler = 1;	//	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);        

	
    TIM_ARRPreloadConfig(TIM2,ENABLE);				//  自重载
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	  	//使能中断         DISABLE
	TIM_ClearFlag(TIM2, TIM_IT_Update);
    /* 启动 TIM 计数 */    
	TIM_Cmd(TIM2 , DISABLE);	
	
}
// /*******************************************************************************
// * 函数名  		: NVIC_Configuration
// * 函数描述    	: 设置各nvic端口功能
// * 输入参数      : 无
// * 输出结果      : 无
// * 返回值        : 无
// *******************************************************************************/

void 	NVIC_Configuration(void)
{
/* 定义 NVIC 初始化结构体 */
	NVIC_InitTypeDef NVIC_InitStructure;
	
		/* #ifdef...#else...#endif结构的作用是根据预编译条件决定中断向量表起始地址*/   
#ifdef  VECT_TAB_RAM  
  		/* 中断向量表起始地址从 0x20000000 开始 */ 
 		NVIC_SetVectorTable(NVIC_VectTab_RAM , 0x0); 
#else 	/* VECT_TAB_FLASH */
  		/* 中断向量表起始地址从 0x80000000 开始 */ 
  		NVIC_SetVectorTable(NVIC_VectTab_FLASH , 0x0);   
#endif

	/* 选择优先级分组0 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* 开启 TIM2 中断, 1级先占优先级，0级后占优先级 */	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ENABLE;  DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	/* 开启 TIM3 中断, 1级先占优先级，0级后占优先级 */	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    /* 开启 TIM4 中断, 1级先占优先级，0级后占优先级 */	
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
* 函数名  		: USART_Configuration
* 函数描述    	: 设置USART1
* 输入参数      : None
* 输出结果      : None
* 返回值        : None
*******************************************************************************/
    /*	
    *	波特率为9600bps
    *	8位数据长度
    *	1个停止位，无校验
    *	禁用硬件流控制
    *	禁止USART时钟
    *	时钟极性低
    *	在第2个边沿捕获数据
    *	最后一位数据的时钟脉冲不从 SCLK 输出
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
	
	USART_InitStructure.USART_BaudRate = 115200;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//8位字长格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;									//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;						//发送｜接收模式
	USART_Init(USART1, &USART_InitStructure);											//初始化串口
	

	USART_ClearFlag(USART1, USART_FLAG_RXNE); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);										//使能接收中断
	
	USART_Cmd(USART1, ENABLE);														    //使能串口
}





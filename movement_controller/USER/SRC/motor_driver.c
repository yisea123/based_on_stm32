/**
 * @brief:  PWM generate files
 * @Author:
 * @Data:
 */
#include "motor_driver.h"
/** extern struct typedef ------------------------------------*/
DC_MOTOR_ControlParamTypeDef DC_MOTOR_ControlParam;
DC_MOTOR_ControlStatusTypeDef DC_MOTOR_RunStatus;
uint32_t pulseCounter = 0;
ErrorStatus errorTemp;
/**
  * @brief:  Configure the TIM1 PWM pins and motor driver power supply pin.
  * @param:  None
  * @return: None
  */
#ifdef AC200_DEBUG

/** PWM 单路调制 */
void DC_MOTOR_CTR_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/** configurate the PWM PWM pin ----------------------*/
	GPIO_InitStructure.GPIO_Pin   = DC_MOTOR_1A | DC_MOTOR_2A;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = DC_MOTOR_1B;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = DC_MOTOR_2B;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTB, &GPIO_InitStructure);

	/** configurate the motor driver power pin ------------------*/
	GPIO_InitStructure.GPIO_Pin   = DC_MOTOR_POWER_CTR;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTB, &GPIO_InitStructure);
	DC_MOTOR_BOOT_V_DISABLE(); // default value set

	/** configurate the motor PWM enable pin -------------------*/
	GPIO_InitStructure.GPIO_Pin   = DC_MOTOR_PWM_CTR;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTB, &GPIO_InitStructure);
	DC_MOTOR_PWM_DISABLE(); // default value set

	/** typedef value init -------------------------------------*/
	DC_MOTOR_RunStatus.DC_MOTOR_PowerStatus        = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_PWM_Status         = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_1A_Status          = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_1B_Status          = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_2A_Status          = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_2B_Status          = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus  = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_OutputStatus       = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_SpeedupStatus      = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_SpeedDownStatus    = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_Standby            = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_CurrentProtection  = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_CMD                = ERROR;
}
void DC_MOTOR_DirectionSet(uint8_t motorDirection, uint8_t targetSpeedPercent)
{
    if(ON == DC_MOTOR_RunStatus.DC_MOTOR_OutputStatus)//启动过
    {
		//方向设置和现在的方向不相同
		if((motorDirection != Forwared) && (DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus == ON))
		{
            DC_MOTOR_Stop();
			DC_MOTOR_1B_Low();
			DC_MOTOR_2B_Low();
			DelayMs(100);
            /** set H bridge Low side -------------------------------*/
            DC_MOTOR_2B_High();
            DC_MOTOR_1B_Low();
            DelayMs(50);
            TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
            TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
            DelayMs(100);
            
            DC_MOTOR_PWM(ENABLE);
            DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus = OFF;
            DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus = ON;
			DelayMs(100);
            DC_MOTOR_SpeedUp(targetSpeedPercent);
            
        }
        else if((motorDirection == Forwared) && (DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus == ON))
        {
            DC_MOTOR_Stop();
            DC_MOTOR_PWM(DISABLE);
            DC_MOTOR_1B_Low();
            DC_MOTOR_2B_Low();
            DelayMs(100);
            /** set H bridge Low side -------------------------------*/
            DC_MOTOR_1B_High();
            DC_MOTOR_2B_Low();
            DelayMs(50);
            TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
            TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
            DelayMs(100);
            DC_MOTOR_PWM(ENABLE);
            DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus = ON;
            DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus = OFF;
            DC_MOTOR_SpeedUp(targetSpeedPercent);
        }
	}
    else    //第一次启动
    {
        if(motorDirection == Forwared)
        {
            
            DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus  = ON;
            DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus = OFF;
            /** set H bridge Low side -------------------------------*/
            DC_MOTOR_1B_High();
            DC_MOTOR_2B_Low();
            DelayMs(100);
            TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
            TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
            DelayMs(100);
            DC_MOTOR_SpeedUp(targetSpeedPercent);
        }
        else
        {
            DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus  = OFF;
            DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus = ON;
            /** set H bridge Low side -------------------------------*/
            DC_MOTOR_2B_High();
            DC_MOTOR_1B_Low();
            DelayMs(100);
            TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
            TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
            DelayMs(100);
        }
        DC_MOTOR_PWM(ENABLE);
        DC_MOTOR_RunStatus.DC_MOTOR_PWM_Status  = ON;
        DelayMs(100);
        DC_MOTOR_SpeedUp(targetSpeedPercent);
    }
}

/**
 *    [Brushed DC Motor Config description]
 *    @param  dir  [Direction for Motor, can choose Forwared or Reversed]
 */
void DC_MOTOR_Init(void)
{
	TIM_TimeBaseInitTypeDef 		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef 				TIM_OCInitStructure;
	TIM_DeInit(TIM1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	DC_MOTOR_CTR_GPIO_Config();

	TIM_TimeBaseStructure.TIM_Period = (6000 - 1);			//72M / 4800 = 15KHz		// timmer clock cycle time APB2_Freq / 100 = 1MHz / 100 = 10KHz
	TIM_TimeBaseStructure.TIM_Prescaler = 0;				//No Prescaler, APB2 Freq = system freq / 72MHz = 1MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM_CounterMode_CenterAligned2;       // center alignment mode.
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// timmer clock don't divide
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			// repet counter
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/** Channel 1 Configuration in PWM mode ---------------------------------*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_Pulse = 400; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_OCInitStructure.TIM_Pulse = 400;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE); 
	TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    
    
	DC_MOTOR_ControlParam.dutyCycleParam = 400;
}

#else
/** PWM，互补输出 */
void DC_MOTOR_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOE Configuration: Channel 1/1N, 2/2N, as alternate function push-pull */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
	/** Disable JTAG ---------------------------------------*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //for using PB3, PB4
	/** configurate the motor PWM pin ----------------------*/
	GPIO_InitStructure.GPIO_Pin = DC_MOTOR_1A | DC_MOTOR_1B | DC_MOTOR_2A;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DC_MOTOR_2B;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTB, &GPIO_InitStructure);

	/** configurate the motor driver power pin ------------------*/
	GPIO_InitStructure.GPIO_Pin = DC_MOTOR_POWER_CTR;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTB, &GPIO_InitStructure);
	DC_MOTOR_BOOT_V_DISABLE(); // default value set

	/** configurate the motor PWM enable pin -------------------*/
	GPIO_InitStructure.GPIO_Pin = DC_MOTOR_PWM_CTR;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORTB, &GPIO_InitStructure);
	DC_MOTOR_PWM_DISABLE(); // default value set

	/** typedef value init -------------------------------------*/
	DC_MOTOR_RunStatus.DC_MOTOR_PowerStatus        = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_PWM_Status         = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_1A_Status          = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_1B_Status          = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_2A_Status          = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_2B_Status          = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus  = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_OutputStatus       = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_SpeedupStatus      = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_SpeedDownStatus    = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_Standby            = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_CurrentProtection  = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_CMD                = ERROR;
}
/**
 *    [PWM_Config description]
 *
 *    @param  PWM_Set  [description]
 */

void DC_MOTOR_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	/*----- default parameter ------------------------------*/
	DC_MOTOR_ControlParam.TimerPeriod = 3600;
	DC_MOTOR_ControlParam.PWM_CompareMode = TIM_OCMode_PWM1;
	DC_MOTOR_ControlParam.Channel1Pulse = 0;
	DC_MOTOR_ControlParam.Channel2Pulse = 0;

	DC_MOTOR_ControlParam.PWM_DeadTime = 0xBF;
	DC_MOTOR_ControlParam.TimmerCounter = 0;

	/*----------------------------------------------------------------------------------------*/
	TIM_DeInit(TIM1);
	DC_MOTOR_GPIO_Config();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = DC_MOTOR_ControlParam.TimerPeriod; // timmer clock cycle time
	TIM_TimeBaseStructure.TIM_Prescaler = 0;								  //No Prescaler, AHB Freq                                     = system freq
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned2;   //TIM_CounterMode_CenterAligned2;       // center alignment mode.
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;					  // timmer clock don't divide
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						  // repet counter
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/*-------------------------------------------------------------------
	--------------------------------------------------------------------*/
	TIM_BDTRInitTypeDef 		TIM_BDTRInitStructure;
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	TIM_BDTRInitStructure.TIM_DeadTime = DC_MOTOR_ControlParam.PWM_DeadTime;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable; // break var
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

	/** Channel 1, 2 Configuration in PWM mode ---------------------------------*/
	TIM_OCInitStructure.TIM_OCMode = DC_MOTOR_ControlParam.PWM_CompareMode;  // mode set to control the pwm CCR calculate direction
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;			 //positive pwm
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;			 //negtive pwm
	TIM_OCInitStructure.TIM_Pulse = DC_MOTOR_ControlParam.Channel1Pulse - 1; //duty cycle
	/** OCIdlestate configurate ------------------------------------------------*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	/*----- OC1 --------------------*/
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); // TIM1 Init
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	/*----- OC2 --------------------*/
	TIM_OCInitStructure.TIM_Pulse = DC_MOTOR_ControlParam.Channel2Pulse - 1;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);   //for break input
	TIM_ARRPreloadConfig(TIM1, ENABLE); // Enable Timmer reload function
}
#endif
/**
 * @brief: function is about set DC_MOTOR to ready status. 
 * @param: None
 * @return: SUCCESS or ERROR.
 */
ErrorStatus DC_MOTOR_Ready(FunctionalState NewState)
{
    if(ENABLE == NewState)
	{
		TIM_Cmd(TIM1, ENABLE);
		DelayMs(100);
		DC_MOTOR_RunStatus.DC_MOTOR_OutputStatus = ON;
		return SUCCESS;
	}
	else
	{
		TIM_Cmd(TIM1, DISABLE);
		DelayMs(100);
		DC_MOTOR_RunStatus.DC_MOTOR_OutputStatus = OFF;
		return SUCCESS;
	}
}
ErrorStatus DC_MOTOR_Periph(FunctionalState NewState)
{
    if(ENABLE == NewState)
    {
        DC_MOTOR_Power(ENABLE);
        DelayMs(100);
        DC_MOTOR_PWM(ENABLE);
        DelayMs(100);
        return SUCCESS;
    }
    else
    {
        
        DC_MOTOR_PWM(DISABLE);
        DelayMs(100); 
        DC_MOTOR_Power(DISABLE);
        DelayMs(100); 
        return SUCCESS;
    }
}

/**	
 * @brief: function can check Motor is existence or not connect to the control board.
 * @param: NO.
 * @return: return check answer,return value is true or false. @refer: truthTable.
 */
ErrorStatus NGDC_MOTOR_Check(void)
{
	; //PWM configurate in puls mode, ADC read the current value.
	return SUCCESS;
}

/**
 * @brief: Enable the H-bridge driver chip power, ready to send PWM signal.
 * @param: NULL
 * @return: NULL
 */
ErrorStatus DC_MOTOR_Power(FunctionalState NewState)
{
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState == ENABLE)
	{
		DC_MOTOR_BOOT_V_EN();
		DelayMs(200);
		DC_MOTOR_RunStatus.DC_MOTOR_PowerStatus = ON;
		return SUCCESS;
	}
	else
	{
		DC_MOTOR_BOOT_V_DISABLE();
		DelayMs(200);
		DC_MOTOR_RunStatus.DC_MOTOR_PowerStatus = OFF;
		return SUCCESS;
	}
}

/**
 * @breif: PWM send as confgurated parameter.
 * @param: DC_MOTOR_ControlParam struct, call DC_MOTOR_Config function.
 * @return: NULL
 */
ErrorStatus DC_MOTOR_PWM(FunctionalState NewState)
{
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState == ENABLE)
	{
		DC_MOTOR_PWM_EN();
		DC_MOTOR_RunStatus.DC_MOTOR_PWM_Status = ON;
		return SUCCESS;
	}
	else
	{
		DC_MOTOR_PWM_DISABLE();
		DC_MOTOR_RunStatus.DC_MOTOR_PWM_Status = OFF;
		return SUCCESS;
	}
}

void DC_MOTOR_SpeedFixed(uint8_t targetSpeedPercent)
{
    if((ON == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus) && (OFF == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus))
    {
        TIM1 ->CCR2 = (targetSpeedPercent * MAX_COMPARE_VALUE * 0.01);
		#ifdef AC200_DEBUG
			printf("\r\nmotor CCR1 speed set ...\r\n");
		#endif
	}
    else if((OFF == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus) && (ON == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus))
    {
		TIM1->CCR1 = (targetSpeedPercent * MAX_COMPARE_VALUE * 0.01);
		#ifdef AC200_DEBUG
				printf("\r\nmotor CCR1 speed set ...\r\n");
		#endif
	}
    
    
}
///** TIM1 ->CCR1,做输出时间控制*/
/**
 * @description:
 * 1. set max duty cycle is 95%, so max TIM_Pulse(TIM1 ->CCRx) = TIM_Period * 95% = 6840.
 * 2. set increase range is 5%, so increase step length = 6840*5% = 342
 * @brief: increase speed function.
 * @param: target speed, set by max speed percent, range 0% ~ 100%.
 * @return: return ErrorStatus.
 */
ErrorStatus DC_MOTOR_SpeedUp(uint8_t targetSpeedPercent)
{
    uint8_t stepCounter = 0;
	uint16_t loadCounter = ((MAX_COMPARE_VALUE * (uint16_t)targetSpeedPercent) / 100);
	if ((targetSpeedPercent > 0) && (targetSpeedPercent <= 100))
	{
		if (ON == (DC_MOTOR_RunStatus.DC_MOTOR_PowerStatus && \
				   DC_MOTOR_RunStatus.DC_MOTOR_PWM_Status)) // check motor is ready
		{
			if ((ON == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus) && (OFF == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus))
			{
				DC_MOTOR_RunStatus.DC_MOTOR_SpeedupStatus = ON;
				// if loadCounter is bigger than precent CCRx loaded value
                if (DC_MOTOR_ControlParam.dutyCycleParam > MAX_COMPARE_VALUE)
				{
					DC_MOTOR_ControlParam.dutyCycleParam = MAX_COMPARE_VALUE;
				}
				while ((ON == DC_MOTOR_RunStatus.DC_MOTOR_SpeedupStatus) && (DC_MOTOR_ControlParam.dutyCycleParam <= loadCounter) && (DC_MOTOR_ControlParam.dutyCycleParam <= MAX_COMPARE_VALUE))
				{
					DC_MOTOR_ControlParam.dutyCycleParam = DC_MOTOR_ControlParam.dutyCycleParam + DUTY_CYCLE_STEP;
					TIM1 ->CCR1 = DC_MOTOR_ControlParam.dutyCycleParam;
					DelayMs(500 - (stepCounter * 10));
                    stepCounter++;
				}
				DC_MOTOR_RunStatus.DC_MOTOR_SpeedupStatus = OFF;
				errorTemp = SUCCESS;
			}
			else if((ON == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus) && (OFF == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus))
			{
				DC_MOTOR_RunStatus.DC_MOTOR_SpeedupStatus = ON;
				if (DC_MOTOR_ControlParam.dutyCycleParam > MAX_COMPARE_VALUE)
				{
					DC_MOTOR_ControlParam.dutyCycleParam = MAX_COMPARE_VALUE;
				}
				while ((ON == DC_MOTOR_RunStatus.DC_MOTOR_SpeedupStatus) && (DC_MOTOR_ControlParam.dutyCycleParam <= loadCounter) && (DC_MOTOR_ControlParam.dutyCycleParam <= MAX_COMPARE_VALUE))
				{
					DC_MOTOR_ControlParam.dutyCycleParam = DC_MOTOR_ControlParam.dutyCycleParam + DUTY_CYCLE_STEP;
					TIM1 ->CCR2 = DC_MOTOR_ControlParam.dutyCycleParam;
					DelayMs(500 - (stepCounter * 10));
                    stepCounter++;
				}
				DC_MOTOR_RunStatus.DC_MOTOR_SpeedupStatus = OFF;
				errorTemp = SUCCESS;
			}
            
		}
		else
		{
			errorTemp = ERROR;
		}
	}
	else
		errorTemp = ERROR;
    
   return errorTemp;
}
/**
 * @brief: reduce speed function. 
 */
ErrorStatus DC_MOTOR_ReduceSpeed(uint8_t targetSpeedPercent)
{
    uint8_t stepCounter = 0;
	uint16_t loadCounter = ((MAX_COMPARE_VALUE * (uint16_t)targetSpeedPercent) / 100);
	if ((targetSpeedPercent >= 0) && (targetSpeedPercent <= 100))
	{
		if ((DC_MOTOR_ControlParam.dutyCycleParam > 0) && (ON == (DC_MOTOR_RunStatus.DC_MOTOR_PowerStatus &&
																   DC_MOTOR_RunStatus.DC_MOTOR_PWM_Status))) // check motor is ready
		{
			DC_MOTOR_RunStatus.DC_MOTOR_SpeedDownStatus = ON;
			if ((ON == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus) && (OFF == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus))
			{
				while ((DC_MOTOR_ControlParam.dutyCycleParam > loadCounter) && (DC_MOTOR_ControlParam.dutyCycleParam < MAX_COMPARE_VALUE))
					{
						DC_MOTOR_ControlParam.dutyCycleParam = DC_MOTOR_ControlParam.dutyCycleParam - DUTY_CYCLE_STEP;
						TIM1->CCR1 = DC_MOTOR_ControlParam.dutyCycleParam;
						DelayMs(500 - (stepCounter * 15));
						stepCounter++;
					}
				/** 0 < DC_MOTOR_ControlParam.dutyCycleParam < reduce step(432) **/
				DC_MOTOR_RunStatus.DC_MOTOR_SpeedDownStatus = OFF;
				errorTemp = SUCCESS;
			}
			else if((ON == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus) && (OFF == DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus))
			{
				while ((DC_MOTOR_ControlParam.dutyCycleParam > loadCounter) && (DC_MOTOR_ControlParam.dutyCycleParam < MAX_COMPARE_VALUE))
				{
					DC_MOTOR_ControlParam.dutyCycleParam = DC_MOTOR_ControlParam.dutyCycleParam - DUTY_CYCLE_STEP;
					TIM1 ->CCR2 = DC_MOTOR_ControlParam.dutyCycleParam;
					DelayMs(500 - (stepCounter * 15));
                    stepCounter++;
				}
				DC_MOTOR_RunStatus.DC_MOTOR_SpeedDownStatus = OFF;
				errorTemp = SUCCESS;
			}
		}
		else
		{
			errorTemp = ERROR;
		}
	}
	else 
		errorTemp = ERROR;
    
    return errorTemp;
}
ErrorStatus DC_MOTOR_CurrentSpeedKeep(void)
{
	return ERROR;
}

/**
 * @brief: DC_Motor shutdown protocols
 * @param:
 * @return:
 */
void DC_MOTOR_Break(void)
{
	//disable pwm
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	DelayMs(200);
	//break
	DC_MOTOR_1B_High();
	DC_MOTOR_2B_High();
	DelayMs(2000);
}

/**
 * @brief: DC_Motor shutdown protocols
 * @param:
 * @return:
 */
void DC_MOTOR_Stop(void)
{
    DC_MOTOR_ReduceSpeed(5);
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	DelayMs(100);
	//break
	DC_MOTOR_1B_High();
	DC_MOTOR_2B_High();
    DelayMs(200);
	DC_MOTOR_PWM(DISABLE);
    DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus  = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus = OFF;
}

/**
 * @brief: DC_Motor shutdown protocols
 * @param:
 * @return:
 */
ErrorStatus DC_MOTOR_Start(uint8_t motorDirection, uint8_t motorSpeedPecent)
{          
    DC_MOTOR_Init();
	errorTemp = ERROR;
    errorTemp = DC_MOTOR_Power(ENABLE);
	DC_MOTOR_DirectionSet(motorDirection, motorSpeedPecent);
    DC_MOTOR_RunStatus.DC_MOTOR_OutputStatus = ON;
    DC_MOTOR_SpeedUp(motorSpeedPecent);
	DelayMs(100);  
    return errorTemp;
    
}

/**
 * @brief: DC_Motor shutdown protocols
 * @param:
 * @return:
 */
ErrorStatus DC_MOTOR_Shutdown(void)
{
	errorTemp = ERROR;

	DC_MOTOR_Stop();
	errorTemp = DC_MOTOR_Power(DISABLE);

	DC_MOTOR_RunStatus.DC_MOTOR_PowerStatus        = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_PWM_Status         = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_DIR_ForwardStatus  = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_DIR_ReversedStatus = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_OutputStatus       = OFF;
	DC_MOTOR_RunStatus.DC_MOTOR_CurrentProtection  = OFF;

	return errorTemp;
}
/*----------------------------------- END LINE ---------------------------------------*/






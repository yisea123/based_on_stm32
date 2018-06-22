/** 
 * @brief: PWM Wave generate
 * @Description: pwm.c file is a drive for generate PWM signal to drive BRUSHED DC MOTO.
 * @Pin description:
 * ------------------------------------------------------------------------------
 * 				|	TIM1_CH1	|	TIM1_CHN	|	TIM1_CH2	|	TIM1_CH2N	|
 * ------------------------------------------------------------------------------
 * 				|	PA7			|	PA8			|	PA9		   |	PB0		    |
 * ------------------------------------------------------------------------------
 * 				/   
 * ------------------------------------------------------------------------------
 * @param:
 * 		 
*/

#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "command.h"
#include "delay.h"
#include "usart.h"
#include "timmer.h"

/** DC_MOTOR GPIO pin define -------------------------------*/
#ifdef AC200_DEBUG
	#define DC_MOTOR_2A GPIO_Pin_8 // alias: TIM1_CH1,  PA8
	#define DC_MOTOR_1B GPIO_Pin_7 // alias: TIM1_CH1N, PA7
	#define DC_MOTOR_1A GPIO_Pin_9 // alias: TIM1_CH2,  PA9
	#define DC_MOTOR_2B GPIO_Pin_0 // alias: TIM1_CH2N, PB0
#else
	#define DC_MOTOR_1A GPIO_Pin_8 // alias: TIM1_CH1,  PA8
	#define DC_MOTOR_1B GPIO_Pin_7 // alias: TIM1_CH1N, PA7
	#define DC_MOTOR_2A GPIO_Pin_9 // alias: TIM1_CH2,  PA9
	#define DC_MOTOR_2B GPIO_Pin_0 // alias: TIM1_CH2N, PB0
#endif

#define DC_MOTOR_1B_High() 		GPIO_SetBits(PORTA, DC_MOTOR_1B)
#define DC_MOTOR_1B_Low() 		GPIO_ResetBits(PORTA, DC_MOTOR_1B)

#define DC_MOTOR_2B_High() 		GPIO_SetBits(PORTB, DC_MOTOR_2B)
#define DC_MOTOR_2B_Low() 		GPIO_ResetBits(PORTB, DC_MOTOR_2B)

/** DC_MOTOR H-bridge power supply control pin -------------*/
#define DC_MOTOR_POWER_CTR      GPIO_Pin_3 //alias: PB3
/** DC_MOTOR PWM Signal enable pin -------------------------*/
#define DC_MOTOR_PWM_CTR        GPIO_Pin_4 //alias: PB4
/** H-Bridge power supply -----------------------------------*/
#define DC_MOTOR_BOOT_V_EN() 		GPIO_ResetBits(PORTB, DC_MOTOR_POWER_CTR)
#define DC_MOTOR_BOOT_V_DISABLE() 	GPIO_SetBits(PORTB, DC_MOTOR_POWER_CTR)
/** PWM enable set -------------------------------------------*/
#define DC_MOTOR_PWM_EN() 			GPIO_SetBits(PORTB, DC_MOTOR_PWM_CTR)
#define DC_MOTOR_PWM_DISABLE() 		GPIO_ResetBits(PORTB, DC_MOTOR_PWM_CTR)

/** Range define ---------------------------------------------*/
/** 
 * TIM_Period = 7200
 * 1. set max duty cycle is 95%, so max TIM_Pulse(TIM1 ->CCRx) = TIM_Period(7200) * 95% = 6840 = 0x1AB8. 
 * 2. step range is 5% of the maximum , so increase step length = 6840*5% = 342 = 0x156.
 * TIM_Period = 4800
 * 1. max TIM_Pulse(TIM1 ->CCRx) = TIM_Period(6000) * 50% = 3000 = 0x0BB8.
 * 2. step length = 3000*5% = 216 = 0x0096.
 */
#define MAX_COMPARE_VALUE (uint16_t)    0x0BB8
#define DUTY_CYCLE_STEP (uint16_t)      0x0096


/*-----------------------------------------------------------------------------------*/
/** DC_MOTOR_Status enum -------------------------------------*/
typedef enum {
	OFF = 0,
	ON = !OFF
} DC_MOTOR_Status;

/** DC_MOTOR Direction enum -------------------------------------*/
typedef enum {
	Forwared = 0,
	Reversed = !Forwared
} DC_MOTOR_DIR_Enum;

/** typedef to configurate the DC motor PWM function --------*/
typedef struct
{

	uint16_t PWM_CompareMode; // change the register compater polarity when DC moto change direction
	uint16_t PWM_DeadTime;	// Output PWM dead time set parameter
	uint16_t TimerPeriod;	 // Frequency set parameter
	uint16_t Channel1Pulse;   // PWM channel1 duty cycle set parameter
	uint16_t Channel2Pulse;   // PWM channel2 duty cycle set parameter
	uint16_t TimmerCounter;
	uint16_t dutyCycleParam;
} DC_MOTOR_ControlParamTypeDef;

/** typedef to configurate the DC motor status function --------*/
typedef struct
{
	DC_MOTOR_Status 	DC_MOTOR_PowerStatus;
	DC_MOTOR_Status 	DC_MOTOR_PWM_Status;
	DC_MOTOR_Status 	DC_MOTOR_1A_Status;
	DC_MOTOR_Status 	DC_MOTOR_1B_Status;
	DC_MOTOR_Status 	DC_MOTOR_2A_Status;
	DC_MOTOR_Status 	DC_MOTOR_2B_Status;
	DC_MOTOR_Status 	DC_MOTOR_DIR_ForwardStatus;
	DC_MOTOR_Status 	DC_MOTOR_DIR_ReversedStatus;
	DC_MOTOR_Status 	DC_MOTOR_OutputStatus;
	DC_MOTOR_Status 	DC_MOTOR_SpeedupStatus;
	DC_MOTOR_Status 	DC_MOTOR_SpeedDownStatus;
	DC_MOTOR_Status     DC_MOTOR_Standby;
	DC_MOTOR_Status	    DC_MOTOR_CurrentProtection;
	ErrorStatus 		DC_MOTOR_CMD;
} DC_MOTOR_ControlStatusTypeDef;
/** extern define --------------------------------------------*/
extern uint32_t pulseCounter;
extern DC_MOTOR_ControlParamTypeDef DC_MOTOR_ControlParam;
extern DC_MOTOR_ControlStatusTypeDef DC_MOTOR_RunStatus;

/** ---------------------------------------------------------*/

/** function statement --------------------------------------*/
#ifdef AC200_DEBUG
	void DC_MOTOR_CTR_GPIO_Config(void);
	void DC_MOTOR_DirectionSet(uint8_t motorDirection, uint8_t targetSpeedPercent);
	void DC_MOTOR_Init(void);
#else 
	void DC_MOTOR_GPIO_Config(void);
	void DC_MOTOR_Config(void);
#endif

ErrorStatus DC_MOTOR_Ready(FunctionalState NewState);
ErrorStatus DC_MOTOR_Periph(FunctionalState NewState);
// ErrorStatus DC_MOTOR_Param_Set(DC_MOTOR_ControlParamTypeDef *DC_MOTOR_Param);
truthTable DC_MOTOR_Check(void);							                                // motor check befor start turning
ErrorStatus DC_MOTOR_Power(FunctionalState NewState);		                                // motor ready to turning configuration
ErrorStatus DC_MOTOR_PWM(FunctionalState NewState);			                                // PWM start here
ErrorStatus DC_MOTOR_SpeedUp(uint8_t targetSpeedPercent);                                   // normally accelerate to the specified value
ErrorStatus DC_MOTOR_ReduceSpeed(uint8_t targetSpeedPercent);                               // normally reduce speed to zero
ErrorStatus DC_MOTOR_CurrentSpeedKeep(void);                                         // motor rotates direction change
void DC_MOTOR_Break(void);					                                                // motor break protocol function
void DC_MOTOR_Stop(void);																	// motor stop and disable flag, eable pins
ErrorStatus DC_MOTOR_Shutdown(void);
ErrorStatus DC_MOTOR_Start(uint8_t direction, uint8_t speed);
void DC_MOTOR_SpeedFixed(uint8_t targetSpeedPercent);



#endif

/*-------------------------------------- END LINE -----------------------------------------*/

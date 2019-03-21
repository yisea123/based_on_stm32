/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "blink.h"
#include "usart.h"
#include "usr_task.h"
#include "i2c.h"
#include "oled.h"
#include "w5500_driver.h"

CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

DSI_HandleTypeDef hdsi;

LTDC_HandleTypeDef hltdc;

QSPI_HandleTypeDef hqspi;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_tx;

TIM_HandleTypeDef htim1;

SDRAM_HandleTypeDef hsdram1;

/* Private function prototypes -----------------------------------------------*/
void
SystemClock_Config (void);
static void
MX_GPIO_Init (void);
static void
MX_DMA_Init (void);
static void
MX_CRC_Init (void);
static void
MX_DMA2D_Init (void);
static void
MX_DSIHOST_DSI_Init (void);
static void
MX_FMC_Init (void);
static void
MX_LTDC_Init (void);
static void
MX_QUADSPI_Init (void);
static void
MX_SDIO_SD_Init (void);
static void
MX_TIM1_Init (void);

int
main (void)
{

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init ();

	/* Configure the system clock */
	SystemClock_Config ();

	/* USER CODE BEGIN SysInit */
	blink_init ();
	MX_USARTx_Init (USART3, 115200);
	MX_I2C1_Init (400000);
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init ();
	MX_DMA_Init ();
	MX_CRC_Init ();
	MX_DMA2D_Init ();
	MX_DSIHOST_DSI_Init ();
	MX_FMC_Init ();
	MX_LTDC_Init ();
	MX_QUADSPI_Init ();
	MX_SDIO_SD_Init ();
	MX_TIM1_Init ();
	/* init code for FATFS */
	MX_FATFS_Init ();

	oled_power_on ();

	Usr_TaskCreate ();
	/* Start scheduler */
	osKernelStart ();

	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void
SystemClock_Config (void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

	/**Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE()
	;
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 180;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 6;
	RCC_OscInitStruct.PLL.PLLR = 6;
	if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler ();
	}
	/**Activate the Over-Drive mode
	 */
	if (HAL_PWREx_EnableOverDrive () != HAL_OK) {
		Error_Handler ();
	}
	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler ();
	}
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDIO
			| RCC_PERIPHCLK_CLK48 | RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 144;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
	PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV6;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
	PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLSAIP;
	PeriphClkInitStruct.SdioClockSelection = RCC_SDIOCLKSOURCE_CLK48;
	if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct) != HAL_OK) {
		Error_Handler ();
	}
}

/**
 * @brief CRC Initialization Function
 * @param None
 * @retval None
 */
static void
MX_CRC_Init (void)
{
	hcrc.Instance = CRC;
	if (HAL_CRC_Init (&hcrc) != HAL_OK) {
		Error_Handler ();
	}

}

/**
 * @brief DMA2D Initialization Function
 * @param None
 * @retval None
 */
static void
MX_DMA2D_Init (void)
{

	hdma2d.Instance = DMA2D;
	hdma2d.Init.Mode = DMA2D_M2M;
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
	hdma2d.Init.OutputOffset = 0;
	hdma2d.LayerCfg[1].InputOffset = 0;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha = 0;
	if (HAL_DMA2D_Init (&hdma2d) != HAL_OK) {
		Error_Handler ();
	}
	if (HAL_DMA2D_ConfigLayer (&hdma2d, 1) != HAL_OK) {
		Error_Handler ();
	}
}

/**
 * @brief DSIHOST Initialization Function
 * @param None
 * @retval None
 */
static void
MX_DSIHOST_DSI_Init (void)
{
	DSI_PLLInitTypeDef PLLInit = { 0 };
	DSI_HOST_TimeoutTypeDef HostTimeouts = { 0 };
	DSI_PHY_TimerTypeDef PhyTimings = { 0 };
	DSI_LPCmdTypeDef LPCmd = { 0 };
	DSI_CmdCfgTypeDef CmdCfg = { 0 };

	hdsi.Instance = DSI;
	hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
	hdsi.Init.TXEscapeCkdiv = 4;
	hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
	PLLInit.PLLNDIV = 125;
	PLLInit.PLLIDF = DSI_PLL_IN_DIV2;
	PLLInit.PLLODF = DSI_PLL_OUT_DIV1;
	if (HAL_DSI_Init (&hdsi, &PLLInit) != HAL_OK) {
		Error_Handler ();
	}
	HostTimeouts.TimeoutCkdiv = 1;
	HostTimeouts.HighSpeedTransmissionTimeout = 0;
	HostTimeouts.LowPowerReceptionTimeout = 0;
	HostTimeouts.HighSpeedReadTimeout = 0;
	HostTimeouts.LowPowerReadTimeout = 0;
	HostTimeouts.HighSpeedWriteTimeout = 0;
	HostTimeouts.HighSpeedWritePrespMode = DSI_HS_PM_DISABLE;
	HostTimeouts.LowPowerWriteTimeout = 0;
	HostTimeouts.BTATimeout = 0;
	if (HAL_DSI_ConfigHostTimeouts (&hdsi, &HostTimeouts) != HAL_OK) {
		Error_Handler ();
	}
	PhyTimings.ClockLaneHS2LPTime = 28;
	PhyTimings.ClockLaneLP2HSTime = 33;
	PhyTimings.DataLaneHS2LPTime = 15;
	PhyTimings.DataLaneLP2HSTime = 25;
	PhyTimings.DataLaneMaxReadTime = 0;
	PhyTimings.StopWaitTime = 10;
	if (HAL_DSI_ConfigPhyTimer (&hdsi, &PhyTimings) != HAL_OK) {
		Error_Handler ();
	}
	if (HAL_DSI_ConfigFlowControl (&hdsi, DSI_FLOW_CONTROL_BTA) != HAL_OK) {
		Error_Handler ();
	}
	if (HAL_DSI_SetLowPowerRXFilter (&hdsi, 10000) != HAL_OK) {
		Error_Handler ();
	}
	if (HAL_DSI_ConfigErrorMonitor (&hdsi, HAL_DSI_ERROR_NONE) != HAL_OK) {
		Error_Handler ();
	}
	LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_ENABLE;
	LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_ENABLE;
	LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_ENABLE;
	LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_ENABLE;
	LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_ENABLE;
	LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_ENABLE;
	LPCmd.LPGenLongWrite = DSI_LP_GLW_ENABLE;
	LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_ENABLE;
	LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_ENABLE;
	LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_ENABLE;
	LPCmd.LPDcsLongWrite = DSI_LP_DLW_ENABLE;
	LPCmd.LPMaxReadPacket = DSI_LP_MRDP_ENABLE;
	LPCmd.AcknowledgeRequest = DSI_ACKNOWLEDGE_DISABLE;
	if (HAL_DSI_ConfigCommand (&hdsi, &LPCmd) != HAL_OK) {
		Error_Handler ();
	}
	CmdCfg.VirtualChannelID = 0;
	CmdCfg.ColorCoding = DSI_RGB888;
	CmdCfg.CommandSize = 200;
	CmdCfg.TearingEffectSource = DSI_TE_EXTERNAL;
	CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
	CmdCfg.HSPolarity = DSI_HSYNC_ACTIVE_LOW;
	CmdCfg.VSPolarity = DSI_VSYNC_ACTIVE_LOW;
	CmdCfg.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
	CmdCfg.VSyncPol = DSI_VSYNC_FALLING;
	CmdCfg.AutomaticRefresh = DSI_AR_ENABLE;
	CmdCfg.TEAcknowledgeRequest = DSI_TE_ACKNOWLEDGE_ENABLE;
	if (HAL_DSI_ConfigAdaptedCommandMode (&hdsi, &CmdCfg) != HAL_OK) {
		Error_Handler ();
	}
	if (HAL_DSI_SetGenericVCID (&hdsi, 0) != HAL_OK) {
		Error_Handler ();
	}
	/* USER CODE BEGIN DSIHOST_Init 2 */

	/* USER CODE END DSIHOST_Init 2 */

}

/**
 * @brief LTDC Initialization Function
 * @param None
 * @retval None
 */
static void
MX_LTDC_Init (void)
{

	/* USER CODE BEGIN LTDC_Init 0 */

	/* USER CODE END LTDC_Init 0 */

	LTDC_LayerCfgTypeDef pLayerCfg = { 0 };

	/* USER CODE BEGIN LTDC_Init 1 */

	/* USER CODE END LTDC_Init 1 */
	hltdc.Instance = LTDC;
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
	hltdc.Init.HorizontalSync = 0;
	hltdc.Init.VerticalSync = 0;
	hltdc.Init.AccumulatedHBP = 1;
	hltdc.Init.AccumulatedVBP = 1;
	hltdc.Init.AccumulatedActiveW = 201;
	hltdc.Init.AccumulatedActiveH = 481;
	hltdc.Init.TotalWidth = 202;
	hltdc.Init.TotalHeigh = 482;
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 0;
	hltdc.Init.Backcolor.Red = 0;
	if (HAL_LTDC_Init (&hltdc) != HAL_OK) {
		Error_Handler ();
	}
	pLayerCfg.WindowX0 = 0;
	pLayerCfg.WindowX1 = 200;
	pLayerCfg.WindowY0 = 0;
	pLayerCfg.WindowY1 = 480;
	pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
	pLayerCfg.Alpha = 255;
	pLayerCfg.Alpha0 = 0;
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	pLayerCfg.FBStartAdress = 0xC0000000;
	pLayerCfg.ImageWidth = 200;
	pLayerCfg.ImageHeight = 480;
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;
	if (HAL_LTDC_ConfigLayer (&hltdc, &pLayerCfg, 0) != HAL_OK) {
		Error_Handler ();
	}
	/* USER CODE BEGIN LTDC_Init 2 */

	/* USER CODE END LTDC_Init 2 */

}

/**
 * @brief QUADSPI Initialization Function
 * @param None
 * @retval None
 */
static void
MX_QUADSPI_Init (void)
{

	/* USER CODE BEGIN QUADSPI_Init 0 */

	/* USER CODE END QUADSPI_Init 0 */

	/* USER CODE BEGIN QUADSPI_Init 1 */

	/* USER CODE END QUADSPI_Init 1 */
	/* QUADSPI parameter configuration*/
	hqspi.Instance = QUADSPI;
	hqspi.Init.ClockPrescaler = 255;
	hqspi.Init.FifoThreshold = 1;
	hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
	hqspi.Init.FlashSize = 1;
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
	hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
	hqspi.Init.FlashID = QSPI_FLASH_ID_1;
	hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
	if (HAL_QSPI_Init (&hqspi) != HAL_OK) {
		Error_Handler ();
	}
	/* USER CODE BEGIN QUADSPI_Init 2 */

	/* USER CODE END QUADSPI_Init 2 */

}

/**
 * @brief SDIO Initialization Function
 * @param None
 * @retval None
 */
static void
MX_SDIO_SD_Init (void)
{

	/* USER CODE BEGIN SDIO_Init 0 */

	/* USER CODE END SDIO_Init 0 */

	/* USER CODE BEGIN SDIO_Init 1 */

	/* USER CODE END SDIO_Init 1 */
	hsd.Instance = SDIO;
	hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd.Init.ClockDiv = 0;
	/* USER CODE BEGIN SDIO_Init 2 */

	/* USER CODE END SDIO_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void
MX_TIM1_Init (void)
{

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 0;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	if (HAL_TIM_Base_Init (&htim1) != HAL_OK) {
		Error_Handler ();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource (&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler ();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization (&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler ();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void
MX_DMA_Init (void)
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE()
	;

	/* DMA interrupt init */
	/* DMA2_Stream3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority (DMA2_Stream3_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ (DMA2_Stream3_IRQn);

}
/* FMC initialization function */
static void
MX_FMC_Init (void)
{
	FMC_SDRAM_TimingTypeDef SdramTiming;

	/** Perform the SDRAM1 memory initialization sequence
	 */
	hsdram1.Instance = FMC_SDRAM_DEVICE;
	/* hsdram1.Init */
	hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
	hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
	hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
	hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
	hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
	hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
	hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
	hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
	/* SdramTiming */
	SdramTiming.LoadToActiveDelay = 2;
	SdramTiming.ExitSelfRefreshDelay = 7;
	SdramTiming.SelfRefreshTime = 4;
	SdramTiming.RowCycleDelay = 7;
	SdramTiming.WriteRecoveryTime = 3;
	SdramTiming.RPDelay = 2;
	SdramTiming.RCDDelay = 2;

	if (HAL_SDRAM_Init (&hsdram1, &SdramTiming) != HAL_OK) {
		Error_Handler ();
	}

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void
MX_GPIO_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;

	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;

	__HAL_RCC_GPIOI_CLK_ENABLE()
	;
	__HAL_RCC_GPIOF_CLK_ENABLE()
	;

	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOJ_CLK_ENABLE()
	;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin (GPIOE, SPKR_HP_Pin | AUDIO_RST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin (GPIOB, OTG_FS1_PowerSwitchOn_Pin | EXT_RESET_Pin,
						GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin (GPIOH, GPIO_PIN_7, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin (LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : SPKR_HP_Pin AUDIO_RST_Pin */
	GPIO_InitStruct.Pin = SPKR_HP_Pin | AUDIO_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init (GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : OTG_FS1_OverCurrent_Pin */
	GPIO_InitStruct.Pin = OTG_FS1_OverCurrent_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init (OTG_FS1_OverCurrent_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : OTG_FS1_PowerSwitchOn_Pin EXT_RESET_Pin */
	GPIO_InitStruct.Pin = OTG_FS1_PowerSwitchOn_Pin | EXT_RESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init (GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : uSD_Detect_Pin */
	GPIO_InitStruct.Pin = uSD_Detect_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init (uSD_Detect_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LCD_INT_Pin */
	GPIO_InitStruct.Pin = LCD_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init (LCD_INT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PH7 */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init (GPIOH, &GPIO_InitStruct);

	/*Configure GPIO pin : LCD_BL_CTRL_Pin */
	GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init (LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void
HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick ();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void
Error_Handler (void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

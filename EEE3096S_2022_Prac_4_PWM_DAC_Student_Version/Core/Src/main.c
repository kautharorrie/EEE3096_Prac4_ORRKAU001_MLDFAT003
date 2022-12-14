/* USER CODE BEGIN Header */
/**
*******************************************************
Info:		STM32 DMA and PWM with HAL
Author:		Amaan Vally
*******************************************************
In this practical you will to use PWM using DMA on the STM32 using the HAL.
We also set up an interrupt to switch the waveform between various LUTs.

  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//TO DO:
//TASK 2
//Assign values to NS, TIM2CLK and F_SIGNAL
#define TIM2CLK 48000000
#define F_SIGNAL 4000
#define NS 128

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim2_ch1;
int LUT_Mode = 1;
int prev = 0;

/* USER CODE BEGIN PV */

//TO DO:
//TASK 1
//Create global variables for LUTs
uint32_t sin_LUT[NS] = {512,537,562,587,611,636,660,684,707,730,753,774,796,816,836,855,
                        873,890,907,922,937,950,963,974,
                        984,993,1001,1008,1013,1017,1021,1022,
                        1023,1022,1021,1017,1013,1008,1001,993,
                        984,974,963,950,937,922,907,890,
                        873,855,836,816,796,774,753,730,
                        707,684,660,636,611,587,562,537,
                        512,486,461,436,412,387,363,339,
                        316,293,270,249,227,207,187,168,
                        150,133,116,101,86,73,60,49,
                        39,30,22,15,10,6,2,1,
                        0,1,2,6,10,15,22,30,
                        39,49,60,73,86,101,116,133,
                        150,168,187,207,227,249,270,293,
                        316,339,363,387,412,436,461,486};
uint32_t saw_LUT[NS] = { 0,    8,   16,   24,   32,   40,   48,   56,   64,   72,   80,   88,
		   96,  104,  112,  120,  128,  136,  144,  153,  161,  169,  177,  185,
		  193,  201,  209,  217,  225,  233,  241,  249,  257,  265,  273,  281,
		  289,  298,  306,  314,  322,  330,  338,  346,  354,  362,  370,  378,
		  386,  394,  402,  410,  418,  426,  434,  443,  451,  459,  467,  475,
		  483,  491,  499,  507,  515,  523,  531,  539,  547,  555,  563,  571,
		  579,  588,  596,  604,  612,  620,  628,  636,  644,  652,  660,  668,
		  676,  684,  692,  700,  708,  716,  724,  733,  741,  749,  757,  765,
		  773,  781,  789,  797,  805,  813,  821,  829,  837,  845,  853,  861,
		  869,  878,  886,  894,  902,  910,  918,  926,  934,  942,  950,  958,
		  966,  974,  982,  990,  998, 1006, 1014,    0};
uint32_t triangle_LUT[NS] = {16,32,48,64,80,96,112,128,
                             144,160,176,192,208,224,240,256,
                             272,288,304,320,336,352,368,384,
                             400,416,432,448,464,480,496,512,
                             527,543,559,575,591,607,623,639,
                             655,671,687,703,719,735,751,767,
                             783,799,815,831,847,863,879,895,
                             911,927,943,959,975,991,1007,1023,
                             1007,991,975,959,943,927,911,895,
                             879,863,847,831,815,799,783,767,
                             751,735,719,703,687,671,655,639,
                             623,607,591,575,559,543,527,512,
                             496,480,464,448,432,416,400,384,
                             368,352,336,320,304,288,272,256,
                             240,224,208,192,176,160,144,128,
                             112,96,80,64,48,32,16,0};


//TO DO:
//TASK 3
//Calculate TIM2_Ticks
uint32_t TIM2_Ticks = TIM2CLK / (NS * F_SIGNAL);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void EXTI0_1_IRQHandler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  //TO DO:
  //TASK 4
  //Start TIM3 in PWM mode on channel 1
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  //Start TIM2 in Output Compare (OC) mode on channel 1.
  HAL_TIM_OC_Start(&htim2,TIM_CHANNEL_1);
  //Start the DMA in interrupt (IT) mode.
  uint32_t DestAddress = (uint32_t) &(TIM3->CCR1);
  HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t) &sin_LUT,DestAddress,128);
  //Start the DMA transfer
  __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */


	  //No need to do anything in the main loop for this practical


    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIM2_Ticks - 1; //To make the frequency what we want it to be
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1023;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LD4_Pin|LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

}

/* USER CODE BEGIN 4 */
void EXTI0_1_IRQHandler(void)
{
	//TO DO:
	//TASK 5
	//Disable DMA transfer, start DMA in IT mode with new source and re enable transfer
	//Remember to debounce using HAL_GetTick()
    uint32_t tick = HAL_GetTick();
    if((tick-prev)>100){
        prev = tick;
        HAL_DMA_Abort(&hdma_tim2_ch1);
        uint32_t DestAddress = (uint32_t) &(TIM3->CCR1);
        switch(LUT_Mode){
            case 1:
                HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t) &saw_LUT,DestAddress,128);
                __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);
                LUT_Mode=2;
                break;
            case 2:
                HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t) &triangle_LUT,DestAddress,128);
                __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);
                LUT_Mode=3;
                break;
            case 3:
                HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t) &sin_LUT,DestAddress,128);
                __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);
                LUT_Mode=1;
                break;
        }
    }
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); // Clear interrupt flags
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

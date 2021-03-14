/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MY_ILI9341.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int rising_value=0;
int flag_first_rising=0;
int flag_change=0;
uint16_t rising_value1;
uint16_t rising_value2;
uint16_t period;
uint16_t Positon=0;
uint16_t Past_Positon=0;
void drawline(int Pos,uint16_t Color);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	//initilazing the LCD
	ILI9341_Init(&hspi1,LCD_CS_GPIO_Port,LCD_CS_Pin,LCD_DC_GPIO_Port,LCD_DC_Pin,LCD_RST_GPIO_Port,LCD_RST_Pin);
	//top to bottom
	ILI9341_setRotation(3);
	HAL_Delay(7000);
	
	//drawing main circle 
	ILI9341_drawCircle(120,110,90,COLOR_BLACK);
	
	//drawing the degrees
	ILI9341_drawFastVLine(120,20,10,COLOR_BLACK); //for 0
	ILI9341_drawFastVLine(120,190,10,COLOR_BLACK); //for 90 
	
	ILI9341_drawFastHLine(30,110,10,COLOR_BLACK); //for 135
	ILI9341_drawFastHLine(200,110,10,COLOR_BLACK); //for 45
	
	
	ILI9341_drawLine(195,65,190,70,COLOR_BLACK); //for 30
	ILI9341_drawLine(200,150,195,145,COLOR_BLACK); //for 60


	ILI9341_drawLine(45,65,50,70,COLOR_BLACK); //for 150
	ILI9341_drawLine(45,155,50,150,COLOR_BLACK); //for 120
	
	ILI9341_drawLine(165,31,160,36,COLOR_BLACK); //for 15
	ILI9341_drawLine(165,186,160,179,COLOR_BLACK); //for 75
	
	ILI9341_drawLine(73,31,78,36,COLOR_BLACK); //for 165
	ILI9341_drawLine(70,186,75,179,COLOR_BLACK); //for 105
	
	//wrting the degrees numbers
	ILI9341_drawChar(119,10,'0',COLOR_BLACK,COLOR_WHITE,1);//for 0
	
	char *degrees = malloc(256);
	
	strcpy(degrees, "90");
	ILI9341_printText(degrees,110,205,COLOR_BLACK,COLOR_WHITE,1);//for 90
	strcpy(degrees, "135");
	ILI9341_printText(degrees,10,110,COLOR_BLACK,COLOR_WHITE,1);//for 135
	strcpy(degrees, "45");
	ILI9341_printText(degrees,215,110,COLOR_BLACK,COLOR_WHITE,1);//for 45
	
	
	strcpy(degrees, "30");
	ILI9341_printText(degrees,200,60,COLOR_BLACK,COLOR_WHITE,1);//for 30
	strcpy(degrees, "60");
	ILI9341_printText(degrees,210,155,COLOR_BLACK,COLOR_WHITE,1);//for 60
	strcpy(degrees, "150");
	ILI9341_printText(degrees,20,65,COLOR_BLACK,COLOR_WHITE,1);//for 120
	strcpy(degrees, "120");
	ILI9341_printText(degrees,20,155,COLOR_BLACK,COLOR_WHITE,1);//for 150
	
	strcpy(degrees, "15");
	ILI9341_printText(degrees,163,20,COLOR_BLACK,COLOR_WHITE,1);//for 15
	
	strcpy(degrees, "75");
	ILI9341_printText(degrees,163,200,COLOR_BLACK,COLOR_WHITE,1);//for 75
	
	strcpy(degrees, "165");
	ILI9341_printText(degrees,60,20,COLOR_BLACK,COLOR_WHITE,1);//for 165
	
	strcpy(degrees, "105");
	ILI9341_printText(degrees,55,195,COLOR_BLACK,COLOR_WHITE,1);//for 105
	
	strcpy(degrees, "RPM");
	ILI9341_printText(degrees,90,250,COLOR_BLACK,COLOR_WHITE,4);//for RPM

	//starting timer
	HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin|LCD_DC_Pin|LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_RST_Pin LCD_DC_Pin LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_DC_Pin|LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void drawline(int Pos,uint16_t Color)
{
	
	//for differnet Pos we draw differnet line
	if(Pos>=0 && Pos<15)
		{
			ILI9341_drawFastVLine(120,110,50,Color); //0
		}
	if(Pos>=15 && Pos<30)
		{
			ILI9341_drawLine(120,110,140,56,Color); //15
		}
	if (Pos>=30 && Pos<45)
		{
			ILI9341_drawLine(120,110,170,85,Color); //30
		}
	if (Pos>=45 && Pos<60)
		{
			ILI9341_drawFastHLine(120,110,60,Color); //45
		}
	if (Pos>=60 && Pos<75)
		{
			ILI9341_drawLine(120,110,175,135,Color); //60
		}
	if (Pos>=75 && Pos<90)
		{
			ILI9341_drawLine(120,110,150,169,Color); //75
		}

	if (Pos>=90 && Pos<105)
		{
				ILI9341_drawFastVLine(120,110,60,Color); //90
		}
	if (Pos>=105 && Pos<120)
		{
			ILI9341_drawLine(120,110,85,159,Color); //105
		}
	if (Pos>=120 && Pos<135)
		{
			ILI9341_drawLine(120,110,65,140,Color); //120
		}
	if (Pos>=135 && Pos<150)
		{
				ILI9341_drawFastHLine(60,110,60,Color); //135
		}

		if (Pos>=150 && Pos<165)
		{
			ILI9341_drawLine(120,110,65,70,Color); //150
		}
	if (Pos>=165 && Pos<180)
		{
				ILI9341_drawLine(120,110,88,56,Color); //165
		}
		
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim1 &&  htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) //rising edge detected.
	{
		if(flag_first_rising == 0) //first rising edge
		{
			rising_value1 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			flag_first_rising = 1;
		}
		else
		{
			rising_value2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
			flag_first_rising = 0;
			
			if(rising_value2 > rising_value1) //now both edges has came.
			{
				period = rising_value2 - rising_value1;
				char charray[200];
				Positon=1000000/period;				//Calculating frequency
				Positon=Positon*60/24; //Calculating RPM
				/*
				sprintf(charray, "%d", Positon);
				ILI9341_printText(charray,110,120,COLOR_BLACK,COLOR_WHITE,1);//90
				*/
				if(abs(Positon-Past_Positon)>7)
				{
					drawline(Past_Positon,COLOR_WHITE);//earsing the last rpm speed
					drawline(Positon,COLOR_RED);//drawing the new rpm speed
					Past_Positon=Positon;
				}
			else
			{
				period = (0xffff - rising_value1) + rising_value2;
				char charray[200];
				Positon=1000000/period; //Calculating frequency
				Positon=Positon*60/24; //Calculating RPM
				if(abs(Positon-Past_Positon)>7)
				{
					drawline(Past_Positon,COLOR_WHITE);//earsing the last rpm speed
					drawline(Positon,COLOR_RED);//drawing the new rpm speed
					Past_Positon=Positon;
				}
			}
		}
		
	}
}
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

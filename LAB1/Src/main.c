/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
	//                     7     8      9    A     4      5    6  	 B      1    2     3     C     D     0     E    F 
	uint8_t _7_seg[16] = {0x07, 0x7F, 0x6F, 0x77, 0x66, 0x6D, 0x7D, 0x7C, 0x06, 0x5B, 0x4F, 0x39, 0x5E, 0x3F, 0x79,0x71};
	void set_segment(uint8_t input);
	int flag_interupt=0; //when this flag set to high, one key is pressed and it's time to scan keypad.
	int keypad_scan();
	void shift(int i); //this function is used to move one zero between rows of keypad.
	
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  /* USER CODE BEGIN 2 */
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(flag_interupt == 1) //if interupt has came.
		{
			set_segment(_7_seg[keypad_scan()]); //scan keyboard and change seven segment pins.
			flag_interupt = 0; //to get out of if.
		}
		
	}
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void set_segment(uint8_t input)
{
	GPIOA -> ODR = input;  //with this register we can set value to pins of GPIOA.(output data register)
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	flag_interupt = 1; // interupt has come.
}

void shift(int i)
{	
	unsigned char pins[4] = {0x07,0xB,0xD,0xE};
	
	HAL_GPIO_WritePin(row1_GPIO_Port,row1_Pin,(GPIO_PinState) (pins[i] & 0x8)); //if output of (pins[i] & 0x8) is anything excpet 0x00 it will cast to GPIO_PIN_SET
	HAL_GPIO_WritePin(row2_GPIO_Port,row2_Pin,(GPIO_PinState) (pins[i] & 0x4));
	HAL_GPIO_WritePin(row3_GPIO_Port,row3_Pin,(GPIO_PinState) (pins[i] & 0x2));
	HAL_GPIO_WritePin(row4_GPIO_Port,row4_Pin,(GPIO_PinState) (pins[i] & 0x1));
}


int keypad_scan()
{
	int position; //position in keys array.
	
	for (int row = 0; row<4; row++)
	{	
		shift(row); //shifting zero in rows.
		int col = -1; 
		if(HAL_GPIO_ReadPin(col1_GPIO_Port,col1_Pin) == GPIO_PIN_RESET) //reading which column is pressed.
			col = 0;
		if(HAL_GPIO_ReadPin(col2_GPIO_Port,col2_Pin) == GPIO_PIN_RESET)
			col = 1;
		if(HAL_GPIO_ReadPin(col3_GPIO_Port,col3_Pin) == GPIO_PIN_RESET)
			col = 2;
		if(HAL_GPIO_ReadPin(col4_GPIO_Port,col4_Pin) == GPIO_PIN_RESET)
			col = 3;
		if(col != -1) //some key is pressed.
		{
			
			position = 4 * row + col; //finding index of pressed key in array with variables, row and col. (according to the _7_seg[16] array)
			col = -1; 
			HAL_GPIO_WritePin(row1_GPIO_Port,row1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(row2_GPIO_Port,row2_Pin,GPIO_PIN_RESET); //reseting pins to initial values so when some key is pressed,we have falling edge interupt again.
			HAL_GPIO_WritePin(row3_GPIO_Port,row3_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(row4_GPIO_Port,row4_Pin,GPIO_PIN_RESET);
			return position;
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

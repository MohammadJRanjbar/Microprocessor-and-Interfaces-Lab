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
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lm016.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t adc_data[2]; //buffer for adc using DMA.
int temperature;
int resistance; //LDR Resistance.
char lcd_buffer[3];
unsigned char myRxData[20]; //buffer used for UART transmission.
char command[20]; //received command from UArt.
int index_string = 0;
void Compare_Strigns(char* command); //To Compare command received from user and known commands for system.
unsigned char* message[5][2] = {{"Manual\r","Auto\r"},{"Off\r","On\r"},{"Mode Set to Auto\r","Mode Set to Manual\r"} , {"Heater Turned Off\r","Heater Turned On\r"} , {"Lamp Turned Off\r","Lamp Turned On\r"}  } ;
int Mode = 1;  //0 for manual , 1 for auto
int status_heater = 0; // 0 for off 1 for on
int status_lamp = 0; // 0 for off 1 for on
#define LDR_Curve_Sample_size 10 
int res_vs_illumination[LDR_Curve_Sample_size][2]=
{{500,900},{1000,800},{2000,700},{5000,600},{10000,500},
{20000,400},{50000,300},{100000,200},{200000,100},{1000000,0}};
float Lux=0;
int R2 = 1000;


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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_Clear();	
	LCD_PutString("T = ");
	LCD_SetCursor(1,9);
	LCD_PutString("DEG");
	LCD_SetCursor(2,1);
	LCD_PutString("I = ");
	LCD_SetCursor(2,9);
	LCD_PutString("LUX");
	HAL_UART_Receive_IT(&huart1, myRxData, 1); //start receiving one characater because commands are not the same length.
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_ADC_Start_DMA(&hadc1,adc_data,2); //Start ADC in DMA mode to convert two channels and give interupt when conversions are completed.
		HAL_Delay(100);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	temperature = adc_data[0] * 3300 / 4095 / 10; //because of characteristics of LM35 sensor, for having temperature, we have to divide calculated voltage by 10.
	LCD_SetCursor(1,5);
	sprintf(lcd_buffer,"%03d",temperature);
	LCD_PutString(lcd_buffer);
	
	//CALCULATE LUX AND SHOW ON LCD.
	resistance = R2 / ((float) adc_data[1]/4095) - R2;
	int location = 0 ;
	for(int i=0;i<10;i++)
	{
		if(abs(res_vs_illumination[i][0]-resistance< abs(res_vs_illumination[location][0]-resistance))) //compare calculated resistance with ideal ones and find minimum error from ideal and find its Lux according to Table.
		{
			location = i;
			
		}
	}
	Lux=res_vs_illumination[location][1];//Select Lux according the resistor calculated.
	LCD_SetCursor(2,5);
	sprintf(lcd_buffer,"%03d",(int)Lux);
	LCD_PutString(lcd_buffer);
	//CHECK for commands
	if(Mode ==  1) //Auto Mode
	{
		if(temperature<20)
		{
			status_heater=1; //heater on.
			HAL_GPIO_WritePin(HEATER_GPIO_Port,HEATER_Pin,GPIO_PIN_SET);
		}
		if(temperature>24)
		{
			status_heater=0; //heater off.
			HAL_GPIO_WritePin(HEATER_GPIO_Port,HEATER_Pin,GPIO_PIN_RESET);
		}
		
		if(Lux<400)
		{
			HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET); //lamp on.
			status_lamp=1;
		}
		if(Lux>500) 
		{
			HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET); //lamp off.
			status_lamp=0;
		}

	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//We will write the command in our buffer until we reach end symbole(Enter character).
	if(myRxData[0]!=13 ) //if received char is not Enter.
	{ 
		command[index_string]= myRxData[0]; //saving received character to command buffer.
		index_string++;
	}
	else{
		//this will help us to use string's functions
		command[index_string] = '\0';
		index_string=0;
		Compare_Strigns(command); //now we can compare received data and commands of system.
	}
	HAL_UART_Receive_IT(&huart1, myRxData, 1);
	
}

void Compare_Strigns(char* command)
{
	if(strcmp("AT_GetMode" , command) ==  0)
	{
		if(Mode == 0) //manual mode.
			HAL_UART_Transmit_IT(&huart1,message[0][0],7);
		else
			HAL_UART_Transmit_IT(&huart1,message[0][1],5);
	}
	
	else if(strcmp("AT_GetStats_Heater" , command) ==  0)
	{
		if(status_heater == 0) //off
			HAL_UART_Transmit_IT(&huart1,message[1][0],4);
		else
			HAL_UART_Transmit_IT(&huart1,message[1][1],3);
	}
	
	else if(strcmp("AT_GetStats_Lamp" , command) ==  0)
	{
		if(status_lamp == 0) //off
			HAL_UART_Transmit_IT(&huart1,message[1][0],4);
		else
			HAL_UART_Transmit_IT(&huart1,message[1][1],3);
	}
	
	else if(strcmp("AT_SetMode_Auto" , command) ==  0)
	{
		Mode = 1; //auto mode.
		HAL_UART_Transmit_IT(&huart1,message[2][0],17);
	}
	
	else if(strcmp("AT_SetMode_Manual" , command) ==  0)
	{
		Mode = 0; //Manual mode.
		
		HAL_UART_Transmit_IT(&huart1,message[2][1],19);
	}
	else if(strcmp("AT_TurnOff_Heater" , command) ==  0 && Mode == 0) //we should be at manual mode to change state of heater and lamp.
	{
		status_heater = 0;
		HAL_GPIO_WritePin(HEATER_GPIO_Port,HEATER_Pin,GPIO_PIN_RESET);
		HAL_UART_Transmit_IT(&huart1,message[3][0],18);
	}
	else if(strcmp("AT_TurnOn_Heater" , command) ==  0 && Mode == 0)
	{
		status_heater = 1;
		HAL_GPIO_WritePin(HEATER_GPIO_Port,HEATER_Pin,GPIO_PIN_SET);
		HAL_UART_Transmit_IT(&huart1,message[3][1],17);
	}
	
	else if(strcmp("AT_TurnOff_Lamp" , command) ==  0 && Mode == 0)
	{
		status_lamp = 0;
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
		HAL_UART_Transmit_IT(&huart1,message[4][0],16);
	}
	else if(strcmp("AT_TurnOn_Lamp" , command) ==  0 && Mode == 0)
	{
		status_lamp = 1;
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
		HAL_UART_Transmit_IT(&huart1,message[4][1],15);
	}
	else if(strcmp("AT_GetTemperature" , command) ==  0)
	{
		char buff[10];
		sprintf(buff,"%d DEG\r",temperature);
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)buff,strlen(buff));
	}
	else if (strcmp("AT_GetIllumination" , command) ==  0)
	{
		char buff[10];
		sprintf(buff,"%d LUX\r",(int)Lux);
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)buff,strlen(buff));
	}
	else
	{
			HAL_UART_Transmit_IT(&huart1,"Wrong Command\r",16);
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

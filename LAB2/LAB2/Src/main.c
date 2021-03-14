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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lm016.h"
#include <stdio.h>
#include <math.h>
unsigned char keys[16] = {'7', '8', '9', 'A', '4', '5', '6', 'B', '1', '2', '3', 'C', '.', '0', 'E','F'};
volatile int flag_interupt=0; //when this flag set to high, one key is pressed and it's time to scan keypad.
int keypad_scan();
void shift(int i); //this function is used to move one zero between rows of keypad.
void stepper(int direction);
void steper_degree(float teta); 
int Steps=0; //this variable is used to know what step we should take in moving stepper motor.
void setMotor(int A_State,int B_State,int C_State,int D_State); //setting motor pins.
int steps_to_move; //this mean number of steps we have to take. if its negative we should go anticlockwise, else clockwise.
int menu1_state = 0; //this means that we are at menu1
void menu1(void); //used for printing degree of motor on lcd
int dot_pressed = 0;
float number_pressed = 0; //number that user has pressed on keypad.
int direction;
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
int curren_position = 0;

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_Clear();
	menu1();
	//steper_degree(100);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
			if(menu1_state == 1 && flag_interupt == 1 ) //if we are at menu1 and keypad is pressed
			{
				if(keypad_scan() == 14) //if "=" is pressed
				{
					menu1_state = 0; //we go to menu2
					LCD_Clear(); 
					LCD_PutString("TYPE ANGLE > = ");
					LCD_SetCursor(2,1);
				}
				flag_interupt = 0; //to trigger if once until next interupt has come.
			}
			if(menu1_state == 0 && flag_interupt==1) //if we are at menu2 and key is pressed
			{
				int index_pressed = keypad_scan();
				if(index_pressed == 14) //if "=" is pressed
				{
					steper_degree(number_pressed); //setting degree that we want to move there.
					menu1_state = 1; //go to menu1
					number_pressed = 0; //reseting number_pressed so that when setting new value,it starts at zero not last number.
				}
				flag_interupt = 0; //to trigger if once until next interupt has come.
				if(menu1_state == 0) //if we are at menu2
				{
					LCD_PutChar(keys[index_pressed]);
				if(index_pressed == 12)
					dot_pressed = 1;
				if(dot_pressed == 1 && index_pressed != 12)
				{
					int x = keys[index_pressed] - '0';
					float y = x/10.0;
					number_pressed = number_pressed + y;
					dot_pressed = 0;
				}
				else if(index_pressed != 12)
				{
					number_pressed = 10 * number_pressed + (keys[index_pressed] - '0');
				}
				
			}
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	flag_interupt = 1; //this means that some key is pressed.
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
			HAL_Delay(10);
	}		
}

void steper_degree(float teta)
{
	//based on teta finds how many steps the motor should take
	float teta_to_move = teta - curren_position*0.9;   //current position is number of steps that we have gone. so if we multiply by 0.9, it will change to degree.
	
	if(teta_to_move > 180) //this if is for when we want to move least steps.
		teta_to_move = teta_to_move - 360;
	else if(-1 * teta_to_move > 180)
		teta_to_move = 360 + teta_to_move;
	
	float a = round(teta_to_move / 0.9); //changing from degree to number of steps we need to take.
	steps_to_move = (int) a; //cast to integer
	
	if(steps_to_move < 0) //anti clockwise
	{
		direction = 1;
		steps_to_move = -1 * steps_to_move;
	}
	else //clockwise
		direction = 0;
		
	
	HAL_TIM_Base_Start_IT(&htim1); // starting timer of motor
	HAL_TIM_Base_Start_IT(&htim2); // starting timer of lcd.
	menu1_state = 1; //go to menu1.
}




void setMotor(int A_State,int B_State,int C_State,int D_State)
{
	HAL_GPIO_WritePin(Stepper_M_A_GPIO_Port,Stepper_M_A_Pin,(GPIO_PinState) A_State);
	HAL_GPIO_WritePin(Stepper_M_B_GPIO_Port,Stepper_M_B_Pin,(GPIO_PinState) B_State);
	HAL_GPIO_WritePin(Stepper_M_C_GPIO_Port,Stepper_M_C_Pin,(GPIO_PinState) C_State);
	HAL_GPIO_WritePin(Stepper_M_D_GPIO_Port,Stepper_M_D_Pin,(GPIO_PinState) D_State);
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
	if(direction ==  1) //we should move anti_clockwise
	{
		if(steps_to_move > 0) //while we have steps to take.
		{
			stepper(1); //ANTI_clockwise
			curren_position -= 1;
			steps_to_move --;
		}			
	}
	else //clockwise
	{
			if(steps_to_move > 0 )
			{
				stepper(0); //clockwise
				curren_position += 1;
				steps_to_move --;
			}	
	}
}
	else if(menu1_state == 1 && htim->Instance == TIM2) //if we want to show menu1 and interupt lcd has come.
		menu1();
}	

void stepper(int direction)
{
	
		if(direction==0)//clockwise
			Steps++;
		else
			Steps--;
		
		if(Steps > 8)
			Steps = 1;
		
		if(Steps == 0)
			Steps = 8;
		
		if(Steps == -1) //when we are at 0 degree, going to step 8 wont change motor. so we go to case 7 to take one step anticlockwise.
			Steps = 7;
		
		switch(Steps)
		{
			case 1 : setMotor(1,0,0,0); break;
			case 2 : setMotor(1,1,0,0); break;
			case 3 : setMotor(0,1,0,0); break;
			case 4 : setMotor(0,1,1,0); break;
			case 5 : setMotor(0,0,1,0); break;
			case 6 : setMotor(0,0,1,1); break;
			case 7 : setMotor(0,0,0,1); break;
			case 8 : setMotor(1,0,0,1); break;
			
		}
		
}

void menu1()
{
	menu1_state = 1;
	char str[16];
	float t = curren_position * 0.9;
	if(t < 0)
		t = t + 360;
	int teta = t * 10;
	int x = teta % 10 ;
	teta = teta/10;
	int y = teta%10;
	teta = teta/10;
	int z = teta%10;
	teta = teta/10;
	int e = teta%10;
	
	uint8_t degree = 0xDF;
	LCD_SetCursor(1,1);
	sprintf(str,"ANGLE:  %d%d%d.%d",e,z,y,x);
	LCD_PutString(str);
	lcd_data(degree);
	LCD_SetCursor(2,1);
	LCD_PutString("= >CHANGE ANGLE");
	
	
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

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define A_Seg_Pin GPIO_PIN_0
#define A_Seg_GPIO_Port GPIOA
#define B_Seg_Pin GPIO_PIN_1
#define B_Seg_GPIO_Port GPIOA
#define C_Seg_Pin GPIO_PIN_2
#define C_Seg_GPIO_Port GPIOA
#define D_Seg_Pin GPIO_PIN_3
#define D_Seg_GPIO_Port GPIOA
#define E_Seg_Pin GPIO_PIN_4
#define E_Seg_GPIO_Port GPIOA
#define F_Seg_Pin GPIO_PIN_5
#define F_Seg_GPIO_Port GPIOA
#define G_Seg_Pin GPIO_PIN_6
#define G_Seg_GPIO_Port GPIOA
#define Dp_Seg_Pin GPIO_PIN_7
#define Dp_Seg_GPIO_Port GPIOA
#define row1_Pin GPIO_PIN_0
#define row1_GPIO_Port GPIOB
#define row2_Pin GPIO_PIN_1
#define row2_GPIO_Port GPIOB
#define row3_Pin GPIO_PIN_2
#define row3_GPIO_Port GPIOB
#define col1_Pin GPIO_PIN_10
#define col1_GPIO_Port GPIOB
#define col1_EXTI_IRQn EXTI15_10_IRQn
#define col2_Pin GPIO_PIN_11
#define col2_GPIO_Port GPIOB
#define col2_EXTI_IRQn EXTI15_10_IRQn
#define col3_Pin GPIO_PIN_12
#define col3_GPIO_Port GPIOB
#define col3_EXTI_IRQn EXTI15_10_IRQn
#define col4_Pin GPIO_PIN_13
#define col4_GPIO_Port GPIOB
#define col4_EXTI_IRQn EXTI15_10_IRQn
#define row4_Pin GPIO_PIN_3
#define row4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

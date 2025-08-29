/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#define segA_Pin GPIO_PIN_0
#define segA_GPIO_Port GPIOA
#define segB_Pin GPIO_PIN_1
#define segB_GPIO_Port GPIOA
#define segC_Pin GPIO_PIN_2
#define segC_GPIO_Port GPIOA
#define segD_Pin GPIO_PIN_3
#define segD_GPIO_Port GPIOA
#define segE_Pin GPIO_PIN_4
#define segE_GPIO_Port GPIOA
#define segF_Pin GPIO_PIN_5
#define segF_GPIO_Port GPIOA
#define segG_Pin GPIO_PIN_6
#define segG_GPIO_Port GPIOA
#define RED_x_Pin GPIO_PIN_7
#define RED_x_GPIO_Port GPIOA
#define dclk0_Pin GPIO_PIN_0
#define dclk0_GPIO_Port GPIOB
#define dclk1_Pin GPIO_PIN_1
#define dclk1_GPIO_Port GPIOB
#define dclk2_Pin GPIO_PIN_2
#define dclk2_GPIO_Port GPIOB
#define dclk10_Pin GPIO_PIN_10
#define dclk10_GPIO_Port GPIOB
#define dclk11_Pin GPIO_PIN_11
#define dclk11_GPIO_Port GPIOB
#define YELLOW_x_Pin GPIO_PIN_8
#define YELLOW_x_GPIO_Port GPIOA
#define GREEN_x_Pin GPIO_PIN_9
#define GREEN_x_GPIO_Port GPIOA
#define RED_y_Pin GPIO_PIN_10
#define RED_y_GPIO_Port GPIOA
#define YELLOW_y_Pin GPIO_PIN_11
#define YELLOW_y_GPIO_Port GPIOA
#define GREEN_y_Pin GPIO_PIN_12
#define GREEN_y_GPIO_Port GPIOA
#define dclk3_Pin GPIO_PIN_3
#define dclk3_GPIO_Port GPIOB
#define dclk4_Pin GPIO_PIN_4
#define dclk4_GPIO_Port GPIOB
#define dclk5_Pin GPIO_PIN_5
#define dclk5_GPIO_Port GPIOB
#define dclk6_Pin GPIO_PIN_6
#define dclk6_GPIO_Port GPIOB
#define dclk7_Pin GPIO_PIN_7
#define dclk7_GPIO_Port GPIOB
#define dclk8_Pin GPIO_PIN_8
#define dclk8_GPIO_Port GPIOB
#define dclk9_Pin GPIO_PIN_9
#define dclk9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

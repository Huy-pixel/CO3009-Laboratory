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
#define outp0_Pin GPIO_PIN_0
#define outp0_GPIO_Port GPIOA
#define outp1_Pin GPIO_PIN_1
#define outp1_GPIO_Port GPIOA
#define outp2_Pin GPIO_PIN_2
#define outp2_GPIO_Port GPIOA
#define outp3_Pin GPIO_PIN_3
#define outp3_GPIO_Port GPIOA
#define outp4_Pin GPIO_PIN_4
#define outp4_GPIO_Port GPIOA
#define outp5_Pin GPIO_PIN_5
#define outp5_GPIO_Port GPIOA
#define outp6_Pin GPIO_PIN_6
#define outp6_GPIO_Port GPIOA
#define outp7_Pin GPIO_PIN_7
#define outp7_GPIO_Port GPIOA
#define button_Pin GPIO_PIN_0
#define button_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

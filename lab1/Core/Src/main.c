/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	uint8_t output;
	uint32_t delay;
	uint8_t next_state;
}state;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define S0 0U
#define S1 1U
#define S2 2U
#define S3 3U

//Define LED value for bitmask usage
#define xRED 	((uint8_t) 1 << 0)
#define xYELLOW ((uint8_t) 1 << 1)
#define xGREEN 	((uint8_t) 1 << 2)
#define yRED 	((uint8_t) 1 << 3)
#define yYELLOW ((uint8_t) 1 << 4)
#define yGREEN 	((uint8_t) 1 << 5)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
const state traffic_controller[4] =
{		//output	  Delay	Input
		{xRED | yGREEN,	3, S1},
		{xRED | yYELLOW,2, S2},
		{yRED | xGREEN,	3, S3},
		{yRED | xYELLOW,2, S0}
};

const uint8_t seg_code[10] =
{
		0x3f, // 0
		0x06, // 1
		0x5B, // 2
		0x4F, // 3
		0x66, // 4
		0x6D, // 5
		0x7D, // 6
		0x07, // 7
		0x7F, // 8
		0x6F, // 9
};

GPIO_TypeDef* port[7] =
{
		segA_GPIO_Port,
		segB_GPIO_Port,
		segC_GPIO_Port,
		segD_GPIO_Port,
		segE_GPIO_Port,
		segF_GPIO_Port,
		segG_GPIO_Port
};

uint16_t pin[7] =
{
		segA_Pin,
		segB_Pin,
		segC_Pin,
		segD_Pin,
		segE_Pin,
		segF_Pin,
		segG_Pin
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void init7SEG();
void Light_control(uint8_t bitmask);
uint8_t Two_way_traffic(state output);
void display7SEG(uint8_t num);
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
  init7SEG();
  state input = traffic_controller[0];
  int counter = 4;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (counter < 0) counter = 4;
	  display7SEG(counter--);
	  counter--;
	  uint8_t output = Two_way_traffic(input);
	  HAL_Delay(1000);
	  if (counter == 2)
		  input = traffic_controller[output];
	  else if (counter == 0)
		  input = traffic_controller[output];
	  else;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, segA_Pin|segB_Pin|segC_Pin|segD_Pin
                          |segE_Pin|segF_Pin|segG_Pin|RED_x_Pin
                          |YELLOW_x_Pin|GREEN_x_Pin|RED_y_Pin|YELLOW_y_Pin
                          |GREEN_y_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : segA_Pin segB_Pin segC_Pin segD_Pin
                           segE_Pin segF_Pin segG_Pin RED_x_Pin
                           YELLOW_x_Pin GREEN_x_Pin RED_y_Pin YELLOW_y_Pin
                           GREEN_y_Pin */
  GPIO_InitStruct.Pin = segA_Pin|segB_Pin|segC_Pin|segD_Pin
                          |segE_Pin|segF_Pin|segG_Pin|RED_x_Pin
                          |YELLOW_x_Pin|GREEN_x_Pin|RED_y_Pin|YELLOW_y_Pin
                          |GREEN_y_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void init7SEG()
{
	for (uint8_t i = 0; i<7; i++)
		HAL_GPIO_WritePin(port[i], pin[i], GPIO_PIN_SET);
}

void Light_control(uint8_t bitmask)
{
	HAL_GPIO_WritePin(RED_x_GPIO_Port, RED_x_Pin, (bitmask & xRED) ? 1:0);
	HAL_GPIO_WritePin(YELLOW_x_GPIO_Port, YELLOW_x_Pin, (bitmask & xYELLOW) ? 1:0);
	HAL_GPIO_WritePin(GREEN_x_GPIO_Port, GREEN_x_Pin, (bitmask & xGREEN) ? 1:0);

	HAL_GPIO_WritePin(RED_y_GPIO_Port, RED_y_Pin, (bitmask & yRED) ? 1:0);
	HAL_GPIO_WritePin(YELLOW_y_GPIO_Port, YELLOW_y_Pin, (bitmask & yYELLOW) ? 1:0);
	HAL_GPIO_WritePin(GREEN_y_GPIO_Port, GREEN_y_Pin, (bitmask & yGREEN) ? 1:0);
}

uint8_t Two_way_traffic(state transition)
{
	Light_control(transition.output);
	return transition.next_state;
}

void display7SEG(uint8_t num)
{
	num = num % 10;
	uint8_t index = seg_code[num];
	for (uint8_t i = 0; i<7; i++)
		HAL_GPIO_WritePin(port[i], pin[i], ((index & (1 << i)) ? GPIO_PIN_RESET : GPIO_PIN_SET));
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
#ifdef USE_FULL_ASSERT
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

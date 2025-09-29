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

/* !!! UNCOMMENT OUT 1 line that need for testing, others need to be commented out !!!*/

//#define ex1 1		//---Exercise 1 execution---//
//#define ex2 1		//---Exercise 2 execution---//
//#define ex3 1		//---Exercise 3 execution---//
//#define ex4 1		//---Exercise 4 execution---//
#define ex5 1		//---Exercise 5 execution---//
//#define ex6 1		//---Exercise 6 execution---//
//#define dclk 1	//---Exercise 10 (7,8,9 integrated) execution---//

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "traffic_light.h"
#include "led7seg.h"
#include "digital_clk.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define t_RED 		9U	/*RED light interval */
#define t_GREEN		8U	/* GREEN light */
#define t_YELLOW	3U	/* YELLOW light */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define init_state				0U
#define xRED_yGREEN_state		xRED 	| yGREEN
#define xRED_yYELLOW_state		xRED 	| yYELLOW
#define xGREEN_yRED_state		xGREEN 	| yRED
#define xYELLOW_yRED_state		xYELLOW | yRED
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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
#if defined(ex1) || defined(ex2) || defined(ex3) || defined(ex4) || defined(ex5)
  uint8_t amber_green_countdown;
  uint8_t red_countdown;
#if !defined(ex1) && !defined(ex4)
  uint8_t state = init_state;
#endif
#endif /* defined ex1 || ex2 || ex3 || ex4 || ex5 */

#ifdef dclk
  uint8_t hour = 11;
  uint8_t min = 17;
  uint8_t sec = 30;
  clearAllClock(); //Initialize digital clock state
#endif /* dclk */

  while (1)
  {
#ifdef ex1
	  HAL_GPIO_WritePin(YELLOW_x_GPIO_Port, YELLOW_x_Pin, counter >> 1);
	  HAL_GPIO_WritePin(RED_x_GPIO_Port, RED_x_Pin, !(counter >> 1));

	  counter = (counter + 1) % 4;

#elif ex2
	  mask = 0;
	  if (counter < 5)
		  mask = xRED;
	  else if (counter < 8)
		  mask = xGREEN;
	  else
		  mask = xYELLOW;

	  Light_Control(mask);

	  counter = (counter + 1) % 10;

#elif ex3
	  mask = 0;
	  if (counter < 3)
		  mask = xRED | yGREEN;
	  else if (counter < 5)
		  mask = xRED | yYELLOW;
	  else if (counter < 8)
		  mask = yRED | xGREEN;
	  else
		  mask = yRED | xYELLOW;

	  Light_Control(mask);

	  counter = (counter + 1) % 10;

#elif ex4
	  if (counter >= 10) counter = 0;
	  display7SEG(counter++, 0);

#elif ex5
	  switch(state)
	  {
	  case (init_state):
			  amber_green_countdown = t_GREEN;
	  	  	  red_countdown = t_RED;
	  	  	  state = xRED_yGREEN_state;
			  continue;
	  case (xRED_yGREEN_state):
			  if (amber_green_countdown <= 0)
			  {
				  amber_green_countdown = t_YELLOW;
				  state = xRED_yYELLOW_state;
			  }
			  break;
	  case (xRED_yYELLOW_state):
			  if (amber_green_countdown <= 0)
			  {
				  amber_green_countdown = t_GREEN;
				  red_countdown = t_RED;
				  state = xGREEN_yRED_state;
			  }
			  break;
	  case (xGREEN_yRED_state):
			  if (amber_green_countdown <= 0)
			  {
				  amber_green_countdown = t_YELLOW;
				  state = xYELLOW_yRED_state;
			  }
			  break;
	  case (xYELLOW_yRED_state):
			  if (amber_green_countdown <= 0)
			  {
				  amber_green_countdown = t_GREEN;
				  red_countdown = t_RED;
				  state = xRED_yGREEN_state;
			  }
			  break;
	  default:
		  break;
	  }
	  Light_Control(state);
	  display7SEG(amber_green_countdown, (state & xRED) ? 1 : 0);
	  display7SEG(red_countdown, (state & xRED) ? 0 : 1);
	  amber_green_countdown--;
	  red_countdown--;
#elif ex6
	  testClock(counter++);

#elif dclk
	  setNumberOnClock(hour);
	  setNumberOnClock(min/5);
	  setNumberOnClock(sec/5);
#endif

	  HAL_Delay(1000);

#ifdef dclk
	  clearNumberOnClock(hour);
	  clearNumberOnClock(min/5);
	  clearNumberOnClock(sec/5);

	  sec++;
	  if (sec >= 60)
	  {
		  min++;
		  sec = 0;
	  }
	  if (min >= 60)
	  {
		  hour++;
		  min = 0;
	  }
	  if (hour >= 12)
		  hour = 0;

#endif /* dclk */
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, segA_Pin|segB_Pin|segC_Pin|segD_Pin
                          |segE_Pin|segF_Pin|segG_Pin|RED_x_Pin
                          |YELLOW_x_Pin|GREEN_x_Pin|RED_y_Pin|YELLOW_y_Pin
                          |GREEN_y_Pin|segA__Pin|segB__Pin|segC__Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, dclk0_Pin|dclk1_Pin|dclk2_Pin|dclk10_Pin
                          |dclk11_Pin|segD__Pin|segE__Pin|segF__Pin
                          |segG__Pin|dclk3_Pin|dclk4_Pin|dclk5_Pin
                          |dclk6_Pin|dclk7_Pin|dclk8_Pin|dclk9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : segA_Pin segB_Pin segC_Pin segD_Pin
                           segE_Pin segF_Pin segG_Pin RED_x_Pin
                           YELLOW_x_Pin GREEN_x_Pin RED_y_Pin YELLOW_y_Pin
                           GREEN_y_Pin segA__Pin segB__Pin segC__Pin */
  GPIO_InitStruct.Pin = segA_Pin|segB_Pin|segC_Pin|segD_Pin
                          |segE_Pin|segF_Pin|segG_Pin|RED_x_Pin
                          |YELLOW_x_Pin|GREEN_x_Pin|RED_y_Pin|YELLOW_y_Pin
                          |GREEN_y_Pin|segA__Pin|segB__Pin|segC__Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : dclk0_Pin dclk1_Pin dclk2_Pin dclk10_Pin
                           dclk11_Pin segD__Pin segE__Pin segF__Pin
                           segG__Pin dclk3_Pin dclk4_Pin dclk5_Pin
                           dclk6_Pin dclk7_Pin dclk8_Pin dclk9_Pin */
  GPIO_InitStruct.Pin = dclk0_Pin|dclk1_Pin|dclk2_Pin|dclk10_Pin
                          |dclk11_Pin|segD__Pin|segE__Pin|segF__Pin
                          |segG__Pin|dclk3_Pin|dclk4_Pin|dclk5_Pin
                          |dclk6_Pin|dclk7_Pin|dclk8_Pin|dclk9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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

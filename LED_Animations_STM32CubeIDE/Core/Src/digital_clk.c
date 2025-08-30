/*
 * @file:	digital_clk.c
 * @brief:	12_LEDs-based digital clock source file
 * 			contains private source code that controls clock
 *  Created on: Aug 29, 2025
 *      Author: soaic
 */

/* Private includes ----------------------------------------------------------*/
#include "digital_clk.h"
/* Private variables ----------------------------------------------------------*/
static GPIO_TypeDef* port[12] =
{
		dclk0_GPIO_Port,
		dclk1_GPIO_Port,
		dclk2_GPIO_Port,
		dclk3_GPIO_Port,
		dclk4_GPIO_Port,
		dclk5_GPIO_Port,
		dclk6_GPIO_Port,
		dclk7_GPIO_Port,
		dclk8_GPIO_Port,
		dclk9_GPIO_Port,
		dclk10_GPIO_Port,
		dclk11_GPIO_Port
};

static uint16_t pin[12] =
{
		dclk0_Pin,
		dclk1_Pin,
		dclk2_Pin,
		dclk3_Pin,
		dclk4_Pin,
		dclk5_Pin,
		dclk6_Pin,
		dclk7_Pin,
		dclk8_Pin,
		dclk9_Pin,
		dclk10_Pin,
		dclk11_Pin
};
/* Private code --------------------------------------------------------------*/

/**
 * @brief	A test function, turn on 12 LEDs in sequence
 * @param	LED counter
 * @retval	None
 */
void testClock(uint8_t counter)
{
	uint8_t num = counter % 12;
	if (num == 0)
			clearAllClock();
	for (uint8_t i = 0; i <= num; i++)
		HAL_GPIO_WritePin(port[i], pin[i], 1);
}

/**
 * @brief	Turn off all LEDs of digital clock
 * @param	None
 * @retval	None
 */
void clearAllClock()
{
	for (uint8_t i = 0; i < 12; i++)
			HAL_GPIO_WritePin(port[i], pin[i], 0);
}

/**
 * @brief	set high a LED on clock
 * @param	LED's position on clock
 * @retval	None
 */
void setNumberOnClock(uint8_t num)
{
	HAL_GPIO_WritePin(port[num], pin[num], 1);
}

/**
 * @brief	clear a LED
 * @param	LED's position
 * @retval	None
 */
void clearNumberOnClock(uint8_t num)
{
	HAL_GPIO_WritePin(port[num], pin[num], 0);
}



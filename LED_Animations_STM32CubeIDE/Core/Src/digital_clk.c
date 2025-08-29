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
GPIO_TypeDef* clock_port[12] =
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

uint16_t clock_pin[12] =
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
void testClock(uint8_t counter)
{
	uint8_t num = counter % 12;
	if (num == 0)
			clearAllClock();
	for (uint8_t i = 0; i <= num; i++)
		HAL_GPIO_WritePin(clock_port[i], clock_pin[i], 1);
}

void clearAllClock()
{
	for (uint8_t i = 0; i < 12; i++)
			HAL_GPIO_WritePin(clock_port[i], clock_pin[i], 0);
}

void setNumberOnClock(uint8_t num)
{
	HAL_GPIO_WritePin(clock_port[num], clock_pin[num], 1);
}

void clearNumberOnClock(uint8_t num)
{
	HAL_GPIO_WritePin(clock_port[num], clock_pin[num], 0);
}



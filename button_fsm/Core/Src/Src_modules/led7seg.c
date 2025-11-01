/*
 * @file:	led7seg.c
 * @brief:	7-segment LED source file
 * 			contains private source code that programs 7-segment LED
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */
/* Private includes ----------------------------------------------------------*/
#include "stdint.h"
#include "main.h"

#include "Inc_modules/led7seg.h"

/* Private variables ----------------------------------------------------------*/

/**
 * @Note	An array for looking up bit-mask used to display 7-segments LED
 */
static const uint8_t seg_code[11] =
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
		0x79  // E: error, exceeding 9
};

static GPIO_TypeDef* port[7] =
{
		SEG0_PORT,
		SEG1_PORT,
		SEG2_PORT,
		SEG3_PORT,
		SEG4_PORT,
		SEG5_PORT,
		SEG6_PORT
};

static uint16_t pin[7] =
{
		SEG0_PIN,
		SEG1_PIN,
		SEG2_PIN,
		SEG3_PIN,
		SEG4_PIN,
		SEG5_PIN,
		SEG6_PIN
};

/* Private code --------------------------------------------------------------*/

/**
 * @brief	Initialize pins that controls 7-segment LED
 * @param	None
 * @retval	None
 */
void Led7seg_Init(void)
{
	for (uint8_t i = 0; i < 7; i++)
		HAL_GPIO_WritePin(port[i], pin[i], LED7SEG_RESET);
}

/**
 * @brief	Display a single 7-segment LED
 * @param	A number to display
 * @retval 	None
 */
void display_single_7SEG(uint8_t num)
{
	uint8_t index = seg_code[(num > 9) ? 10 : num];
	for (uint8_t i = 0; i < 7; i++)
	{
		HAL_GPIO_WritePin(port[i], pin[i], ((index & (1 << i)) ? LED7SEG_SET : LED7SEG_RESET));
	}
}

/**
 * @brief	Display a pair of 7-segment LED, using scanning method, scan frequency depends on TIMER_CYCLE
 * @param	A number to display, references to ENABLE pin, 2 ENABLE pin requires
 * @retval 	None
 */
void display_pair_7SEG(uint8_t num, GPIO_TypeDef* en0_port, uint16_t en0_pin, GPIO_TypeDef* en1_port, uint16_t en1_pin)
{
	static uint8_t intr = 0;
	static uint8_t state = 0;

	if (!intr)
	{
		intr = setTimer(0, 10);
	}

	uint8_t flag = get_flag();
	if (flag == intr)
	{
		clear_flag();
		state = !state;
		switch(state)
		{
		case 0:
			HAL_GPIO_WritePin(en0_port, en0_pin, EN_SET);
			HAL_GPIO_WritePin(en1_port, en1_pin, EN_RST);
			display_single_7SEG(num/10);
		break;
		case 1:
			HAL_GPIO_WritePin(en0_port, en0_pin, EN_RST);
			HAL_GPIO_WritePin(en1_port, en1_pin, EN_SET);
			display_single_7SEG(num%10);
		break;
		}
	}
}

void display_2pairs_7SEG();

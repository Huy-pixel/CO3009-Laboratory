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

#include "led7seg.h"

/* Private variables ----------------------------------------------------------*/

/**
 * @Note	An array for looking up bit-mask used to display 7-segments LED
 */
static const uint8_t seg_code[10] =
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

/**
 * @Note	A lookup table ensures number not exceed limits
 */
static const uint8_t dmap[16] =
{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,	// 0-9
		0, 1, 2, 3, 4, 5				// 10-15
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
void display7SEG(uint8_t num)
{
	num = dmap[num & 0x0F];	//get 4-last bit in order to not exceeding [0;9]
	uint8_t index = seg_code[num];
	for (uint8_t i = 0; i < 7; i++)
		HAL_GPIO_WritePin(port[i], pin[i], ((index & (1 << i)) ? LED7SEG_SET : LED7SEG_RESET));
}

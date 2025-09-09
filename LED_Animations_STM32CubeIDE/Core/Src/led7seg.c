/*
 * @file:	led7seg.c
 * @brief:	7-segment LED source file
 * 			contains private source code that programs 7-segment LED
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */
/* Private includes ----------------------------------------------------------*/
#include "led7seg.h"

/* Private variables ----------------------------------------------------------*/

/**
 * An array for looking up bit-mask used to display 7-segments LED
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
		segA_GPIO_Port,
		segB_GPIO_Port,
		segC_GPIO_Port,
		segD_GPIO_Port,
		segE_GPIO_Port,
		segF_GPIO_Port,
		segG_GPIO_Port
};

static uint16_t pin[7] =
{
		segA_Pin,
		segB_Pin,
		segC_Pin,
		segD_Pin,
		segE_Pin,
		segF_Pin,
		segG_Pin
};

/**
 * A lookup table preserving number not exceed limits
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
void Led7seg_Init()
{
	for (uint8_t i = 0; i<7; i++)
		HAL_GPIO_WritePin(port[i], pin[i], GPIO_PIN_SET);
}

/**
 * @brief	Display 7-segment LED at a time
 * @param	A number to display
 * @retval 	None
 */
void display7SEG(uint8_t num)
{
	num = dmap[num & 0x0F];	//get 4-last bit, make sure not exceed 0-9
	uint8_t index = seg_code[num];
	for (uint8_t i = 0; i<7; i++)
		HAL_GPIO_WritePin(port[i], pin[i], ((index & (1 << i)) ? GPIO_PIN_RESET : GPIO_PIN_SET));
}

/*
 * led7seg.c
 *
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */
/* Private includes ----------------------------------------------------------*/
#include "led7seg.h"

/* Private variables ----------------------------------------------------------*/
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
/* Private code --------------------------------------------------------------*/
void Led7seg_Init()
{
	for (uint8_t i = 0; i<7; i++)
		HAL_GPIO_WritePin(port[i], pin[i], GPIO_PIN_SET);
}

void display7SEG(uint8_t num)
{
	num = num % 10;
	uint8_t index = seg_code[num];
	for (uint8_t i = 0; i<7; i++)
		HAL_GPIO_WritePin(port[i], pin[i], ((index & (1 << i)) ? GPIO_PIN_RESET : GPIO_PIN_SET));
}

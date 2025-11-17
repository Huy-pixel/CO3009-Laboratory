/*
 * @file:	led7seg.c
 * @brief:	7-segment LED source file
 * 			contains private source code that programs 7-segment LED
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */
/* Private includes ----------------------------------------------------------*/

/* Header file ---------------------------------------------------------------*/
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
		0x00  // error, exceeding 9
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

static GPIO_TypeDef* en_port[4] =
{
		EN0_PORT,
		EN1_PORT,
		EN2_PORT,
		EN3_PORT
};

static uint16_t en_pin[4] =
{
		EN0_PIN,
		EN1_PIN,
		EN2_PIN,
		EN3_PIN
};

static volatile uint8_t led_buf[MAX_LED];
/* Private code --------------------------------------------------------------*/

/**
 * @brief	Display a single 7-segment LED
 * @param	A number to display
 * @retval 	None
 */
static inline void display_single_7SEG(uint8_t num)
{
	uint8_t index = seg_code[(num > 9) ? 10 : num];
	for (uint8_t i = 0; i < 7; i++)
	{
		HAL_GPIO_WritePin(port[i], pin[i], ((index & (1 << i)) ? LED7SEG_SET : LED7SEG_RESET));
	}
}

static inline void set_enable_pin(uint8_t index)
{
	switch(index)
	{
	case 0:
		HAL_GPIO_WritePin(en_port[0], en_pin[0], EN_SET);
		HAL_GPIO_WritePin(en_port[1], en_pin[1], EN_RST);
		HAL_GPIO_WritePin(en_port[2], en_pin[2], EN_RST);
		HAL_GPIO_WritePin(en_port[3], en_pin[3], EN_RST);
	break;
	case 1:
		HAL_GPIO_WritePin(en_port[0], en_pin[0], EN_RST);
		HAL_GPIO_WritePin(en_port[1], en_pin[1], EN_SET);
		HAL_GPIO_WritePin(en_port[2], en_pin[2], EN_RST);
		HAL_GPIO_WritePin(en_port[3], en_pin[3], EN_RST);
	break;
	case 2:
		HAL_GPIO_WritePin(en_port[0], en_pin[0], EN_RST);
		HAL_GPIO_WritePin(en_port[1], en_pin[1], EN_RST);
		HAL_GPIO_WritePin(en_port[2], en_pin[2], EN_SET);
		HAL_GPIO_WritePin(en_port[3], en_pin[3], EN_RST);
	break;
	case 3:
		HAL_GPIO_WritePin(en_port[0], en_pin[0], EN_RST);
		HAL_GPIO_WritePin(en_port[1], en_pin[1], EN_RST);
		HAL_GPIO_WritePin(en_port[2], en_pin[2], EN_RST);
		HAL_GPIO_WritePin(en_port[3], en_pin[3], EN_SET);
	break;
	}
}
/* API -----------------------------------------------------------------------*/
void update_7SEG_buffer(uint8_t num1, uint8_t num2)
{
	if (num1 > 99)
	{
		led_buf[0] = 10;
		led_buf[1] = 10;
	}
	else
	{
		led_buf[0] = num1/10;
		led_buf[1] = num1%10;
	}
	if (num2 > 99)
	{
		led_buf[2] = 10;
		led_buf[3] = 10;
	}
	else
	{
		led_buf[2] = num2/10;
		led_buf[3] = num2%10;
	}
}


void Led7SEG_display(uint8_t index)
{
	uint8_t num = led_buf[index];
	set_enable_pin(index);
	display_single_7SEG(num);
}

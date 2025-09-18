/*
 * @file:	led_matrix.c
 * @brief:	8x8 led matrix source file
 *  Created on: Sep 10, 2025
 *      Author: huy
 */

/* Private includes ----------------------------------------------------------*/
#include "led_matrix.h"

/* Private defines -----------------------------------------------------------*/
#define matrix_row 8
#define matrix_col 8

/* Private variables ---------------------------------------------------------*/
static GPIO_TypeDef* column_port[8] =
{
		ENM0_GPIO_Port,
		ENM1_GPIO_Port,
		ENM2_GPIO_Port,
		ENM3_GPIO_Port,
		ENM4_GPIO_Port,
		ENM5_GPIO_Port,
		ENM6_GPIO_Port,
		ENM7_GPIO_Port
};

static GPIO_TypeDef* row_port[8] =
{
		ROW0_GPIO_Port,
		ROW1_GPIO_Port,
		ROW2_GPIO_Port,
		ROW3_GPIO_Port,
		ROW4_GPIO_Port,
		ROW5_GPIO_Port,
		ROW6_GPIO_Port,
		ROW7_GPIO_Port
};

static uint16_t column_pin[8] =
{
		ENM0_Pin,
		ENM1_Pin,
		ENM2_Pin,
		ENM3_Pin,
		ENM4_Pin,
		ENM5_Pin,
		ENM6_Pin,
		ENM7_Pin
};

static uint16_t row_pin[8] =
{
		ROW0_Pin,
		ROW1_Pin,
		ROW2_Pin,
		ROW3_Pin,
		ROW4_Pin,
		ROW5_Pin,
		ROW6_Pin,
		ROW7_Pin
};

static uint32_t word_32_name[matrix_row] =
{
		0b01100110011001100110011000000000,
		0b01100110011001100110011000000000,
		0b01100110011001100110011000000000,
		0b01111110011001100011110000000000,
		0b01111110011001100001100000000000,
		0b01100110011001100001100000000000,
		0b01100110011001100001100000000000,
		0b01100110001111000001100000000000
};

uint8_t frame[matrix_col];
static uint8_t bitmask;
/* Private implementation ----------------------------------------------------*/

/**
 * @brief	clear LED matrix by pull high all cathode pin
 * @param	None
 * @retval	None
 */
static void clearLEDMatrix()
{
	for (uint8_t i = 0; i < 8; i++)
		HAL_GPIO_WritePin(row_port[i], row_pin[i], 1);
}

/**
 * @brief	update buffer, this buffer specified LED state in one row of matrix
 * @param	row index
 * @retval	None
 */
static void update_buffer(uint8_t row)
{
#ifdef name_display
	bitmask = (uint8_t)word_32_name[row] >> 24;
#else
	bitmask = frame[row];
#endif
}

/**
 * @brief	circular shift a 8-bit word left
 * @param	num: a 8-bit-long word
 * @retval	a left-shifted 8-bit word
 */
static uint8_t circular_shift_left(uint8_t num, uint8_t isLeft)
{
	if (isLeft)
	{
		uint8_t msb = num >> 7;
		return (num << 1) | msb;
	}
	else
	{
		uint8_t lsb = num & 1;
		return (num >> 1) | (lsb << 7);
	}
}

/**
 * @brief	circular shift a 32-bit word left
 * @param	num: a 32-bit-long word
 * @retval	a left-shifted 32-bit word
 */
static uint32_t circular_shift_left_32(uint32_t num)
{
	uint8_t msb = num >> 31;
	return (num << 1) | msb;
}

/* Implementation ------------------------------------------------------------*/

void init_frame(uint8_t* ref)
{
	for (uint8_t i=0; i < matrix_col; i++)
		frame[i] = ref[i];
}

/**
 * @brief	display 1 row of led matrix
 * @param	row index
 * reval	None
 */
void updateLEDMatrix(uint8_t index)
{
	clearLEDMatrix(); // all LEDs on led-matrix turn off

	switch(index)
	{
	case 0:
		update_buffer(0); // get a string to display in row 1

		HAL_GPIO_WritePin(row_port[0], row_pin[0], 0);

		for (uint8_t i = 0; i < 8; i++)
			HAL_GPIO_WritePin(column_port[i], column_pin[i], (bitmask & (1 << i)) ? 0 : 1);
		break;
	case 1:
		update_buffer(1); // get a string to display in row 2

		HAL_GPIO_WritePin(row_port[1], row_pin[1], 0);

		for (uint8_t i = 0; i < 8; i++)
			HAL_GPIO_WritePin(column_port[i], column_pin[i], (bitmask & (1 << i)) ? 0 : 1);
		break;
	case 2:
		update_buffer(2); // get a string to display in row 3

		HAL_GPIO_WritePin(row_port[2], row_pin[2], 0);

		for (uint8_t i = 0; i < 8; i++)
			HAL_GPIO_WritePin(column_port[i], column_pin[i], (bitmask & (1 << i)) ? 0 : 1);
		break;
	case 3:
		update_buffer(3); // get a string to display in row 4

		HAL_GPIO_WritePin(row_port[3], row_pin[3], 0);

		for (uint8_t i = 0; i < 8; i++)
			HAL_GPIO_WritePin(column_port[i], column_pin[i], (bitmask & (1 << i)) ? 0 : 1);
		break;
	case 4:
		update_buffer(4); // get a string to display in row 5

		HAL_GPIO_WritePin(row_port[4], row_pin[4], 0);

		for (uint8_t i = 0; i < 8; i++)
			HAL_GPIO_WritePin(column_port[i], column_pin[i], (bitmask & (1 << i)) ? 0 : 1);
		break;
	case 5:
		update_buffer(5); // get a string to display in row 6

		HAL_GPIO_WritePin(row_port[5], row_pin[5], 0);

		for (uint8_t i = 0; i < 8; i++)
			HAL_GPIO_WritePin(column_port[i], column_pin[i], (bitmask & (1 << i)) ? 0 : 1);
		break;
	case 6:
		update_buffer(6); // get a string to display in row 7

		HAL_GPIO_WritePin(row_port[6], row_pin[6], 0);

		for (uint8_t i = 0; i < 8; i++)
			HAL_GPIO_WritePin(column_port[i], column_pin[i], (bitmask & (1 << i)) ? 0 : 1);
		break;
	case 7:
		update_buffer(7); // get a string to display in row 8

		HAL_GPIO_WritePin(row_port[7], row_pin[7], 0);

		for (uint8_t i = 0; i < 8; i++)
			HAL_GPIO_WritePin(column_port[i], column_pin[i], (bitmask & (1 << i)) ? 0 : 1);
		break;
	}
}

/**
 * @brief	shift left character A which is 8-bit long on 8x8 led matrix
 * @param	None
 * @retval	None
 */
void shift_left(uint8_t isLeft)
{
	if (isLeft)
	{
		for(uint8_t i = 0; i < 8; i++)
				frame[i] = circular_shift_left(frame[i], isLeft);
	}
	else
	{
		for(uint8_t i = 0; i < 8; i++)
				frame[i] = circular_shift_left(frame[i], isLeft);
	}
}

void shift_up(uint8_t isUp)
{
	if (isUp)
	{
		uint8_t mask = frame[0];
		for (uint8_t i = 0; i < 7; i++)
			frame[i] = frame[i+1];
		frame[7] = mask;
	}
	else
	{
		uint8_t mask = frame[7];
		for (uint8_t i = 7; i > 0; i--)
			frame[i] = frame[i-1];
		frame[0] = mask;
	}
}

/**
 * @brief	shift left a 32-bit long on 8x8 led matrix
 * @param	None
 * @retval	None
 */
void shift_left_32()
{
	for(uint8_t i=0; i < 8; i++)
	{
#ifdef display_name
		word_32_name[i] = circular_shift_left_32(word_32_name[i]);
#endif

	}
}


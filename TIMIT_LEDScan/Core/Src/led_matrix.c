/*
 * @file:	led_matrix.c
 * @brief:	8x8 led matrix source file
 *  Created on: Sep 10, 2025
 *      Author: huy
 */

/* Private includes ----------------------------------------------------------*/
#include "led_matrix.h"

/* Private defines -----------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static GPIO_TypeDef* column_port_array[8] =
{
		COL0_port,
		COL1_port,
		COL2_port,
		COL3_port,
		COL4_port,
		COL5_port,
		COL6_port,
		COL7_port
};

static GPIO_TypeDef* row_port_array[8] =
{
		ROW0_port,
		ROW1_port,
		ROW2_port,
		ROW3_port,
		ROW4_port,
		ROW5_port,
		ROW6_port,
		ROW7_port
};

static uint16_t column_pin_array[8] =
{
		COL0_pin,
		COL1_pin,
		COL2_pin,
		COL3_pin,
		COL4_pin,
		COL5_pin,
		COL6_pin,
		COL7_pin
};

static uint16_t row_pin_array[8] =
{
		ROW0_pin,
		ROW1_pin,
		ROW2_pin,
		ROW3_pin,
		ROW4_pin,
		ROW5_pin,
		ROW6_pin,
		ROW7_pin
};

#ifdef name_display
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
#endif

uint8_t frame[matrix_row];
static uint8_t buffer;
/* Private implementation ----------------------------------------------------*/

/**
 * @brief	clear LED matrix by pull high all cathode pin
 * @param	None
 * @retval	None
 * @note	In order to clear all LED, deactive all row pin
 */
static void clearLEDMatrix()
{
	for (uint8_t i = 0; i < matrix_row; i++)
		HAL_GPIO_WritePin(row_port_array[i], row_pin_array[i], row_reset);
}

/**
 * @brief 	Display 1 one row of LED_Matrix, used for LED scanning
 * @param 	row index
 * @retval	None
 */
static void display_row(uint8_t row)
{
	HAL_GPIO_WritePin(row_port_array[row], row_pin_array[row], row_set); /* Active desire row */
	for (int8_t i = matrix_col - 1; i >= 0; i--)
		HAL_GPIO_WritePin(column_port_array[i], column_pin_array[i], (buffer & (1 << i)) ? column_set : column_reset);
}

/**
 * @brief	update buffer, this buffer specified LED state in one row of matrix
 * @param	row index
 * @retval	None
 */
static void update_buffer(uint8_t row)
{
#ifdef name_display
	buffer = (uint8_t)word_32_name[row] >> 24;
#else
	buffer = frame[row];
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
		uint8_t msb = num >> (matrix_col - 1);
		return (num << 1) | msb;
	}
	else
	{
		uint8_t lsb = num & 1;
		return (num >> 1) | (lsb << (matrix_col - 1));
	}
}

#ifdef name_display
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
#endif

/* Implementation ------------------------------------------------------------*/

void init_frame(uint8_t* ref)
{
	for (uint8_t i=0; i < matrix_row; i++)
		frame[i] = ref[i];
}

/**
 * @brief	display 1 row of led matrix
 * @param	row index
 * reval	None
 */
void updateLEDMatrix(uint8_t row)
{
	clearLEDMatrix(); // all LEDs on led-matrix turn off

	update_buffer(row);
	display_row(row);
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
		for(uint8_t i = 0; i < matrix_row; i++)
				frame[i] = circular_shift_left(frame[i], isLeft);
	}
	else
	{
		for(uint8_t i = 0; i < matrix_row; i++)
				frame[i] = circular_shift_left(frame[i], isLeft);
	}
}

void shift_up(uint8_t isUp)
{
	if (isUp)
	{
		uint8_t first = frame[0];
		for (uint8_t i = 0; i < matrix_row - 1; i++)
			frame[i] = frame[i+1];
		frame[matrix_row - 1] = first;
	}
	else
	{
		uint8_t last = frame[matrix_row - 1];
		for (uint8_t i = matrix_row - 1; i > 0; i--)
			frame[i] = frame[i-1];
		frame[0] = last;
	}
}

//void animation_machine(void)
//{
//	swich
//}

#ifdef display_name
/**
 * @brief	shift left a 32-bit long on 8x8 led matrix
 * @param	None
 * @retval	None
 */
void shift_left_32()
{
	for(uint8_t i=0; i < 8; i++)
	{

		word_32_name[i] = circular_shift_left_32(word_32_name[i]);
	}
}
#endif

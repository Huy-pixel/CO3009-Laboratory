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
		COL0_PORT,
		COL1_PORT,
		COL2_PORT,
		COL3_PORT,
		COL4_PORT,
		COL5_PORT,
		COL6_PORT,
		COL7_PORT
};

static GPIO_TypeDef* row_port_array[8] =
{
		ROW0_PORT,
		ROW1_PORT,
		ROW2_PORT,
		ROW3_PORT,
		ROW4_PORT,
		ROW5_PORT,
		ROW6_PORT,
		ROW7_PORT
};

static uint16_t column_pin_array[8] =
{
		COL0_PIN,
		COL1_PIN,
		COL2_PIN,
		COL3_PIN,
		COL4_PIN,
		COL5_PIN,
		COL6_PIN,
		COL7_PIN
};

static uint16_t row_pin_array[8] =
{
		ROW0_PIN,
		ROW1_PIN,
		ROW2_PIN,
		ROW3_PIN,
		ROW4_PIN,
		ROW5_PIN,
		ROW6_PIN,
		ROW7_PIN
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

uint8_t frame[LED_MATRIX_ROW];
static uint8_t g_buf;
/* Private implementation ----------------------------------------------------*/

/**
 * @brief	clear LED matrix by pull high all cathode pin
 * @param	None
 * @retval	None
 * @note	In order to clear all LED, deactive all row pin
 */
static void clearLEDMatrix()
{
	for (uint8_t i = 0; i < LED_MATRIX_ROW; i++)
		HAL_GPIO_WritePin(row_port_array[i], row_pin_array[i], LED_MATRIX_ROWRESET);
}

/**
 * @brief 	Display 1 one row of LED_Matrix, used for LED scanning
 * @param 	row index
 * @retval	None
 */
static void display_row(uint8_t row)
{
	HAL_GPIO_WritePin(row_port_array[row], row_pin_array[row], LED_MATRIX_ROWSET); /* Active desire row */
	for (int8_t i = LED_MATRIX_COL - 1; i >= 0; i--)
		HAL_GPIO_WritePin(column_port_array[i], column_pin_array[i], (g_buf & (1 << i)) ? LED_MATRIX_COLSET : LED_MATRIX_COLRESET);
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
	g_buf = frame[row];
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
		uint8_t msb = num >> (LED_MATRIX_COL - 1);
		return (num << 1) | msb;
	}
	else
	{
		uint8_t lsb = num & 1;
		return (num >> 1) | (lsb << (LED_MATRIX_COL - 1));
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
	for (uint8_t i=0; i < LED_MATRIX_ROW; i++)
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
		for(uint8_t i = 0; i < LED_MATRIX_ROW; i++)
				frame[i] = circular_shift_left(frame[i], isLeft);
	}
	else
	{
		for(uint8_t i = 0; i < LED_MATRIX_ROW; i++)
				frame[i] = circular_shift_left(frame[i], isLeft);
	}
}

void shift_up(uint8_t isUp)
{
	if (isUp)
	{
		uint8_t first = frame[0];
		for (uint8_t i = 0; i < LED_MATRIX_ROW - 1; i++)
			frame[i] = frame[i+1];
		frame[LED_MATRIX_ROW - 1] = first;
	}
	else
	{
		uint8_t last = frame[LED_MATRIX_ROW - 1];
		for (uint8_t i = LED_MATRIX_ROW - 1; i > 0; i--)
			frame[i] = frame[i-1];
		frame[0] = last;
	}
}

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

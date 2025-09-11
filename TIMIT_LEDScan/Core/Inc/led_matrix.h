/*
 * @file:	led_matrix.h
 * @brief:	8x8 led matrix header file
 *  Created on: Sep 10, 2025
 *      Author: soaic
 */

#ifndef INC_LED_MATRIX_H_
#define INC_LED_MATRIX_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "main.h"

/* Function prototypes -------------------------------------------------------*/
void updateLEDMatrix(uint8_t index);
void shift_left();

#endif /* INC_LED_MATRIX_H_ */

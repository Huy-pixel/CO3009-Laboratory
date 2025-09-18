/*
 * @file:	led_matrix.h
 * @brief:	8x8 led matrix header file
 *  Created on: Sep 10, 2025
 *      Author: huy
 */

#ifndef INC_LED_MATRIX_H_
#define INC_LED_MATRIX_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "main.h"

/* Defines -------------------------------------------------------------------*/
//#define name_display

/* Exported Variables --------------------------------------------------------*/
extern uint8_t frame[];

/* Function prototypes -------------------------------------------------------*/
void init_frame(uint8_t* ref);
void updateLEDMatrix(uint8_t index);
void shift_left(uint8_t isLeft);
void shift_left_32();
void shift_up(uint8_t isUp);

#endif /* INC_LED_MATRIX_H_ */

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

/* Defines -------------------------------------------------------------------
 * User of this module must read thoroughly and set these things up carefully-*/

/*!!! MUST set this correctly represent to actual hardware */
#define matrix_row 5
#define matrix_col 7
/* GPIO_TypeDef* <> user label column pins used for control LED matrix */
#define COL0_port COL0_GPIO_Port
#define COL1_port COL1_GPIO_Port
#define COL2_port COL2_GPIO_Port
#define COL3_port COL3_GPIO_Port
#define COL4_port COL4_GPIO_Port
#define COL5_port COL5_GPIO_Port	/* If not use, set this to 0 */
#define COL6_port COL6_GPIO_Port	/* If not use, set this to 0 */
#define COL7_port COL7_GPIO_Port	/* If not use, set this to 0 */
/* GPIO_TypeDef* <> user label row pins used for control LED matrix*/
#define ROW0_port ROW0_GPIO_Port
#define ROW1_port ROW1_GPIO_Port
#define ROW2_port ROW2_GPIO_Port
#define ROW3_port ROW3_GPIO_Port
#define ROW4_port ROW4_GPIO_Port
#define ROW5_port ROW5_GPIO_Port
#define ROW6_port ROW6_GPIO_Port
#define ROW7_port ROW7_GPIO_Port	/* If not use, set this to 0 */
/* uint16_t <> user label column pins used for control LED matrix */
#define COL0_pin COL0_Pin
#define COL1_pin COL1_Pin
#define COL2_pin COL2_Pin
#define COL3_pin COL3_Pin
#define COL4_pin COL4_Pin
#define COL5_pin COL5_Pin 			/* If not use, set this to 0 */
#define COL6_pin COL6_Pin			/* If not use, set this to 0 */
#define COL7_pin COL7_Pin			/* If not use, set this to 0 */
/* uint16_t <> user label column pins used for control LED matrix */
#define ROW0_pin ROW0_Pin
#define ROW1_pin ROW1_Pin
#define ROW2_pin ROW2_Pin
#define ROW3_pin ROW3_Pin
#define ROW4_pin ROW4_Pin
#define ROW5_pin ROW5_Pin
#define ROW6_pin ROW6_Pin
#define ROW7_pin ROW7_Pin			/* If not use, set this to 0 */
/* Refer to hardware design on LED Matrix for compatible active/deactive state*/
#define row_set			1U
#define row_reset		0U
#define column_set		1U
#define column_reset	0U

//#define name_display
/* Exported Variables --------------------------------------------------------*/
extern uint8_t frame[];

/* Function prototypes -------------------------------------------------------*/
void init_frame(uint8_t* ref);
void updateLEDMatrix(uint8_t index);
void shift_left(uint8_t isLeft);
void shift_up(uint8_t isUp);
#ifdef name_display
void shift_left_32();
#endif
#endif /* INC_LED_MATRIX_H_ */

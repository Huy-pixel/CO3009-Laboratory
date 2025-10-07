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
#define LED_MATRIX_ROW 8
#define LED_MATRIX_COL 8
/* GPIO_TypeDef* <> user label column pins used for control LED matrix */
#define COL0_PORT COL0_GPIO_Port
#define COL1_PORT COL1_GPIO_Port
#define COL2_PORT COL2_GPIO_Port
#define COL3_PORT COL3_GPIO_Port
#define COL4_PORT COL4_GPIO_Port
#define COL5_PORT COL5_GPIO_Port	/* If not use, set this to 0 */
#define COL6_PORT COL6_GPIO_Port	/* If not use, set this to 0 */
#define COL7_PORT COL7_GPIO_Port	/* If not use, set this to 0 */
/* GPIO_TypeDef* <> user label row pins used for control LED matrix*/
#define ROW0_PORT ROW0_GPIO_Port
#define ROW1_PORT ROW1_GPIO_Port
#define ROW2_PORT ROW2_GPIO_Port
#define ROW3_PORT ROW3_GPIO_Port
#define ROW4_PORT ROW4_GPIO_Port
#define ROW5_PORT ROW5_GPIO_Port
#define ROW6_PORT ROW6_GPIO_Port
#define ROW7_PORT ROW7_GPIO_Port	/* If not use, set this to 0 */
/* uint16_t <> user label column pins used for control LED matrix */
#define COL0_PIN COL0_Pin
#define COL1_PIN COL1_Pin
#define COL2_PIN COL2_Pin
#define COL3_PIN COL3_Pin
#define COL4_PIN COL4_Pin
#define COL5_PIN COL5_Pin 			/* If not use, set this to 0 */
#define COL6_PIN COL6_Pin			/* If not use, set this to 0 */
#define COL7_PIN COL7_Pin			/* If not use, set this to 0 */
/* uint16_t <> user label column pins used for control LED matrix */
#define ROW0_PIN ROW0_Pin
#define ROW1_PIN ROW1_Pin
#define ROW2_PIN ROW2_Pin
#define ROW3_PIN ROW3_Pin
#define ROW4_PIN ROW4_Pin
#define ROW5_PIN ROW5_Pin
#define ROW6_PIN ROW6_Pin
#define ROW7_PIN ROW7_Pin			/* If not use, set this to 0 */
/* Refer to hardware design on LED Matrix for compatible active/deactive state*/
#define LED_MATRIX_ROWSET		1U
#define LED_MATRIX_ROWRESET		0U
#define LED_MATRIX_COLSET		1U
#define LED_MATRIX_COLRESET		0U

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

/*
 * @file:	led7seg.h
 * @brief:	7-segment LED header file
 * 			contains API provided for control 7-segment LED
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */

#ifndef INC_LED7SEG_H_
#define INC_LED7SEG_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "main.h"

/* Function prototypes -------------------------------------------------------*/
void Led7seg_Init();
void display7SEG(uint8_t num, uint8_t led);

#endif /* INC_LED7SEG_H_ */

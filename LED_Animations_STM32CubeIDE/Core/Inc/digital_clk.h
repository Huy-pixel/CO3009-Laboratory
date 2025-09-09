/*
 * @file:	digital_clk.h
 * @brief:	12_LEDs-based digital clock header file
 * 			contains API provided for control a 12_LEDs-based digital clock
 *  Created on: Aug 29, 2025
 *      Author: soaic
 */

#ifndef INC_DIGITAL_CLK_H_
#define INC_DIGITAL_CLK_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "main.h"

/* Function prototypes -------------------------------------------------------*/
void clearAllClock();
void testClock(uint8_t counter);
void setNumberOnClock(uint8_t num);
void clearNumberOnClock(uint8_t num);

#endif /* INC_DIGITAL_CLK_H_ */

/*
 * @file:	software_timer.h
 * @brief:	software timer header file defines some APIs and interrupt flag
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Variables -----------------------------------------------------------------*/
extern uint16_t pTIM_counter[];
extern uint8_t  pTIM_flag[];
/* Function prototypes -------------------------------------------------------*/
void setTimer(uint16_t duration, uint8_t index);
void timer_run(uint8_t index);

#endif /* INC_SOFTWARE_TIMER_H_ */

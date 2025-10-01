/*
 * @file:	software_timer.h
 * @brief:	software-timer header file defines some APIs and interrupt flag
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stddef.h"
/* Defines -------------------------------------------------------------------*/
typedef enum {
	ONESHOT = 0U,
	PERIODIC
}TIMER_TYPE;

#define MAX_TIMER 10U	/* Configures number of timers for timer pool */
#define TIMER_CYCLE 10U /* The hardware-timer cycle, in milliseconds;
 	 	 	 	 	 	   Please refers to "STM32 Timers configuration" to set this up correctly */

/* Function prototypes -------------------------------------------------------*/
void software_timer_init(void);
uint8_t setTimer(uint16_t duration, TIMER_TYPE type);
uint8_t isTimer_expired(void);
void clear_flag(void);
void timer_run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */

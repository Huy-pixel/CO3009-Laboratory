/*
 * @file:	software_timer.h
 * @brief:	software-timer header file defines some APIs and interrupt flag
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
/* Defines -------------------------------------------------------------------*/
#define MAX_TIMER 	10U		/* Configures number of timers for timer memory pool
 	 	 	 	 	 	 	   This can range on use purposes, depends on on-chip flash and RAM */
#ifndef TIMER_CYCLE
#define TIMER_CYCLE 1U 		/* The hardware-timer cycle, in milliseconds;
 	 	 	 	 	 	   	   Please refers to "STM32 Timers configuration" to set this up correctly */
#endif
/* Function prototypes -------------------------------------------------------*/
void software_timer_init(void);
uint8_t setTimer(uint16_t delay, uint16_t period);
uint8_t get_flag(void);
void clear_flag(void);
void timer_run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */

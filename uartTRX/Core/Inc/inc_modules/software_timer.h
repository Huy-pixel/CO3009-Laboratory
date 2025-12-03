/*
 * @file:	software_timer.h
 * @brief:	software-timer header file defines some APIs and interrupt flag
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"

#define SIMPLIFIED_FOR_SIMULATION
//#	define queue_event
/* Defines -------------------------------------------------------------------*/
#define MAX_TIMER 	10U		/* Configures number of timers for timer memory pool
 	 	 	 	 	 	 	   This can range on use purposes, depends on on-chip flash and RAM */
#ifndef TIMER_CYCLE
#	define TIMER_CYCLE 10U	/* The hardware-timer cycle;
 	 	 	 	 	 	   	   Please refers to "STM32 Timers configuration" to set this up correctly */
#endif
/* Function prototypes -------------------------------------------------------*/
uint8_t software_timer_init(void);
#ifndef SIMPLIFIED_FOR_SIMULATION
const uint8_t setTimer(uint32_t delay, uint32_t period);
void get_flag(void);
void clear_flag(uint8_t timer_id);
uint8_t is_timer_expired(uint8_t timer_id);
#else
void setTimer(uint32_t duaration, uint8_t index);
uint8_t get_flag(uint8_t index);
#endif
#endif /* INC_SOFTWARE_TIMER_H_ */

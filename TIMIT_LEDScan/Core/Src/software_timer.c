/*
 * @file:	software_timer.c
 * @brief: 	software-timer source file
 * @Note: 	In scope of this exercise, just one hardware-timer TIM2 is being used
 * 			The author uses this TIM2 as base sys-tick for a number of software-timer counters
 * 				hereinafter referred to as timer pool
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

/* Private includes ----------------------------------------------------------*/
#include "software_timer.h"

/* Private defines -----------------------------------------------------------*/
#define MAX_TIMER 10U	/* Configures number of timers for timer pool */
#define TIMER_CYCLE 10U /* Please refers to hardware-timer cycle to set this up correctly */

/* Declare timer's components ------------------------------------------------*/
uint16_t pTIM_counter[MAX_TIMER];	/* a software-timer pool with 10 timer-counters available */
uint8_t  pTIM_flag[MAX_TIMER];		/* a timer flag pool in associate with timer pool */

/* Timer's implementation ----------------------------------------------------*/



/**
 * @brief	set timer interval of which is being used
 * @param	duration: timer's duration, must be a multiple of TIMER_CYCLE
 * 			index: index of timer being used in timer pool, must be in range of [0, MAX_TIMER]
 * @retval	None
 */
void setTimer(uint16_t duration, uint8_t index)
{
	pTIM_counter[index] = duration/TIMER_CYCLE;
	pTIM_flag[index] = 0;
}

/**
 * @brief	run software-timer
 * @param	index: index of timer being used in timer pool, must be in range of [0, MAX_TIMER]
 * @retval	None
 */
void timer_run(uint8_t index)
{
	if (pTIM_counter[index] > 0)
	{
		pTIM_counter[index]--;
		if (pTIM_counter[index] == 0)
			pTIM_flag[index] = 1;
	}
}





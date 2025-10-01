/*
 * @file:	software_timer.c
 * @brief: 	software-timer source file
 * @Note:	The author uses this TIM2 as base sys-tick for a number of software-timer counters
 * 				hereinafter referred to as timer pool
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

/* Private includes ----------------------------------------------------------*/
#include "software_timer.h"

/* Timer's components ------------------------------------------------*/
typedef struct timer_t timer_t;

struct timer_t
{
	/* Timer type components */
	uint8_t id; 			/* timer id: user can define up to 255 timers, 0 is preserved */
	uint16_t countdown; 	/* timer's duration */
	uint16_t default_counter;		/* default duration */
	TIMER_TYPE type;
	timer_t* next; 			/* pointer to the next timer */
};

static timer_t timer_pool[MAX_TIMER];			/* a software-timer pool with 10 timers available */
static uint8_t timer_seedID = 1;
timer_t* timer_head;	/* pointer to the active list */
timer_t* free_list;		/* pointer to the free list */
static uint8_t flag;
/* Software-timer's implementation ----------------------------------------------------*/

void software_timer_init(void)
{
	for (int i = 0; i < MAX_TIMER - 1; i++)
	{
		timer_pool[i].next = &timer_pool[i + 1];
	}
	timer_pool[MAX_TIMER - 1].next = NULL;
	free_list = &timer_pool[0];
}

timer_t* timer_construct(uint16_t counter, TIMER_TYPE type)
{
	if (free_list == NULL) {
	        return NULL;
	}
	timer_t* timer = free_list;
	free_list = free_list->next;

	timer->id = timer_seedID++;
	timer->countdown = counter;
	timer->default_counter = counter;
	timer->type = type;
	timer->next = NULL;
	return timer;
}

void timer_destruct(timer_t* timer)
{
	timer->next = free_list;
	free_list = timer;
}

void timer_add_to_list(timer_t* timer, timer_t* *head)
{
	/* If active list is empty */
	if (*head == NULL)
	{
		*head = timer;
		timer->next = NULL;
		return;
	}

	if (timer->countdown <= (*head)->countdown)
	{
		(*head)->countdown -= timer->countdown;
		timer->next = *head;
		*head = timer;
		return;
	}

	timer->countdown -= (*head)->countdown;
	timer_add_to_list(timer, &((*head)->next));
}

timer_t* timer_delete_from_list(timer_t* head)
{
	timer_t* victim = head;
	head = head->next;
	victim->next = NULL;
	return victim;
}

void timer_raise_flag(timer_t* timer_head)
{
	flag = timer_head->id;
}

/**
 * @brief	set timer interval of which is being used
 * @param	duration: timer's duration, must be a multiple of TIMER_CYCLE
 * 			index: index of timer being used in timer pool, must be in range of [0, MAX_TIMER]
 * @retval	None
 */
uint8_t setTimer(uint16_t interval, TIMER_TYPE type)
{
	uint16_t timer_counter = interval/TIMER_CYCLE;
	timer_t* instance = timer_construct(timer_counter, type);
	timer_add_to_list(instance, &timer_head);
	return instance->id;
}

/**
 * @brief	run software-timer
 * @param	index: index of timer being used in timer pool, must be in range of [0, MAX_TIMER]
 * @note
 * @retval	None
 */
void timer_run(void)
{
	if (timer_head->countdown > 0)
	{
		timer_head->countdown--;
		if (timer_head->countdown == 0)
			timer_raise_flag(timer_head);
	}
}

uint8_t isTimer_expired(void)
{
	if (flag)
	{
		timer_t* sample = timer_delete_from_list(timer_head);

		if (sample->type == PERIODIC)
		{
			sample->countdown = sample->default_counter;
			timer_add_to_list(sample, &timer_head);
		}
		else
		{
			timer_destruct(sample);
		}
	}
	return flag;
}

void clear_flag(void)
{
	flag = 0;
}






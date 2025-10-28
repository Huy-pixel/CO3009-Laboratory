/*
 * @file:	software_timer.c
 * @brief: 	software-timer source file
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "stdint.h"

/* Header file ---------------------------------------------------------------*/
#include "Inc_modules/software_timer.h"

/* Timer_type definition -----------------------------------------------------*/
typedef struct software_timer
{
	uint8_t id; 						/* timer id: user can define up to 255 timers, 0 is preserved for flag */
	volatile uint32_t countdown; 		/* timer's duration
							 	 	 	 * if using unsigned integer 16 bit, countdown can range from 0->65536 (2^16)
							 	 	 	 * if using signed	 integer 16 bit, countdown can range from -32768->32768 (2^15, 1 bit for sign)
							 	 	 	 */
	uint32_t period;					/* timer's period, used for reload countdown
	 	 	 	 	 	 	 	 	 	 * a PERIODIC timer has this attribute none zero, whereas a ONESHOT does not
	 	 	 	 	 	 	 	 	 	 */
	struct software_timer* p_next;		/* pointer to the next timer */
}timer_t;

/* Software-timer components --------------------------------------------------*/
static timer_t timer_memory_pool[MAX_TIMER];	/* a software-timer pool with 10 timers available */
static uint8_t g_seedID = 1;					/* seed for generate timer's id , 0 is preserved for flag*/
static timer_t* p_active_list;					/* pointer to the head of active list */
static timer_t* p_free_list;					/* pointer to the free list */
static volatile uint8_t g_flag;					/* global flag of software timer */

/* Singly linked list method-like functions forward declaration --------------*/
static timer_t* timer_fetch_free_slot(void);
static timer_t* timer_construct(uint32_t delay, uint32_t period);
static void timer_destruct(timer_t* timer);
static void timer_add_to_list(timer_t* timer, timer_t* *head);
static timer_t* timer_delete_head(timer_t* *head);
static void timer_raise_flag(timer_t* timer);
static void timer_memory_pool_init(void);
static inline void timer_run(timer_t* timer);
/* Private implementation ----------------------------------------------------*/

/*
 * This procedure fetch for free slot in free list, lead by pointer free_list
 */
static timer_t* timer_fetch_free_slot(void)
{
	if (!p_free_list) /* No more free slot */
		return NULL;
	timer_t* slot = p_free_list;
	p_free_list = p_free_list->p_next;
	slot->p_next = NULL;
	return slot;
}

/**
 * timer_t object constructor
 */
static timer_t* timer_construct(uint32_t delay, uint32_t period)
{
	timer_t* timer = timer_fetch_free_slot();
	if (!timer)		/* fetch free slot fail */
	{
		return NULL;
	}

	timer->id = g_seedID++;
	timer->countdown = delay;
	timer->period = period;
	timer->p_next = NULL;
	return timer;
}

/**
 * timer_t object destructor
 */
static void timer_destruct(timer_t* timer)
{
	if (!timer)
	{
		return;
	}
	timer->p_next = p_free_list;
	p_free_list = timer;
}

/**
 * Add a new timer into active list, lead by pointer timer_head
 */
static void timer_add_to_list(timer_t* timer, timer_t* *head)
{
	if (!timer || !head)
	{
		return;
	}

	timer->p_next = NULL;

	timer_t* *curr = head;
	while ( (*curr != NULL) && (timer->countdown >= (*curr)->countdown) ) /* the equals here is important */
	{
		timer->countdown -= (*curr)->countdown;
		curr = &(*curr)->p_next;
	}

	/* while break means current is NULL also means it is the last one
	 * or timer is at right place */
	if (*curr != NULL)
	{
		(*curr)->countdown -= timer->countdown;
	}

	timer->p_next = *curr;
	*curr = timer;
}

/*
 * Delete head of active list, because it's always the head expires
 */
static timer_t* timer_delete_head(timer_t* *head)
{
	if (!head || !(*head))
	{
		return NULL;
	}

	timer_t* victim = *head;
	*head = (*head)->p_next;
	victim->p_next = NULL;
	return victim;
}

/*
 * Set the global flag by id of the timer expires
 */
static void timer_raise_flag(timer_t* timer)
{
	g_flag = timer->id;
}

/*
 * Setup timer_t memory pool
 */
static void timer_memory_pool_init(void)
{
	for (int i = 0; i < MAX_TIMER - 1; i++)
	{
		timer_memory_pool[i].p_next = &timer_memory_pool[i + 1];
	}
	timer_memory_pool[MAX_TIMER - 1].p_next = NULL;
	p_free_list = &timer_memory_pool[0];
}

/**
 * @brief	Run the software-timer
 * @param	None
 * @retval	None
 */
static inline void timer_run(timer_t* timer)
{
	if (!timer)			/* No timer is used */
	{
		return;
	}

	if (timer->countdown > 0)
	{
		timer->countdown--;
		if (timer->countdown == 0)
		{
			timer_raise_flag(timer);
		}

	}
}
/* Software-timer API --------------------------------------------------------*/

/**
 * @brief	Initialize hardware TIM2 by HAL and memory pool
 * @param	None
 * @reval	None
 */
void software_timer_init(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
	timer_memory_pool_init();
}

/**
 * @brief	Set up new timer with delay and period
 * @param	delay:  time before timer goes off when this API called
 * 			period: time between a single time the timer goes off after the first expiration
 * @note	through the combined use of delay and period, user can set up either PERIODIC or ONESHOT timer
 * 			e.g: a ONESHOT timer has period value equals zero, whereas PERIODIC does not.
 * @retval	the timer's id
 */
uint8_t setTimer(uint32_t delay, uint32_t period)
{
	if (!delay && !period)				/* user define a null timer */
	{
		return 0;
	}

	if (!delay)
	{
		delay += TIMER_CYCLE; 			/* 0 in users perspective means no delay, but for timer_run() logic, no delay must be value as 1 */
	}

	timer_t* instance = timer_construct(delay/TIMER_CYCLE, period/TIMER_CYCLE);
	if (!instance)
	{
		return 0;
	}

	timer_add_to_list(instance, &p_active_list);
	return instance->id;
}

/**
 * @brief	Clear software-timer's flag
 * @param	None
 * @retval	None
 * @note	This function clear flag at first, a flag is cleared when it is expired.
 * 			Thus, this procedure do either delete a ONESHOT timer or move a PERIODIC
 * 			into its place in list.
 */
void clear_flag(void)
{
	g_flag = 0;

	timer_t* expire = timer_delete_head(&p_active_list);

	if (expire->period)
	{
		expire->countdown = expire->period;			/* reload PERIODIC timer's countdown */
		timer_add_to_list(expire, &p_active_list);		/* move to its place */
	}
	else
	{
		timer_destruct(expire);						/* destruct an expire one */
	}

	/* Re-check the following whether it expires */
	if (p_active_list->countdown == 0)
		timer_raise_flag(p_active_list);
}

/**
 * @brief	Get software-timer global flag
 * @param	None
 * @retval	None
 */
uint8_t get_flag(void)
{
	return g_flag;
}

/*
 * Interrupt Service Routines (ISR)
 * This function is invoked by hardware
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == TIM2)
	{
		timer_run(p_active_list);
	}
}

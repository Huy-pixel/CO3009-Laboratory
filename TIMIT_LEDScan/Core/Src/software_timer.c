/*
 * @file:	software_timer.c
 * @brief: 	software-timer source file
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

/* Private includes ----------------------------------------------------------*/
#include "software_timer.h"

/* Timer_type definition -----------------------------------------------------*/
typedef struct timer_t timer_t;

struct timer_t
{
	uint8_t id; 				/* timer id: user can define up to 255 timers, 0 is preserved for flag */
	uint16_t countdown; 		/* timer's duration
							 	 * if using unsigned integer 16 bit, countdown can range from 0->65536 (2^16)
							 	 * if using signed	 integer 16 bit, countdown can range from -32768->32768 (2^15, 1 bit for sign)
							 	 */
	uint16_t period;			/* timer's period, used for reload countdown
	 	 	 	 	 	 	 	 * a PERIODIC timer has this attribute none zero, whereas a ONESHOT does not
	 	 	 	 	 	 	 	 */
	timer_t* next; 				/* pointer to the next timer */
};

/* Software-timer components --------------------------------------------------*/
static timer_t timer_pool[MAX_TIMER];	/* a software-timer pool with 10 timers available */
static uint8_t timer_seedID = 1;		/* seed for generate timer's id , 0 is preserved for flag*/
static timer_t* timer_head;				/* pointer to the active list */
static timer_t* free_list;				/* pointer to the free list */
static uint8_t flag;					/* global flag of software timer */

/* Singly linked list method-like functions forward declaration --------------*/
timer_t* timer_fetch_free_slot(void);
timer_t* timer_construct(uint16_t delay, uint16_t period);
void timer_destruct(timer_t* timer);
void timer_add_to_list(timer_t* timer, timer_t* *head);
timer_t* timer_delete_head(timer_t* *head);
void timer_raise_flag(timer_t* timer_head);
void timer_memory_pool_init(void);
void timer_run(void);
/* Private implementation ----------------------------------------------------*/

/*
 * This procedure fetch for free slot in free list, lead by pointer free_list
 */
timer_t* timer_fetch_free_slot(void)
{
	if (free_list == NULL) /* No more free slot */
		return NULL;
	timer_t* slot = free_list;
	free_list = free_list->next;
	slot->next = NULL;
	return slot;
}

/**
 * timer_t object constructor
 */
timer_t* timer_construct(uint16_t delay, uint16_t period)
{
	timer_t* timer = timer_fetch_free_slot();
	if (timer == NULL) /* fetch free slot fail */
		return NULL;

	timer->id = timer_seedID++;
	timer->countdown = delay;
	timer->period = period;
	timer->next = NULL;
	return timer;
}

/**
 * timer_t object destructor
 */
void timer_destruct(timer_t* timer)
{
	if (!timer)
		return;
	timer->next = free_list;
	free_list = timer;
}

/**
 * Add a new timer into active list, lead by pointer timer_head
 */
void timer_add_to_list(timer_t* timer, timer_t* *head)
{
	if (!timer || !head)
		return;
	timer->next = NULL;

	timer_t* *current = head;
	while (*current != NULL && timer->countdown >= (*current)->countdown) /* the equals here is important */
	{
		timer->countdown -= (*current)->countdown;
		current = &(*current)->next;
	}

	/* while break means current is NULL also means it is the last one
	 * or timer is at right place also means it is right before the current */
	if (*current != NULL)
	{
		(*current)->countdown -= timer->countdown;
	}

	timer->next = *current;
	*current = timer;
}

/*
 * Delete head of active list, because it's always the head expires
 */
timer_t* timer_delete_head(timer_t* *head)
{
	if (!head || !(*head))
		return NULL;
	timer_t* victim = *head;
	*head = (*head)->next;
	victim->next = NULL;
	return victim;
}

/*
 * Set the global flag by id of the timer expires
 */
void timer_raise_flag(timer_t* timer_head)
{
	flag = timer_head->id;
}

/*
 * Setup timer_t memory pool
 */
void timer_memory_pool_init(void)
{
	for (int i = 0; i < MAX_TIMER - 1; i++)
	{
		timer_pool[i].next = &timer_pool[i + 1];
	}
	timer_pool[MAX_TIMER - 1].next = NULL;
	free_list = &timer_pool[0];
}

/**
 * @brief	Run the software-timer
 * @param	None
 * @retval	None
 */
void timer_run(void)
{
	if (!timer_head) return;	/* No timer is used */

	if (timer_head->countdown > 0)
	{
		timer_head->countdown--;
		if (timer_head->countdown <= 0)
			timer_raise_flag(timer_head);
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
 * @param	delay:  time before timer goes off when this API calls
 * 			period: time between a single time the timer goes off after the first expiration
 * @note	through the combined use of delay and period, user can set up either PERIODIC or ONESHOT timer
 * 			e.g: a ONESHOT timer has period value equals zero, whereas PERIODIC does not.
 * @retval	the timer's id
 */
uint8_t setTimer(uint16_t delay, uint16_t period)
{
	if (!delay && !period)				/* user define a null timer */
		return 0;
	if (!delay)
		delay += TIMER_CYCLE; 			/* 0 in users perspective means no delay, but for timer_run() logic, no delay must be value as 1 */

	timer_t* instance = timer_construct(delay/TIMER_CYCLE, period/TIMER_CYCLE);
	if (!instance)
		return 0;

	timer_add_to_list(instance, &timer_head);
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
	flag = 0;

	timer_t* expire = timer_delete_head(&timer_head);

	if (expire->period)
	{
		expire->countdown = expire->period;			/* reload PERIODIC timer's countdown */
		timer_add_to_list(expire, &timer_head);		/* move to its place */
	}
	else
	{
		timer_destruct(expire);						/* destruct an expire one */
	}

	/* Re-check the following whether it expires */
	if (timer_head->countdown <= 0)
		timer_raise_flag(timer_head);
}

/**
 * @brief	Get software-timer global flag
 * @param	None
 * @retval	None
 */
uint8_t get_flag(void)
{
	return flag;
}

/*
 * Interrupt Service Routines (ISR)
 * This function is invoked by hardware
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	timer_run();
}

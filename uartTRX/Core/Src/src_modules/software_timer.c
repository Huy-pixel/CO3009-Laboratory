/*
 * @file:	software_timer.c
 * @brief: 	software-timer source file
 *  Created on: Sep 9, 2025
 *      Author: soaic
 */

/* Private includes ----------------------------------------------------------*/
//#include "Inc_modules/queue.h"
#include "tim.h"
//#include "Inc_modules/button.h"
/* Header file ---------------------------------------------------------------*/
#include "Inc_modules/software_timer.h"

#ifndef SIMPLIFIED_FOR_SIMULATION

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
static timer_t* gp_active_list;					/* pointer to the head of active list */
static timer_t* gp_free_list;					/* pointer to the free list */
static volatile uint8_t g_flag_list[256];		/* global flag of software timer */
/* Singly linked list method-like functions forward declaration --------------*/
static timer_t* timer_fetch_free_slot(void);
static timer_t* timer_construct(uint32_t delay, uint32_t period);
static void timer_destruct(timer_t* timer);
static void timer_add_to_list(timer_t* timer, timer_t* *head);
static timer_t* timer_delete_head(timer_t* *head);
static void timer_memory_pool_init(void);
static inline void timer_raise_flag(timer_t* timer);
static inline uint8_t is_head_timer_expired();
static inline void timer_run();

/* Private implementation ----------------------------------------------------*/

/*
 * This procedure fetch for free slot in free list, lead by pointer free_list
 */
static timer_t* timer_fetch_free_slot(void)
{
	if (!gp_free_list) /* No more free slot */
		return NULL;
	timer_t* slot = gp_free_list;
	gp_free_list = gp_free_list->p_next;
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
	timer->p_next = gp_free_list;
	gp_free_list = timer;
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
static inline void timer_raise_flag(timer_t* timer)
{
	g_flag_list[timer->id] = timer->id;
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
	gp_free_list = &timer_memory_pool[0];
}

static inline uint8_t is_head_timer_expired()
{
	return gp_active_list->countdown == 0;
}

/**
 * @brief	Run the software-timer
 * @param	None
 * @retval	None
 */
static inline void timer_run()
{
	if (!gp_active_list)			/* No timer is used */
	{
		return;
	}

	if (gp_active_list->countdown > 0)
	{
		gp_active_list->countdown--;
	}
}
#else
static volatile uint32_t pTIM_counter[MAX_TIMER];			/* a software-timer pool with 10 timer-counters available */
static volatile uint8_t  pTIM_flag[MAX_TIMER];				/* a timer-flag pool in associate with timer pool */
static void timer_run()
{
	for (uint8_t i = 0; i < MAX_TIMER; i++)
	{
		if (pTIM_counter[i] > 0)
		{
			pTIM_counter[i]--;
			if (pTIM_counter[i] == 0)
				pTIM_flag[i] = 1;
		}
	}
}

#endif
/* Software-timer API --------------------------------------------------------*/

/**
 * @brief	Initialize hardware TIM2 by HAL and memory pool
 * @param	None
 * @reval	1 success; 0 fail
 */
uint8_t software_timer_init(void)
{
	HAL_StatusTypeDef init_val = HAL_TIM_Base_Start_IT(&htim2);
	while (init_val != HAL_OK);
#ifndef SIMPLIFIED_FOR_SIMULATION
	timer_memory_pool_init();
#endif
	return 1;
}

#ifndef SIMPLIFIED_FOR_SIMULATION
/**
 * @brief	Set up new timer with delay and period
 * @param	delay:  time before timer goes off when this API called
 * 			period: time between a single time the timer goes off after the first expiration
 * @note	through the combined use of delay and period, user can set up either PERIODIC or ONESHOT timer
 * 			e.g: a ONESHOT timer has period value equals zero, whereas PERIODIC does not.
 * @retval	the timer's id
 */
const uint8_t setTimer(uint32_t delay, uint32_t period)
{
	if (!delay && !period)				/* user define a null timer */
	{
		return 0;
	}

	timer_t* instance = timer_construct(delay/TIMER_CYCLE, period/TIMER_CYCLE);
	if (!instance)
	{
		return 0;
	}

	timer_add_to_list(instance, &gp_active_list);

	return instance->id;
}

void desetTimer(uint8_t id)
{

}

/**
 * @brief	Clear software-timer's flag
 * @param	None
 * @retval	None
 * @note	This function clear flag at first, a flag is cleared when it is expired.
 * 			Thus, this procedure do either delete a ONESHOT timer or move a PERIODIC
 * 			into its place in list.
 */
void clear_flag(uint8_t timer_id)
{
	g_flag_list[timer_id] = 0;
}

/**
 * @brief	Get software-timer global flag
 * @param	None
 * @retval	None
 * @Note    Must be call in while(1) or infinite loop without delay
 */
void get_flag(void)
{
	if (is_head_timer_expired())
	{
		timer_raise_flag(gp_active_list);

		timer_t* expire = timer_delete_head(&gp_active_list);

		if (expire->period)
		{
			expire->countdown = expire->period;			/* reload PERIODIC timer's countdown */
			timer_add_to_list(expire, &gp_active_list);		/* move to its place */
		}
		else
		{
			timer_destruct(expire);						/* destruct an expire one */
		}
	}
}

uint8_t is_timer_expired(uint8_t timer_id)
{
	return g_flag_list[timer_id];
}
#else
/* Declare timer's components ------------------------------------------------*/

/**
 * @brief	set timer interval of which is being used
 * @param	duration: timer's duration, must be a multiple of TIMER_CYCLE
 * 			index: index of timer being used in timer pool, must be in range of [0, MAX_TIMER]
 * @retval	None
 */
void setTimer(uint32_t duration, uint8_t index)
{
	pTIM_counter[index] = duration/TIMER_CYCLE;
	pTIM_flag[index] = 0;
}

uint8_t get_flag(uint8_t index)
{
	return pTIM_flag[index];
}
#endif

/*
 * Interrupt Service Routines (ISR)
 * This function is invoked by hardware
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == TIM2)
	{
		timer_run();
	}
}

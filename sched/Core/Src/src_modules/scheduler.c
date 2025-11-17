/*
 * scheduler.c
 *
 *  Created on: Nov 13, 2025
 *      Author: soaic
 */

#include "inc_modules/scheduler.h"
#include "inc_modules/stack.h"
#include "inc_modules/button.h"
#include "tim.h"
typedef struct task_type
{
	void (*pTask)();
	uint8_t taskid;
	volatile uint32_t delay; 			/*
							 	 	 	 * if using unsigned integer 16 bit, delay can range from 0->65536 (2^16)
							 	 	 	 * if using signed	 integer 16 bit, delay can range from -32768->32768 (2^15, 1 bit for sign)
							 	 	 	 */
	uint32_t period;					/* timer's period, used for reload delay
	 	 	 	 	 	 	 	 	 	 * a PERIODIC task has this attribute none zero, whereas a ONESHOT does not
	 	 	 	 	 	 	 	 	 	 */
	volatile uint8_t RunMe;
	struct task_type* p_next;		/* pointer to the next timer */
}task_t;

static task_t* gp_freelist;
static task_t* gp_active_list;					/* pointer to the head of active list */
static stack seedID_collector = {.top = -1};	/* Construct a stack for collecting expire id */

static task_t* task_memory_pool_init(task_t* task_arr)
{
	for (int i = 0; i < SCH_MAX_TASKS - 1; i++)
	{
		task_arr[i].p_next = &task_arr[i + 1];
	}
	task_arr[SCH_MAX_TASKS - 1].p_next = NULL;
	return task_arr;
}

static task_t* task_fetch_free_slot(void)
{
	static task_t task_memory_pool[SCH_MAX_TASKS];
	static uint8_t initialize = 0;
	if (!initialize)
	{
		gp_freelist = task_memory_pool_init(task_memory_pool);
		initialize = 1;
	}

	task_t* slot = gp_freelist;

	if (slot)
	{
		gp_freelist = gp_freelist->p_next;
		slot->p_next = NULL;
	}

	return slot;
}

static task_t* task_allocate(void (*pFunc)(), uint32_t delay, uint32_t period)
{
	task_t* task = task_fetch_free_slot();
	if (!task)		/* fetch free slot fail */
	{
		return NULL;
	}

	static uint8_t ID_seed = 0;

	if (!stack_pop(&seedID_collector, &task->taskid))
	{
		task->taskid = ID_seed++;
	}
	task->pTask = pFunc;
	task->delay = delay;
	task->period = period;
	if (!delay)
	{
		task->RunMe = 1;
	}
	else
	{
		task->RunMe = 0;
	}
	task->p_next = NULL;
	return task;
}

static void task_deallocate(task_t* task)
{
	if (!task)
	{
		return;
	}

	if (!stack_push(&seedID_collector, task->taskid))
	{
		//do something when stack is full
		//report error, abandon this id
	}

	task->p_next = gp_freelist;
	gp_freelist = task;
}

static void task_add(task_t* task, task_t* *head)
{
	if (!task || !head)
	{
		return;
	}

	task->p_next = NULL;

	task_t* *curr = head;
	while ( (*curr != NULL) && (task->delay >= (*curr)->delay) ) /* the equals here is important */
	{
		task->delay -= (*curr)->delay;
		curr = &(*curr)->p_next;
	}

	/* while break means current is NULL also means it is the last one
	 * or timer is at right place */
	if (*curr != NULL)
	{
		(*curr)->delay -= task->delay;
	}

	task->p_next = *curr;
	*curr = task;
}

static task_t* task_delete(uint8_t taskID, task_t* *head)
{
	if (!head || !(*head))
	{
		return NULL;
	}

	task_t* curr = *head;
	task_t* prev = NULL;

	if (curr->taskid == taskID) {
        *head = curr->p_next;
        curr->p_next = NULL;
        return curr;
	}

	while (curr != NULL && curr->taskid != taskID) {
        prev = curr;
        curr = curr->p_next;
	}

	if (curr == NULL)
	{
		return NULL;
	}

	curr->p_next->delay += curr->delay;		/* add its delay for the following, ensure the relations between tasks */
	prev->p_next = curr->p_next;
    curr->p_next = NULL;

	return curr;
}

static inline uint8_t is_task_wakeup(task_t* task)
{
	return task->RunMe;
}

/* API */
void SCH_Init(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
}

void SCH_Update(void)
{
	if (!gp_active_list)			/* No task is added */
	{
		return;
	}

	if (gp_active_list->delay == 0)
	{
		gp_active_list->RunMe += 1;
	}
	else
	{
		gp_active_list->delay--;
	}
}

const uint8_t SCH_Add_Task(void (*pFunc)(), uint32_t delay, uint32_t period)
{
	task_t* instance = task_allocate(pFunc, delay/CPU_TICK, period/CPU_TICK);
	if (!instance)
	{
		return 0;
	}

	task_add(instance, &gp_active_list);

	return instance->taskid;
}

const uint8_t SCH_Delete_Task(uint8_t taskID)
{
	task_t* victim = task_delete(taskID, &gp_active_list);

	if (!victim)
	{
		//report error
		return 0;
	}

	task_deallocate(victim);
	return 1;
}

void SCH_Dispatch(void)
{
	while (is_task_wakeup(gp_active_list))
	{
		task_t* worker = task_delete(gp_active_list->taskid , &gp_active_list);

		if (!worker)
		{
			//report error
			return;
		}

		(*worker->pTask)();
		worker->RunMe -= 1;

		if (worker->period)
		{
			worker->delay = worker->period;					/* reload PERIODIC task */
			task_add(worker, &gp_active_list);				/* move to its place */
		}
		else
		{
			task_deallocate(worker);						/* deallocate an expire one */
		}
	}

	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == TIM2)
	{
		SCH_Update();
		button_scan();
	}
}

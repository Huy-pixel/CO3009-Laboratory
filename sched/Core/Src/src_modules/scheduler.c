/*
 * scheduler.c
 *
 *  Created on: Nov 13, 2025
 *      Author: soaic
 */

#include "inc_modules/scheduler.h"
#include "inc_modules/stack.h"
typedef struct task_type
{
	void (*pTask)(void);
	uint8_t taskid;
	volatile uint32_t delay; 			/*
							 	 	 	 * if using unsigned integer 16 bit, delay can range from 0->65536 (2^16)
							 	 	 	 * if using signed	 integer 16 bit, delay can range from -32768->32768 (2^15, 1 bit for sign)
							 	 	 	 */
	uint32_t period;					/* timer's period, used for reload delay
	 	 	 	 	 	 	 	 	 	 * a PERIODIC task has this attribute none zero, whereas a ONESHOT does not
	 	 	 	 	 	 	 	 	 	 */
	uint8_t RunMe;
	struct task_type* p_next;		/* pointer to the next timer */
}task_t;

static task_t* gp_active_list;					/* pointer to the head of active list */

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
	static task_t* p_freelist = NULL;
	static initialize = 0;
	if (!p_freelist)
	{
		p_freelist = task_memory_pool_init(task_memory_pool);
	}

	task_t* slot = p_freelist;
	p_freelist = p_freelist->p_next;
	slot->p_next = NULL;

	if (!p_freelist)
	{
		//Report Error here
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

	static initialize = 0;
	static Stack seedID_collector;
	static ID_seed = 0;

	if (!initialize)
	{
		stack_init(&seedID_collector);
	}

	if (!stack_pop(&seedID_collector, &task->taskid))
	{
		task->taskid = ID_seed++;
	}
	task->pTask = pFunc;
	task->delay = delay;
	task->period = period;
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
	}

	task->p_next = gp_free_list;
	gp_free_list = task;
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

static task_t* task_delete(task_t* *head)
{
	if (!head || !(*head))
	{
		return NULL;
	}

	task_t* victim = *head;
	*head = (*head)->p_next;
	victim->p_next = NULL;
	return victim;
}

static inline void task_wake_up(task_t* task)
{
	task->RunMe += 1;
}

static inline uint8_t is_head_task_expired()
{
	return gp_active_list->delay == 0;
}

/* API */
void SCH_Init()
{
	//task_memory_pool_init();
	//stack_init(&seedID_collector);
}

static inline void SCH_Update(void)
{
	if (!gp_active_list)			/* No task is added */
	{
		return;
	}

	if (gp_active_list->delay > 0)
	{
		gp_active_list->delay--;
	}
}

const uint8_t SCH_Add_Task(void (*pFunc)(), uint32_t delay, uint32_t period)
{
	task_t* instance = task_allocate(pFunc, delay, period);
	if (!instance)
	{
		return 0;
	}

	task_add(instance, &gp_active_list);

	return instance->taskid;
}

void SCH_Dispatch(void)
{
	if (is_head_task_expired())
	{
		//task_wake_up(gp_active_list);

		task_t* worker = task_delete(&gp_active_list);

		(*worker->pTask)();

		if (worker->period)
		{
			worker->delay = worker->period;					/* reload PERIODIC timer's countdown */
			task_add_to_list(worker, &gp_active_list);		/* move to its place */
		}
		else
		{
			task_destruct(worker);						/* destruct an expire one */
		}
	}
}

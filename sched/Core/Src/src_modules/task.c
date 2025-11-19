/*
 * task.c
 *
 *  Created on: Nov 15, 2025
 *      Author: soaic
 */
#include "stdarg.h"
#include "inc_modules/led7seg.h"
#include "inc_modules/fsm_trafficlight.h"
#include "inc_modules/button.h"
#include "inc_modules/traffic_light.h"
#include "inc_modules/scheduler.h"

void task_status_led()
{
	HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
}

void task_display_7SEG()
{
	static uint8_t index = 0;
	Led7SEG_display(index);
	index = (index + 1) & 3;
}

void task_normode()
{
	if (button_press_consume(0))
	{
		normal_mode(1);
	}
	else
	{
		normal_mode(0);
	}
}

void task_manmode()
{
	if (button_press_consume(0))
	{
		manual_mode(1);
	}

	if (button_press_consume(1))
	{
		manual_mode(0);
	}
}

void task_cfgmode()
{
	if (button_press_consume(0))
	{
		configure_mode(1);
	}
	else
	{
		configure_mode(0);
	}
}

void task_amberblinkmode()
{
	if (button_press_consume(0))
	{
		amber_blinking_mode(1);
	}
	else
	{
		amber_blinking_mode(0);
	}
}

void task_main_fsm()
{
	static mode_t mode = init;
	static uint8_t task;

	static uint8_t index = 0;
	Led7SEG_display(index);

	switch(mode)
	{
	case init:
		mode = normode;
		task = SCH_Add_Task(task_normode, 2, 1000);
	break;
	case normode:
		if (is_button_pressed(0))
		{
			mode = manmode;
			if (!SCH_Delete_Task(task))
			{}
			task = SCH_Add_Task(task_manmode, 3, 10);
		}
		break;
	break;
	case manmode:
		if (is_button_pressed(0))
		{
			mode = cfgmode;
			if (!SCH_Delete_Task(task))
			{}
			task = SCH_Add_Task(task_cfgmode, 4, 10);
		}
		break;
	break;
	case cfgmode:
		if (is_button_pressed(0))
		{
			mode = amberblinkmode;
			if (!SCH_Delete_Task(task))
			{}
			task = SCH_Add_Task(task_amberblinkmode, 3, 250);
			break;
		}
	break;
	case amberblinkmode:
		if (is_button_pressed(0))
		{
			mode = normode;
			if (!SCH_Delete_Task(task))
			{}
			task = SCH_Add_Task(task_normode, 2, 1000);
			break;
		}
	break;
	default:
	break;
	}
	index = (index + 1) & 3;
}


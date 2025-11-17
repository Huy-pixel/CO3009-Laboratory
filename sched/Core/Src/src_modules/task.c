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
	if (is_button_pressed(0))
	{
		clear_button_press_flag(0);
		normal_mode(1);
	}
	else
	{
		normal_mode(0);
	}
}

void task_manmode()
{
	if (is_button_pressed(0))
	{
		clear_button_press_flag(0);
		manual_mode(1);
	}

	if (is_button_pressed(1))
	{
		clear_button_press_flag(1);
		manual_mode(0);
	}
}

void task_cfgmode()
{
	configure_mode(0);
}

void task_amberblinkmode()
{
	if (is_button_pressed(0))
	{
		clear_button_press_flag(0);
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
	switch(mode)
	{
	case init:
		mode = normode;
		task = SCH_Add_Task(task_normode, 0, 1000);
	break;
	case normode:
		if (is_button_pressed(0))
		{
			mode = manmode;
			if (!SCH_Delete_Task(task))
			{}
			task = SCH_Add_Task(task_manmode, 0, 10);
		}
		break;
	break;
	case manmode:
		if (is_button_pressed(0))
		{
			mode = cfgmode;
			if (!SCH_Delete_Task(task))
			{}
			task = SCH_Add_Task(task_cfgmode, 0, 10);
		}
		break;
	break;
	case cfgmode:
		if (is_button_pressed(0))
		{
			mode = amberblinkmode;
			if (!SCH_Delete_Task(task))
			{}
			task = SCH_Add_Task(task_amberblinkmode, 0, 250);
			break;
		}
	break;
	case amberblinkmode:
		if (is_button_pressed(0))
		{
			mode = normode;
			if (!SCH_Delete_Task(task))
			{}
			task = SCH_Add_Task(task_normode, 0, 1000);
			break;
		}
	break;
	default:
	break;
	}
}


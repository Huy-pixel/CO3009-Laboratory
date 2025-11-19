/*
 * fsm_trafficlight.c
 *
 *  Created on: Oct 28, 2025
 *      Author: soaic
 */

#include "Inc_modules/fsm_trafficlight.h"
#include "Inc_modules/led7seg.h"
#include "Inc_modules/button.h"
#include "Inc_modules/traffic_light.h"

static uint8_t g_RED_time = RED_TIME;
static uint8_t g_GREEN_time = GREEN_TIME;
static uint8_t g_AMBER_time = RED_TIME - GREEN_TIME;

typedef enum {
	traffic_init,
	red_green_state,
	red_amber_state,
	green_red_state,
	amber_red_state,
	error_state
}TrafficState;

typedef enum {
	cfg_red,
	cfg_green,
	save
}CfgMode;

uint8_t fsm_trafficlight_init(void)
{
	//setTimer(1000, 3);		/* Timer for real-time, interval of 1 second */
	return 1;
}

static TrafficState get_state(uint8_t mask)
{
	uint8_t RED0   = (mask >> 0) & 1;
	uint8_t AMBER0 = (mask >> 1) & 1;
    uint8_t GREEN0 = (mask >> 2) & 1;
    uint8_t RED1   = (mask >> 3) & 1;
    uint8_t AMBER1 = (mask >> 4) & 1;
    uint8_t GREEN1 = (mask >> 5) & 1;

    if (RED0 && GREEN1)  return red_green_state;
    if (RED0 && AMBER1)  return red_amber_state;
    if (GREEN0 && RED1)  return green_red_state;
    if (AMBER0 && RED1)  return amber_red_state;

    return error_state;
}

static inline uint8_t light_time_increment(uint8_t time)
{
	time++;
	if (time > 99)
	{
		time = 1;
	}
	return time;
}

static inline uint8_t light_time_decrement(uint8_t time)
{
	time--;
	if (!time)
	{
		time = 99;
	}
	return time;
}

void normal_mode(uint8_t is_transition)
{
	static TrafficState state = traffic_init; /* By default */
	static uint8_t state_change;
	static uint8_t RED_time = RED_TIME;
	static uint8_t GREEN_time = GREEN_TIME;
	static uint8_t AMBER_time = RED_TIME - GREEN_TIME;

	if (state == traffic_init)
	{
		state = red_green_state;
		state_change = 1;
	}

	if (is_transition)
	{
		state_change = 1;
		RED_time = g_RED_time;
		GREEN_time = g_GREEN_time;
		AMBER_time = g_AMBER_time;
	}

	if (state_change)
	{
		state_change = 0;
		switch(state)
		{
		case red_green_state:
			set_lights(light_set, light_rst, light_rst, light_rst, light_rst, light_set);
		break;
		case red_amber_state:
			set_lights(light_set, light_rst, light_rst, light_rst, light_set, light_rst);
		break;
		case green_red_state:
			set_lights(light_rst, light_rst, light_set, light_set, light_rst, light_rst);
		break;
		case amber_red_state:
			set_lights(light_rst, light_set, light_rst, light_set, light_rst, light_rst);
		break;
		default:
		break;
		}
	}

	switch(state)
	{
	case red_green_state:
		update_7SEG_buffer(RED_time, GREEN_time);

		if (RED_time > 0) RED_time--;
		if (GREEN_time > 0) GREEN_time--;

		if (GREEN_time == 0)
		{
			state = red_amber_state;
			state_change = 1;
		}
	break;
	case red_amber_state:
		update_7SEG_buffer(RED_time, AMBER_time);

		if (RED_time > 0) RED_time--;
		if (AMBER_time > 0) AMBER_time--;

		if (AMBER_time == 0)
		{
			state = green_red_state;
			state_change = 1;
			GREEN_time = g_GREEN_time;
			RED_time = g_RED_time;
			AMBER_time = g_AMBER_time;
		}
	break;
	case green_red_state:
		update_7SEG_buffer(GREEN_time, RED_time);

		if (GREEN_time > 0) GREEN_time--;
		if (RED_time > 0) RED_time--;

		if (GREEN_time == 0)
		{
			state = amber_red_state;
			state_change = 1;
		}
	break;
	case amber_red_state:
		update_7SEG_buffer(AMBER_time, RED_time);

		if (AMBER_time > 0) AMBER_time--;
		if (RED_time > 0) RED_time--;

		if (AMBER_time == 0)
		{
			state = red_green_state;
			state_change = 1;
			RED_time = g_RED_time;
			GREEN_time = g_GREEN_time;
			AMBER_time = g_AMBER_time;
		}
	break;
	default:
	break;
	}
}

void manual_mode(uint8_t is_transition)
{
	static TrafficState state = traffic_init; /* By default */

	if (is_transition)
	{
		update_7SEG_buffer(99, 99);

		uint8_t light_mask = get_lights();
		state = get_state(light_mask);
		if (state == error_state)
		{
			state = red_green_state;
		}

		switch(state)
		{
		case red_green_state:
			set_lights(light_set, light_rst, light_rst, light_rst, light_rst, light_set);
		break;
		case red_amber_state:
			set_lights(light_set, light_rst, light_rst, light_rst, light_set, light_rst);
		break;
		case green_red_state:
			set_lights(light_rst, light_rst, light_set, light_set, light_rst, light_rst);
		break;
		case amber_red_state:
			set_lights(light_set, light_rst, light_rst, light_rst, light_rst, light_set);
		break;
		default:
		break;
		}
	}
	else
	{
		switch(state)
		{
		case red_green_state:
			set_lights(light_set, light_rst, light_rst, light_rst, light_set, light_rst);
			state = red_amber_state;
		break;
		case red_amber_state:
			set_lights(light_rst, light_rst, light_set, light_set, light_rst, light_rst);
			state = green_red_state;
		break;
		case green_red_state:
			set_lights(light_rst, light_set, light_rst, light_set, light_rst, light_rst);
			state = amber_red_state;
		break;
		case amber_red_state:
			set_lights(light_set, light_rst, light_rst, light_rst, light_rst, light_set);
			state = red_green_state;
		break;
		default:
		break;
		}
	}
}

/**
 * @brief	Configure mode
 */
void configure_mode(uint8_t is_transition)
{
	static uint8_t counter = 0;
	static CfgMode mode = cfg_red;	/* By default */

	static uint8_t RED_time = RED_TIME;
	static uint8_t GREEN_time = GREEN_TIME;

	if (is_transition)
	{
		switch(mode)
		{
		case cfg_red:
			update_7SEG_buffer(RED_time, RED_time);
		break;
		case cfg_green:
			update_7SEG_buffer(GREEN_time, GREEN_time);
		break;
		default:
			mode = cfg_red;
			update_7SEG_buffer(RED_time, RED_time);
		break;
		}
	}

	if (button_hold_consume(3))
	{
		if (RED_time > GREEN_time)
		{
			g_RED_time = RED_time;
			g_GREEN_time = GREEN_time;
			g_AMBER_time = RED_time - GREEN_time;

			set_lights(light_set, light_rst, light_rst, light_rst, light_rst, light_set);
			update_7SEG_buffer(RED_time, GREEN_time);

			mode = save;
			counter = 0;
		}
	}

	switch(mode)
	{
	case cfg_red:
		if (button_press_consume(1))
		{
			RED_time = light_time_increment(RED_time);
			update_7SEG_buffer(RED_time, RED_time);
		}

		if (button_press_consume(2))
		{
			RED_time = light_time_decrement(RED_time);
			update_7SEG_buffer(RED_time, RED_time);
		}

		if (button_press_consume(3))
		{
			mode = cfg_green;
			counter = 0;
			update_7SEG_buffer(GREEN_time, GREEN_time);
		}

		if (counter < 64)
		{
			set_lights(light_set, light_rst, light_rst, light_set, light_rst, light_rst);
		}
		else
		{
			set_lights(light_rst, light_rst, light_rst, light_rst, light_rst, light_rst);
		}
	break;
	case cfg_green:
		if (button_press_consume(1))
		{
			GREEN_time = light_time_increment(GREEN_time);
			update_7SEG_buffer(GREEN_time, GREEN_time);
		}

		if (button_press_consume(2))
		{
			GREEN_time = light_time_decrement(GREEN_time);
			update_7SEG_buffer(GREEN_time, GREEN_time);
		}

		if (button_press_consume(3))
		{
			mode = cfg_red;
			counter = 0;
			update_7SEG_buffer(RED_time, RED_time);
		}

		if (counter < 64)
		{
			set_lights(light_rst, light_rst, light_set, light_rst, light_rst, light_set);
		}
		else
		{
			set_lights(light_rst, light_rst, light_rst, light_rst, light_rst, light_rst);
		}
	break;
	case save:
		 counter += 1;
		 if (counter >= 200)
		 {
			 mode = cfg_red;
			 counter = 0;
		 }
	break;
	default:
	break;
	}

	counter += 1;
}

/**
 * @brief	Amber blinking mode (caution mode)
 * 			Duty cycle of 50%, frequency 1Hz
 */
void amber_blinking_mode(uint8_t is_transition)
{
	static uint8_t counter = 0;

	if (is_transition)
	{
		update_7SEG_buffer(100, 100);
	}

	if (counter < 3)
	{
		set_lights(light_rst, light_set, light_rst, light_rst, light_set, light_rst);
	}
	else
	{
		set_lights(light_rst, light_rst, light_rst, light_rst, light_rst, light_rst);
	}

	counter = (counter + 1) & 3;
}

/**
 * @brief 	Main finite state machine, contains all mode
 */
void fsm_trafficlight()
{
	static mode_t mode = init;	/* By default */
	static uint8_t is_transition = 0;
	switch(mode)
	{
	case init:
		mode = normode;
		is_transition = 1;
	break;
	case normode:
		normal_mode(is_transition);
		if (is_button_pressed(0))
		{
			mode = manmode;
			is_transition = 1;
			break;
		}
		is_transition = 0;
	break;
	case manmode:
		manual_mode(is_transition);
		if (is_button_pressed(0))
		{
			mode = cfgmode;
			is_transition = 1;
			break;
		}
		is_transition = 0;
	break;
	case cfgmode:
		configure_mode(is_transition);
		if (is_button_pressed(0))
		{
			mode = amberblinkmode;
			is_transition = 1;
			break;
		}
		is_transition = 0;
	break;
	case amberblinkmode:
		amber_blinking_mode(is_transition);
		if (is_button_pressed(0))
		{
			mode = normode;
			is_transition = 1;
			break;
		}
		is_transition = 0;
	break;
	default:
	break;
	}
}

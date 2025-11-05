/*
 * button.c
 *
 *  Created on: Oct 19, 2025
 *      Author: soaic
 */

#include "Inc_modules/software_timer.h"
#include "Inc_modules/button.h"

#define LONG_PRESS_COUNT			(LONG_PRESS_TIMEOUT/TIMER_CYCLE)
#define DEBOUNCE_COUNT				(BUTTON_BOUNCE_TIME/TIMER_CYCLE)
#define REPEAT_DELAY_COUNT			(BUTTON_REPEATED_PRESS_TIME/TIMER_CYCLE)

#define SAMPLE_MASK					((uint32_t) ((1UL << (DEBOUNCE_COUNT + 1)) - 1))

#define IS_STABLE_PRESSED(buf) 		(((buf) & SAMPLE_MASK) == SAMPLE_MASK)
#define IS_STABLE_RELEASED(buf)  	(((buf) & SAMPLE_MASK) == 0)

typedef enum{
	release = 0,
	press,
	hold
}ButtonState;

typedef struct {
    volatile uint32_t sample_buffer;
    volatile ButtonState state;
    volatile uint16_t press_counter;
    volatile uint16_t repeat_counter;
} button_t;

static button_t g_button[NUM_BUTTON];

static volatile uint16_t g_button_press_event_mask;
static volatile uint16_t g_button_hold_event_mask;

GPIO_TypeDef* button_port[NUM_BUTTON] =
{
		BUTTON_PORT0,
		BUTTON_PORT1,
		BUTTON_PORT2,
		BUTTON_PORT3
};

uint16_t button_pin[NUM_BUTTON] =
{
		BUTTON_PIN0,
		BUTTON_PIN1,
		BUTTON_PIN2,
		BUTTON_PIN3
};

void button_scan(void)
{
	for (uint8_t i = 0; i < NUM_BUTTON; i++)
	{
		button_t *btn = &g_button[i];

		uint32_t sample = HAL_GPIO_ReadPin(button_port[i], button_pin[i]) ? BUTTON_PRESSED : BUTTON_RELEASED;
		btn->sample_buffer = ((btn->sample_buffer << 1) | sample) & SAMPLE_MASK;

		if (IS_STABLE_PRESSED(btn->sample_buffer))
		{
			switch (btn->state)
			{
				case release:
					btn->state = press;
					btn->press_counter = 0;
					btn->repeat_counter = 0;
				break;
				case press:
					if (btn->press_counter < 0xFFFFU) btn->press_counter++;
					if (btn->press_counter >= LONG_PRESS_COUNT)
					{
						btn->state = hold;
						btn->press_counter = LONG_PRESS_COUNT;
						btn->repeat_counter = 0;
						g_button_hold_event_mask |= (1 << i);
					}
				break;
				case hold:
					if (btn->repeat_counter < 0xFFFFU) btn->repeat_counter++;
					if (btn->repeat_counter >= REPEAT_DELAY_COUNT)
					{
						btn->repeat_counter = 0;
						g_button_press_event_mask |= (1 << i);
					}
				break;
		    }
		}
		else if (IS_STABLE_RELEASED(btn->sample_buffer))
		{
			if (btn->state != release)
			{
				btn->state = release;
				btn->press_counter = 0;
				btn->repeat_counter = 0;
				g_button_press_event_mask |= (1 << i);
			}
		}
		else
		{}
	}
}

uint8_t is_button_pressed(uint8_t index)
{
	if (index >= NUM_BUTTON) return 0;
	return (g_button_press_event_mask & (1 << index));
}

void clear_button_press_flag(uint8_t index)
{
	g_button_press_event_mask &= ~(1 << index);
}

uint8_t is_button_hold(uint8_t index)
{
	if (index >= NUM_BUTTON) return 0xff;
	return (g_button_hold_event_mask & (1 << index));
}

void clear_button_hold_flag(uint8_t index)
{
	g_button_hold_event_mask &= ~(1 << index);
}

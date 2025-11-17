/*
 * stack.c
 *
 *  Created on: Nov 13, 2025
 *      Author: soaic
 */

#include "inc_modules/stack.h"

uint8_t stack_is_empty(stack *s)
{
	return s->top < 0;
}

uint8_t stack_is_full(stack *s)
{
	return s->top >= SEED_STACK_MAX - 1;
}

uint8_t stack_push(stack *s, uint8_t data)
{
	if (!stack_is_full(s))
	{
		s->data[++s->top] = data;
		return 1;
	}
	return 0;
}

uint8_t stack_pop(stack *s, uint8_t* data)
{
	if (!stack_is_empty(s))
	{
		*data = s->data[s->top--];
		return 1;
	}
	return 0;
}

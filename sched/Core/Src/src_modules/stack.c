/*
 * stack.c
 *
 *  Created on: Nov 13, 2025
 *      Author: soaic
 */

#include "inc_modules/stack.h"

void stack_init(Stack *s)
{
    s->top = -1;
}

uint8_t stack_is_empty(Stack *s)
{
	return s->top < 0;
}

uint8_t stack_is_full(Stack *s)
{
	return s->top >= SEED_STACK_MAX - 1;
}

uint8_t stack_push(Stack *s, uint8_t data)
{
	if (!stack_is_full(s))
	{
		s->data[++s->top] = data;
		return 1;
	}
	return 0;
}

uint8_t stack_pop(Stack *s, uint8_t* data)
{
	if (!stack_is_empty(s))
	{
		*data = s->data[s->top--];
		return 1;
	}
	return 0;
}

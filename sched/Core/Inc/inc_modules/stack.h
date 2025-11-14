/*
 * stack.h
 *
 *  Created on: Nov 13, 2025
 *      Author: soaic
 */

#ifndef INC_INC_MODULES_STACK_H_
#define INC_INC_MODULES_STACK_H_

#include "stdint.h"

#define SEED_STACK_MAX 256

typedef struct {
    uint8_t data[SEED_STACK_MAX];
    int8_t top;
}Stack;

void stack_init(Stack *s);
uint8_t stack_push(Stack *s, uint8_t data);
uint8_t stack_pop(Stack *s, uint8_t* data);

#endif /* INC_INC_MODULES_STACK_H_ */

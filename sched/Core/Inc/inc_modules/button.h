/*
 * button.h
 *
 *  Created on: Oct 19, 2025
 *      Author: soaic
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define TIMER_CYCLE			1U
/**
 *	A press that more than 500ms is a long press
 */
#ifndef BUTTON_TIMEOUTS_CONFIG
#	define LONG_PRESS_TIMEOUT 			500		/* in milliseconds */
#	define BUTTON_BOUNCE_TIME			10 			/* this can vary from 10-50ms */
#	define BUTTON_REPEATED_PRESS_TIME	200
#endif

#define NUM_BUTTON			4
#define BUTTON_PRESSED		GPIO_PIN_RESET
#define BUTTON_RELEASED		GPIO_PIN_SET

#define BUTTON_PORT0		BTN0_GPIO_Port
#define BUTTON_PORT1		BTN1_GPIO_Port
#define BUTTON_PORT2		BTN2_GPIO_Port
#define BUTTON_PORT3		BTN3_GPIO_Port

#define BUTTON_PIN0			BTN0_Pin
#define BUTTON_PIN1			BTN1_Pin
#define BUTTON_PIN2			BTN2_Pin
#define BUTTON_PIN3			BTN3_Pin

void button_scan (void);
uint8_t is_button_pressed(uint8_t index);
uint8_t is_button_hold(uint8_t index);
void clear_button_hold_flag(uint8_t index);
void clear_button_press_flag(uint8_t index);
uint8_t button_press_consume(uint8_t index);
uint8_t button_hold_consume(uint8_t index);
#endif /* INC_BUTTON_H_ */

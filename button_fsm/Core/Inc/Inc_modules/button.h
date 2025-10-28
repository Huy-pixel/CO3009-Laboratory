/*
 * button.h
 *
 *  Created on: Oct 19, 2025
 *      Author: soaic
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"
#include "Inc_modules/software_timer.h"

/**
 *	A press that more than 500ms is a long press
 */
#ifndef LONG_PRESS_TIMEOUT
#	define LONG_PRESS_TIMEOUT 	500	/* in milliseconds */
#endif

#define NUM_BUTTON			3
#define BUTTON_PRESSED		GPIO_PIN_RESET
#define BUTTON_RELEASED		GPIO_PIN_SET

#define BUTTON_PORT0		BUTTON_0_GPIO_Port
#define BUTTON_PORT1		BUTTON_1_GPIO_Port
#define BUTTON_PORT2		BUTTON_2_GPIO_Port

#define BUTTON_PIN0			BUTTON_0_Pin
#define BUTTON_PIN1			BUTTON_1_Pin
#define BUTTON_PIN2			BUTTON_2_Pin

#define BUTTON_BOUNCE_TIME			10 		/* this can vary from 10-50ms  */
#define BUTTON_REPEATED_PRESS_TIME	100

void button_scan (void);
uint8_t is_button_pressed(uint8_t index);
void clear_button_press_flag(uint8_t index);
uint8_t is_button_hold(uint8_t index);
void clear_button_hold_flag(uint8_t index);
#endif /* INC_BUTTON_H_ */

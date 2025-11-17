/*
 * fsm_trafficlight.h
 *
 *  Created on: Oct 28, 2025
 *      Author: soaic
 */

#ifndef INC_INC_MODULES_FSM_TRAFFICLIGHT_H_
#define INC_INC_MODULES_FSM_TRAFFICLIGHT_H_

#include "main.h"

#define RED_TIME		10U
#define GREEN_TIME		9U

typedef enum {
	init,
	normode,
	manmode,
	cfgmode,
	amberblinkmode
}mode_t;

uint8_t fsm_trafficlight_init(void);
void fsm_trafficlight(void);
void normal_mode(uint8_t is_transition);
void manual_mode(uint8_t is_transition);
void configure_mode(uint8_t is_transition);
void amber_blinking_mode(uint8_t is_transition);
#endif /* INC_INC_MODULES_FSM_TRAFFICLIGHT_H_ */

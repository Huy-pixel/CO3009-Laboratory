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

uint8_t fsm_trafficlight_init(void);
void fsm_trafficlight(void);
#endif /* INC_INC_MODULES_FSM_TRAFFICLIGHT_H_ */

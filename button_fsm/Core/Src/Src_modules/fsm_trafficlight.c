/*
 * fsm_trafficlight.c
 *
 *  Created on: Oct 28, 2025
 *      Author: soaic
 */

#include "Inc_modules/fsm_trafficlight.h"

static uint8_t RED_time = RED_TIME;
static uint8_t GREEN_time = GREEN_TIME;
static uint8_t AMBER_time = RED_TIME - GREEN_TIME;

void normal_mode()
{

}

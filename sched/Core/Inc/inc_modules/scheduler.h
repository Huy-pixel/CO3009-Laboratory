/*
 * scheduler.h
 *
 *  Created on: Nov 13, 2025
 *      Author: soaic
 */

#ifndef INC_INC_MODULES_SCHEDULER_H_
#define INC_INC_MODULES_SCHEDULER_H_

#include "main.h"

#define SCH_MAX_TASKS		40U
#define CPU_TICK			1U
void SCH_Init(void);
void SCH_Update(void);
const uint8_t SCH_Add_Task(void (*pFunc)(), uint32_t delay, uint32_t period);
const uint8_t SCH_Delete_Task(uint8_t taskID);
void SCH_Dispatch(void);

#endif /* INC_INC_MODULES_SCHEDULER_H_ */

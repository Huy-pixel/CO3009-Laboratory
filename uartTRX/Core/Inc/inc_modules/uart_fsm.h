/*
 * uart_fsm.h
 *
 *  Created on: Dec 2, 2025
 *      Author: soaic
 */

#ifndef INC_INC_MODULES_UART_FSM_H_
#define INC_INC_MODULES_UART_FSM_H_

#define MAX_BUFFER_SIZE 30

extern uint8_t buffer_flag;

void get_uart_message(UART_HandleTypeDef *huart);
void fsm_command_parser();
void fsm_uart_communication();

#endif /* INC_INC_MODULES_UART_FSM_H_ */

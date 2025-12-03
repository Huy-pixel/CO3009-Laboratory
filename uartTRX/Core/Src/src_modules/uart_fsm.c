/*
 * uart_fsm.c
 *
 *  Created on: Dec 2, 2025
 *      Author: soaic
 */

#include "main.h"
#include "inc_modules/software_timer.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "adc.h"
#include "inc_modules/uart_fsm.h"

typedef enum
{
	NONE,
	RST,
	OK
}CommandType;
CommandType command_data;

typedef enum
{
	INIT,
	WAITING,
	SENDING,
	WAIT_ACK
}UartCommunicationState;


typedef enum
{
	CMD_WAIT,
	CMD_GET
}CommandParserState;

static uint8_t temp = 0;
static uint8_t buffer[MAX_BUFFER_SIZE];
static uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
static uint8_t str[10];
static uint8_t command_flag;

uint32_t ADC_value = 0;

void get_uart_message(UART_HandleTypeDef *huart)
{
	buffer[index_buffer++] = temp;

	if (index_buffer == MAX_BUFFER_SIZE)
	{
		index_buffer = 0;
	}

	buffer_flag = 1;

	HAL_UART_Receive_IT(&huart2, &temp, 1);
}

void fsm_command_parser()
{
	static CommandParserState state = CMD_WAIT;
	switch (state)
	{
		case CMD_WAIT:
			if (temp == '!') // Change state: CMD_GET
			{
				index_buffer = 0;
				state = CMD_GET;
			}
			break;
		case CMD_GET:
			if (temp == '#') // Change state: CMD_WAIT
			{
				if (buffer[0] == 'R' && buffer[1] == 'S'  && buffer[2] == 'T' && buffer[3] == '#')
				{
					command_data = RST;
					command_flag = 1;
				}
				if (buffer[0] == 'O' && buffer[1] == 'K'  && buffer[2] == '#')
				{
					command_data = OK;
					command_flag = 1;
				}
				state = CMD_WAIT;
			}
		break;
		default:
		break;
	}
}

void fsm_uart_communication()
{
	static UartCommunicationState state = INIT;
	switch (state)
	{
		case INIT:
			state = WAITING;
		break;
		case WAITING:
			if (command_flag == 1 && command_data == RST)
			{
				ADC_value = HAL_ADC_GetValue(&hadc1);
				sprintf((char*)str, "\r\n!ADC=%ld#\r\n", ADC_value);

				command_flag = 0;
				state = SENDING;
			}
		break;
		case SENDING:
			HAL_UART_Transmit(&huart2, str, sizeof(str) , HAL_MAX_DELAY);
			setTimer(3000, 0);
			state = WAIT_ACK;
		break;

		case WAIT_ACK:

			if (command_flag == 1 && command_data == OK)
			{
				command_flag = 0;
				state = WAITING;
			}
			if (get_flag(0))
			{
				setTimer(3000, 0);
				state = SENDING;
			}
		break;
		default:
		break;
	}
}

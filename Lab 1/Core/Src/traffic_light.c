/*
 * file:	traffic_light.c
 * brief:	Source file for traffic_light control
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */

/* Private includes ----------------------------------------------------------*/
#include "traffic_light.h"
/* Private code --------------------------------------------------------------*/
void Light_Init()
{
	HAL_GPIO_TogglePin(RED_x_GPIO_Port, RED_x_Pin);
	HAL_GPIO_TogglePin(YELLOW_x_GPIO_Port, YELLOW_x_Pin);
	HAL_GPIO_TogglePin(GREEN_x_GPIO_Port, GREEN_x_Pin);

	HAL_GPIO_TogglePin(RED_y_GPIO_Port, RED_y_Pin);
	HAL_GPIO_TogglePin(YELLOW_y_GPIO_Port, YELLOW_y_Pin);
	HAL_GPIO_TogglePin(GREEN_y_GPIO_Port, GREEN_y_Pin);
}

void Light_Control(uint8_t bitmask)
{
	HAL_GPIO_WritePin(RED_x_GPIO_Port, RED_x_Pin, (bitmask & xRED) ? 1:0);
	HAL_GPIO_WritePin(YELLOW_x_GPIO_Port, YELLOW_x_Pin, (bitmask & xYELLOW) ? 1:0);
	HAL_GPIO_WritePin(GREEN_x_GPIO_Port, GREEN_x_Pin, (bitmask & xGREEN) ? 1:0);

	HAL_GPIO_WritePin(RED_y_GPIO_Port, RED_y_Pin, (bitmask & yRED) ? 1:0);
	HAL_GPIO_WritePin(YELLOW_y_GPIO_Port, YELLOW_y_Pin, (bitmask & yYELLOW) ? 1:0);
	HAL_GPIO_WritePin(GREEN_y_GPIO_Port, GREEN_y_Pin, (bitmask & yGREEN) ? 1:0);
}



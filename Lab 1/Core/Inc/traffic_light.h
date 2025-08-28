/*
 * file:	traffic_light.h
 * brief:	Header file provides traffic_light control API
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Define --------------------------------------------------------------------*/
//Define LED value for bit-mask usage
#define xRED 	((uint8_t) 1 << 0)
#define xYELLOW ((uint8_t) 1 << 1)
#define xGREEN 	((uint8_t) 1 << 2)
#define yRED 	((uint8_t) 1 << 3)
#define yYELLOW ((uint8_t) 1 << 4)
#define yGREEN 	((uint8_t) 1 << 5)

/* Function prototypes -------------------------------------------------------*/
void Light_Init();
void Light_Control(uint8_t bitmask);

#endif /* INC_TRAFFIC_LIGHT_H_ */

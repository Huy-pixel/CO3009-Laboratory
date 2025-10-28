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
#ifndef TRAFFIC_LIGHT_HARDWARE_MODULE
#	define xRED 		((uint8_t) 1 << 0)
#	define xGREEN 		((uint8_t) 1 << 1)
#	define xYELLOW 		((uint8_t) 1 << 2)
#	define yRED 		((uint8_t) 1 << 3)
#	define yGREEN		((uint8_t) 1 << 4)
#	define yYELLOW 		((uint8_t) 1 << 5)
#else
#	define xRED			((uint8_t) 0x0001)
#	define xAMBER		((uint8_t) 0x0002)
#	define xGREEN		((uint8_t) 0x0003)
#	define yRED			((uint8_t) 0x0100)
#	define yAMBER		((uint8_t) 0x0200)
#	define yGREEN		((uint8_t) 0x0300)
#endif
#define LIGHT_SET	0U
#define LIGHT_RESET	1U

/* Function prototypes -------------------------------------------------------*/
void Light_Init();
void Light_Control(uint8_t bitmask);

#endif /* INC_TRAFFIC_LIGHT_H_ */

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
#include "software_timer.h"
/* Define --------------------------------------------------------------------*/
//Define LED value for bit-mask usage

#define LIGHT_SET	0U
#define LIGHT_RESET	1U

#define 	REDLIGHT_PORT0		RED_0_GPIO_Port
#define		AMBERLIGHT_PORT0	AMBER_0_GPIO_Port
#define		GREENLIGHT_PORT0	AMBER_1_GPIO_Port
#define		REDLIGHT_PORT1		RED_1_GPIO_Port
#define		AMBERLIGHT_PORT1	AMBER_1_GPIO_Port
#define		GREENLIGHT_PORT1	GREEN_1_GPIO_Port

#define		REDLIGHT_PIN0		RED_0_Pin
#define		AMBERLIGHT_PIN0		AMBER_0_Pin
#define		GREENLIGHT_PIN0		GREEN_0_Pin
#define		REDLIGHT_PIN1		RED_1_Pin
#define		AMBERLIGHT_PIN1		AMBER_1_Pin
#define		GREENLIGHT_PIN1		GREEN_1_Pin

/* Function prototypes -------------------------------------------------------*/

typedef enum
{
	light_rst = 0,
	light_set,
}TrafLig_t;

void set_lights(TrafLig_t red1, TrafLig_t amber1, TrafLig_t green1, TrafLig_t red2, TrafLig_t amber2, TrafLig_t green2);

#endif /* INC_TRAFFIC_LIGHT_H_ */

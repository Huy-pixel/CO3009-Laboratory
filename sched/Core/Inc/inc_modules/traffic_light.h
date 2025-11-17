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

#define LIGHT_SET	1U
#define LIGHT_RESET	0U

#define 	REDLIGHT_PORT0		RED0_GPIO_Port
#define		AMBERLIGHT_PORT0	AMBER0_GPIO_Port
#define		GREENLIGHT_PORT0	GREEN0_GPIO_Port
#define		REDLIGHT_PORT1		RED1_GPIO_Port
#define		AMBERLIGHT_PORT1	AMBER1_GPIO_Port
#define		GREENLIGHT_PORT1	GREEN1_GPIO_Port

#define		REDLIGHT_PIN0		RED0_Pin
#define		AMBERLIGHT_PIN0		AMBER0_Pin
#define		GREENLIGHT_PIN0		GREEN0_Pin
#define		REDLIGHT_PIN1		RED1_Pin
#define		AMBERLIGHT_PIN1		AMBER1_Pin
#define		GREENLIGHT_PIN1		GREEN1_Pin

/* Function prototypes -------------------------------------------------------*/

typedef enum
{
	light_rst = 0,
	light_set,
}TrafLig_t;

void traffic_light_init(void);
void set_lights(TrafLig_t red1, TrafLig_t amber1, TrafLig_t green1, TrafLig_t red2, TrafLig_t amber2, TrafLig_t green2);
uint8_t get_lights(void);
#endif /* INC_TRAFFIC_LIGHT_H_ */

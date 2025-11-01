/*
 * file:	traffic_light.c
 * brief:	Source file for traffic_light control
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */

/* Private includes ----------------------------------------------------------*/
#include "Inc_modules/traffic_light.h"

#ifdef TRAFFIC_LIGHT_HARDWARE_MODULE
#	define RED_0		((uint8_t) 0x0003)
#	define AMBER_0		((uint8_t) 0x0001)
#	define GREEN_0		((uint8_t) 0x0002)
#	define RED_1		((uint8_t) 0x0300)
#	define AMBER_1		((uint8_t) 0x0100)
#	define GREEN_1		((uint8_t) 0x0200)
#else
#	define NUM_PIN		6
#endif

GPIO_TypeDef* light_port[NUM_PIN] =
{
		REDLIGHT_PORT0,
		AMBERLIGHT_PORT0,
		GREENLIGHT_PORT0,
		REDLIGHT_PORT1,
		AMBERLIGHT_PORT1,
		GREENLIGHT_PORT1
};

uint16_t light_pin[NUM_PIN] =
{
		REDLIGHT_PIN0,
		AMBERLIGHT_PIN0,
		GREENLIGHT_PIN0,
		REDLIGHT_PIN1,
		AMBERLIGHT_PIN1,
		GREENLIGHT_PIN1
};
/* Private code --------------------------------------------------------------*/

static inline void set_pin(uint8_t mask)
{
	for (uint8_t i = 0; i < NUM_PIN; i++)
	{
		HAL_GPIO_WritePin(light_port[i], light_pin[i], (mask & (1 << i)) ? LIGHT_SET : LIGHT_RESET);
	}
}

static inline uint8_t set_mask(uint8_t b5, uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0)
{
	return ((!!b0) << 5) |
	       ((!!b1) << 4) |
	       ((!!b2) << 3) |
	       ((!!b3) << 2) |
	       ((!!b4) << 1) |
	       ((!!b5) << 0);
}

void set_lights(TrafLig_t red1, TrafLig_t amber1, TrafLig_t green1, TrafLig_t red2, TrafLig_t amber2, TrafLig_t green2)
{
	set_pin(set_mask(red1, amber1, green1, red2, amber2, green2));
}


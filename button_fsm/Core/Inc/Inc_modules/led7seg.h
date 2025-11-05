/*
 * @file:	led7seg.h
 * @brief:	7-segment LED header file
 * 			contains API provided for control 7-segment LED
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */

#ifndef INC_LED7SEG_H_
#define INC_LED7SEG_H_

#include "main.h"

#define LED7SEG_COMMON_ANODE_WITH_LOGIC_INVERTER
/**
 * Configuration for LED 7 segment set, reset logic level
 * The default configuration is compatible for common usage of LED 7 segment
 * which is COMMON_ANODE without LOGIC INVERTER and COMMON_CATHODE with LOGIC INVERTER
 * The other usage is COMMON ANODE with LOGIC INVERTER for protection GPIO pins
 */
#ifndef LED7SEG_COMMON_ANODE_WITH_LOGIC_INVERTER
#	define LED7SEG_SET 		0U
#	define LED7SEG_RESET	1U
#else
#	define LED7SEG_SET		1U
#	define LED7SEG_RESET	0U
#endif

#define EN_SET				0U
#define EN_RST				1U

#define SINGLE_LED_CYCLE	10

#define SEG0_PORT SEG0_GPIO_Port
#define SEG1_PORT SEG1_GPIO_Port
#define SEG2_PORT SEG2_GPIO_Port
#define SEG3_PORT SEG3_GPIO_Port
#define SEG4_PORT SEG4_GPIO_Port
#define SEG5_PORT SEG5_GPIO_Port
#define SEG6_PORT SEG6_GPIO_Port

#define SEG0_PIN SEG0_Pin
#define SEG1_PIN SEG1_Pin
#define SEG2_PIN SEG2_Pin
#define SEG3_PIN SEG3_Pin
#define SEG4_PIN SEG4_Pin
#define SEG5_PIN SEG5_Pin
#define SEG6_PIN SEG6_Pin

#define EN0_PORT EN0_GPIO_Port
#define EN1_PORT EN1_GPIO_Port
#define EN2_PORT EN2_GPIO_Port
#define EN3_PORT EN3_GPIO_Port

#define EN0_PIN EN0_Pin
#define EN1_PIN EN1_Pin
#define EN2_PIN EN2_Pin
#define EN3_PIN EN3_Pin
/* Function prototypes -------------------------------------------------------*/
void Led7seg_Init(void);
void display_single_7SEG(uint8_t num);
void display_pair_7SEG(uint8_t num, GPIO_TypeDef* en1_port, uint16_t en1_pin, GPIO_TypeDef* en2_port, uint16_t en2_pin);
void display_2pairs_7SEG(uint8_t num1, uint8_t num2);
#endif /* INC_LED7SEG_H_ */

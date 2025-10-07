/*
 * @file:	led7seg.h
 * @brief:	7-segment LED header file
 * 			contains API provided for control 7-segment LED
 *  Created on: Aug 28, 2025
 *      Author: soaic
 */

#ifndef INC_LED7SEG_H_
#define INC_LED7SEG_H_

/* Refer to hardware for compatible state */
#define LED7SEG_SET 	0U
#define LED7SEG_RESET	1U

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
/* Function prototypes -------------------------------------------------------*/
void Led7seg_Init(void);
void display7SEG(uint8_t num);

#endif /* INC_LED7SEG_H_ */

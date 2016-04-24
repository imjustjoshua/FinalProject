/*
 * LEDDisplay.h
 *
 *  Created on: Mar 31, 2016
 *      Author: Jonathan Lowe
 */

#ifndef PROJECT9_JL4GE_JYG4WX_LEDDISPLAY_H_
#define PROJECT9_JL4GE_JYG4WX_LEDDISPLAY_H_

#include "CORDIC.h"
#include "PWM.h"

/*
 * Define statements that allow you to choose which led to light up.
 * Below shows the diagram for which LED is which on the board:
 *
 *         N
 *     NW     NE
 *  W  			  E
 *     SW     SE
 *         S
 */

#define LED_N		0x80
#define LED_NW		0x1
#define LED_W		0x2
#define LED_SW		0x4
#define LED_S		0x8
#define LED_SE		0x10
#define LED_E		0x20
#define LED_NE		0x40
#define LED_ALL		0xff
#define LED_NONE	0x0

void ConfigureLEDDisplay(void);
void UpdateLEDDisplay(unsigned char data);

#endif /* PROJECT9_JL4GE_JYG4WX_LEDDISPLAY_H_ */

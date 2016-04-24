/*
 * TICom.h
 *
 *  Created on: Apr 23, 2016
 *      Author: Jonathan Lowe
 */

#ifndef TICOM_H_
#define TICOM_H_

/*
 * The port pin used for sending zero.
 */

#define TIZero_BIT			BIT4
#define TIZero_PORT			P1OUT
#define TIZero_DDR			P1DIR
#define TIZero_REN			P1REN

#define SET_TIZero_PULLEN	TIZero_REN |= TIZero_BIT
#define SET_TIZero_PULLDE	TIZero_REN &= ~TIZero_BIT
#define SET_TIZero_PULLUP	TIZero_PORT |= TIZero_BIT

#define SET_TIZero_OUTPUT	TIZero_DDR |= TIZeroPortBit
#define SET_TIZero_INPUT	TIZero_DDR &= ~TIZeroPortBit
#define SET_TIZero_LOW		TIZero_PORT	&= ~TIZeroPortBit
#define SET_TIZero_HIGH		TIZero_PORT |= TIZeroPortBit

/*
 * The port pin used for sending one.
 */

#define TIOne_BIT			BIT7
#define TIOne_PORT			P1OUT
#define TIOne_DDR			P1DIR
#define TIOne_REN			P1REN

#define SET_TIOne_PULLEN	TIOne_REN |= TIOne_BIT
#define SET_TIOne_PULLDE	TIOne_REN &= ~TIOne_BIT
#define SET_TIOne_PULLUP	TIOne_PORT |= TIOne_BIT

#define SET_TIOne_OUTPUT	TIOne_DDR |= TIOnePortBit
#define SET_TIOne_INPUT		TIOne_DDR &= ~TIOnePortBit
#define SET_TIOne_LOW		TIOne_PORT &= ~TIOnePortBit
#define SET_TIOne_HIGH		TIOne_PORT |= TIOnePortBit

typedef enum {
	Low, High
} TIPortState;

void TIInitializePins(void);
void TISendByte(unsigned char data);
void TISendList(unsigned int * data[]);

#endif /* TICOM_H_ */

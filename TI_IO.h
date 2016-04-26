/*
 * TICom.h
 *
 *  Created on: Apr 23, 2016
 *      Author: Jonathan Lowe
 */

#ifndef TI_IO_H_
#define TI_IO_H_

#include <msp430.h>

/*
 * The port pin used for sending zero.
 */

#define TIZero_BIT			BIT4
#define TIZero_PORT			P1OUT
#define TIZero_PORT_PTR		(char*) &TIZero_PORT
#define TIZero_DDR			P1DIR
#define TIZero_REN			P1REN

#define SET_TIZero_PULLEN	TIZero_REN |= TIZero_BIT
#define SET_TIZero_PULLDE	TIZero_REN &= ~TIZero_BIT
#define SET_TIZero_PULLUP	TIZero_PORT |= TIZero_BIT

#define SET_TIZero_OUTPUT	TIZero_DDR |= TIZero_BIT
#define SET_TIZero_INPUT	TIZero_DDR &= ~TIZero_BIT
#define SET_TIZero_LOW		TIZero_PORT	&= ~TIZero_BIT
#define SET_TIZero_HIGH		TIZero_PORT |= TIZero_BIT

/*
 * The port pin used for sending one.
 */

#define TIOne_BIT			BIT7
#define TIOne_PORT			P1OUT
#define TIOne_PORT_PTR		(char*) &TIOne_PORT
#define TIOne_DDR			P1DIR
#define TIOne_REN			P1REN

#define SET_TIOne_PULLEN	TIOne_REN |= TIOne_BIT
#define SET_TIOne_PULLDE	TIOne_REN &= ~TIOne_BIT
#define SET_TIOne_PULLUP	TIOne_PORT |= TIOne_BIT

#define SET_TIOne_OUTPUT	TIOne_DDR |= TIOne_BIT
#define SET_TIOne_INPUT		TIOne_DDR &= ~TIOne_BIT
#define SET_TIOne_LOW		TIOne_PORT &= ~TIOne_BIT
#define SET_TIOne_HIGH		TIOne_PORT |= TIOne_BIT

#define SEND_TIMEOUT		20 // Timeout should be 250 us -- double check

typedef enum {
	Low, High
} TIPortState;

void TIInitializePins(void);
void TISendPacket(unsigned char * header, unsigned char * data, unsigned int dataLength);
void TIReceivePacket(unsigned char * header, unsigned char * data, unsigned int * dataLength, unsigned int maxLength);

#endif /* TI_IO_H_ */

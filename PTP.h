/*
 * PTP.h
 *
 *  Created on: May 9, 2016
 *      Author: Jonathan Lowe
 */

#ifndef PTP_H_
#define PTP_H_

#define SLAVE 0
#define MASTER 1

unsigned char masterSlave;

/*
 * The port pin used for sending zero. (red wire)
 */

#define PTPZero_BIT			BIT0
#define PTPZero_PORT		P2OUT
#define PTPZero_PORT_IN		P2IN
#define PTPZero_DDR			P2DIR
#define PTPZero_REN			P2REN

#define SET_PTPZero_PULLEN	PTPZero_REN |= PTPZero_BIT
#define SET_PTPZero_PULLDE	PTPZero_REN &= ~PTPZero_BIT
#define SET_PTPZero_PULLUP	PTPZero_PORT |= PTPZero_BIT

#define SET_PTPZero_OUTPUT	PTPZero_DDR |= PTPZero_BIT
#define SET_PTPZero_LOW		PTPZero_PORT	&= ~PTPZero_BIT
#define SET_PTPZero_HIGH	PTPZero_PORT |= PTPZero_BIT

#define SET_PTPZero_INPUT	PTPZero_DDR &= ~PTPZero_BIT
#define READ_PTPZero		(PTPZero_PORT_IN & PTPZero_BIT)

/*
 * The port pin used for sending a logic one. (white wire)
 */

#define PTPOne_BIT			BIT2
#define PTPOne_PORT			P2OUT
#define PTPOne_PORT_IN		P2IN
#define PTPOne_DDR			P2DIR
#define PTPOne_REN			P2REN

#define SET_PTPOne_PULLEN	PTPOne_REN |= PTPOne_BIT
#define SET_PTPOne_PULLDE	PTPOne_REN &= ~PTPOne_BIT
#define SET_PTPOne_PULLUP	PTPOne_PORT |= PTPOne_BIT

#define SET_PTPOne_OUTPUT	PTPOne_DDR |= PTPOne_BIT
#define SET_PTPOne_LOW		PTPOne_PORT &= ~PTPOne_BIT
#define SET_PTPOne_HIGH		PTPOne_PORT |= PTPOne_BIT

#define SET_PTPOne_INPUT	PTPOne_DDR &= ~PTPOne_BIT
#define READ_PTPOne			(PTPOne_PORT_IN & PTPOne_BIT)

void PTPDetectMaster(void);
void PTPSendReceive(void);
void PTPInitializePins(void);
unsigned char PTPReceiveByte(void);
void PTPSendByte(unsigned char data);

#endif /* PTP_H_ */

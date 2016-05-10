/*
 * SPI.h
 *
 *  Created on: Feb 16, 2016
 *      Author: student
 */

#ifndef UART_H_
#define UART_H_
#include <msp430.h>
//#define MAJOR_PORT P1OUT
//#define MINOR_PORT P2OUT

#define UART_PORT P2OUT
#define UART_BIT BIT1
#define TURN_ON_UART UART_PORT|=UART_BIT
#define TURN_OFF_UART UART_PORT&=~UART_BIT
#define TOGGLE_UART UART_PORT^=UART_BIT
#define SET_UART_AS_AN_OUTPUT	P2DIR |= UART_BIT

volatile unsigned int sendBuffer;
volatile unsigned int indx;

void ConfigureUART();
void sendUARTByte(unsigned char toSend);
void sendScores(unsigned int us, unsigned int them);
void setCursor(unsigned char row, unsigned char col);
void setSplashScreen();
void setCursorUnreachable();
void clear();
void UARTwin();
void UARTlose();
void resetBaud();
#endif /* UART_H_ */

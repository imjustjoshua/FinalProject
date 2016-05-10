/*
 * UART.c
 *
 *  Created on: May 9, 2016
 *      Author: student
 */
#include "UART.h"
//#include "LED.h"
void ConfigureUART(){
	sendBuffer=0;
	SET_UART_AS_AN_OUTPUT;
	indx=10;
	TURN_ON_UART;
	clear();
	setCursor(0,0);
	sendUARTByte('Y');
	sendUARTByte('o');
	sendUARTByte('u');
	sendUARTByte('r');
	sendUARTByte(' ');
	sendUARTByte('S');
	sendUARTByte('c');
	sendUARTByte('o');
	sendUARTByte('r');
	sendUARTByte('e');
	sendUARTByte(':');
	setCursor(1,0);
	sendUARTByte('T');
	sendUARTByte('h');
	sendUARTByte('e');
	sendUARTByte('i');
	sendUARTByte('r');
	sendUARTByte(' ');
	sendUARTByte('S');
	sendUARTByte('c');
	sendUARTByte('o');
	sendUARTByte('r');
	sendUARTByte('e');
	sendUARTByte(':');
	setCursorUnreachable();
}
void sendUARTByte(unsigned char toSend){
	while(indx<10);
	sendBuffer=toSend<<1;
	indx=0;
}
void sendScores(unsigned int us, unsigned int them){
	setCursor(0,14);
	sendUARTByte(us|'0');
	setCursor(1,14);
	sendUARTByte(them|'0');
	setCursorUnreachable();
}
void setCursorUnreachable(){
	sendUARTByte(254);
	sendUARTByte(128|20);
}
void clear(){
	sendUARTByte(254);
	sendUARTByte(1);
}
void setCursor(unsigned char row, unsigned char col){
	sendUARTByte(254);
	sendUARTByte(128|((row==1)<<6)|col);
}
void setSplashScreen(){
	clear();
	sendUARTByte('J');
	sendUARTByte('o');
	sendUARTByte('s');
	sendUARTByte('h');
	sendUARTByte('u');
	sendUARTByte('a');
	sendUARTByte(' ');
	sendUARTByte('R');
	sendUARTByte('o');
	sendUARTByte('y');
	sendUARTByte(124);
	sendUARTByte(10);
}

void UARTwin(){
	clear();
	setCursor(0,4);
	sendUARTByte('Y');
	sendUARTByte('o');
	sendUARTByte('u');
	sendUARTByte(' ');
	sendUARTByte('W');
	sendUARTByte('i');
	sendUARTByte('n');
	sendUARTByte('!');
	setCursorUnreachable();
}
void UARTlose(){
	clear();
	setCursor(0,4);
	sendUARTByte('Y');
	sendUARTByte('o');
	sendUARTByte('u');
	sendUARTByte(' ');
	sendUARTByte('L');
	sendUARTByte('o');
	sendUARTByte('s');
	sendUARTByte('t');
	setCursorUnreachable();
}
void resetBaud(){

}

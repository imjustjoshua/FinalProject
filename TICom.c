/*
 * TICom.c
 *
 *  Created on: Apr 23, 2016
 *      Author: Jonathan Lowe
 */

#include "TICom.h"

/*
 * This initializes the port pins for communicating with the TI calculator.
 */
void TIInitializePins(void) {
	SET_TIZero_INPUT;
	SET_TIOne_INPUT;

	// Sets the port pins for pullup resistors.
	SET_TIOne_HIGH;
	SET_TIZero_HIGH;

	SET_TIOne_PULLEN;
	SET_TIZero_PULLEN;
}

void TIResetPins(void) {

	SET_TIOne_HIGH;
	SET_TIZero_HIGH;

	SET_TIZero_INPUT;
	SET_TIOne_INPUT;

	// Sets the port pins for pullup resistors.
	SET_TIOne_PULLEN;
	SET_TIZero_PULLEN;
}

TIPortState TIGetPortState(char * Port, unsigned char PortPin) {
	if (*Port & PortPin) {
		return High;
	} else {
		return Low;
	}
}


/*
 * This function sends a byte of data to a TI calculator LSB first.
 */
void TISendByte(unsigned char data) {
	int i = 0;

	for (i = 0; i < 8; i++) {
		while((TIGetPortState(TIZero_PORT, TIZero_Bit) == Low) || (TIGetPortState(TIOne_PORT, TIOne_Bit) == Low));

		if (data & 0x1) {
			SET_TIOne_PULLDE;
			SET_TIOne_OUTPUT;
			SET_TIOne_LOW;

			while(TIGetPortState(TIZero_PORT, TIZero_Bit) == High);
			TIResetPins();
			while(TIGetPortState(TIZero_PORT, TIZero_Bit) == Low);

		} else {
			SET_TIZero_PULLDE;
			SET_TIZero_OUTPUT;
			SET_TIZero_LOW;

			while(TIGetPortState(TIOne_PORT, TIZero_Bit) == High);
			TIResetPins();
			while(TIGetPortState(TIOne_PORT, TIZero_Bit) == Low);

		}
		TIResetPins();

		data >>= 1;
	}
}

void TISendList(unsigned int * data[]) {

}

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
 *
 */
void TISendByte(unsigned char data) {
	int i = 0;

	for (i = 0; i < 8; i++) {
		while((TIGetPortState(TIZero_PORT_PTR, TIZero_BIT) == Low) || (TIGetPortState(TIOne_PORT_PTR, TIOne_BIT) == Low));

		if (data & 0x1) {
			SET_TIOne_PULLDE;
			SET_TIOne_OUTPUT;
			SET_TIOne_LOW;

			while(TIGetPortState(TIZero_PORT_PTR, TIZero_BIT) == High); // TODO timeout
			TIResetPins();
			while(TIGetPortState(TIZero_PORT_PTR, TIZero_BIT) == Low); // TODO timeout

		} else {
			SET_TIZero_PULLDE;
			SET_TIZero_OUTPUT;
			SET_TIZero_LOW;

			while(TIGetPortState(TIOne_PORT_PTR, TIZero_BIT) == High); // TODO timeout
			TIResetPins();
			while(TIGetPortState(TIOne_PORT_PTR, TIZero_BIT) == Low); // TODO timeout

		}
		TIResetPins();

		data >>= 1;
	}
}

void TISendPacket(unsigned char * header[], unsigned char * data[], int dataLength) {
	int i = 0;

	// Sends the packet header to the Calculator
	for (i = 0; i < 4; i++) {
		TISendByte(header[i]);
		// TODO Check Error
	}

	unsigned int checkSum = 0;

	// Sends the data to the calculator.
	for (i = 0; i < dataLength; i++) {

		TISendByte(data[i]);
		// TODO Check Error

		checkSum += data[i];
	}


	// Sends the checkSum to the calculator.
	TISendByte(checkSum & 0xFF);
	// TODO Check Error

	TISendByte((checkSum >> 8) & 0xFF);
	// TODO Check Error

}

/*
 *
 */
void TISendList(unsigned char * array[]) {
	unsigned char header[4] = {0x73, 0x06, 0x0D, 0x00};
	unsigned char data[13] = {0x09, 0x00, 0x01, 0x5D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	unsigned char header2[4] = {0x73, 0x15, 0x3F, 0x00};
	unsigned char data2[63] = {};

	for (i = 0; i < 7; i++) {
		TIUnsignedCharToTIReal(data2[9*i], array[i]);
	}

	TISendPacket(&header, &data, 13);

	TISendPacket(&header2, &data2, 63);
}

/*
 * Updates the array[9] array to a real number in TI
 */
void TIUnsignedCharToTIReal(unsigned char * array[], unsigned char data){
	unsigned char d100s = 0;
	unsigned char d10s = 0;
	unsigned char d1s = 0;

	while (data >= 100) {
		d100s += 1;
		data -= 100;
	}

	while (data >= 10) {
		d10s += 1;
		data -= 10;
	}

	d1s = data;

	array[0] = 0x0;
	array[1] = 0x82;
	array[2] = (d100s << 8) | d10s;
	array[3] = (d1s << 8);
	array[4] = 0x0;
	array[5] = 0x0;
	array[6] = 0x0;
	array[7] = 0x0;
	array[8] = 0x0;
}

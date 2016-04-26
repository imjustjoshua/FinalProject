/*
 * TIVar.c
 *
 *  Created on: Apr 25, 2016
 *      Author: Jonathan Lowe
 */

#include "TIVar.h"

/*
 * Updates the array[9] array to a real number in TI
 *
 * Real number in TI are formatted as follows:
 *
 * TODO add table and explenation.
 *
 */
void TIUnsignedCharToTIReal(unsigned char * array, unsigned char data){
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


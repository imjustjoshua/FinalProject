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
void UnsignedCharToTIReal(unsigned char * array, unsigned char data){
	unsigned int d100s = 0;
	unsigned int d10s = 0;
	unsigned int d1s = 0;

	while (data >= 100) {
		d100s += 1;
		data -= 100;
	}

	while (data >= 10) {
		d10s += 1;
		data -= 10;
	}

	while (data >= 1) {
		d1s += 1;
		data -= 1;
	}

	if (d100s > 0) {
		array[0] = 0x0;
		array[1] = 0x82;
		array[2] = (d100s << 4) | d10s;
		array[3] = (d1s << 4);
		array[4] = 0x0;
		array[5] = 0x0;
		array[6] = 0x0;
		array[7] = 0x0;
		array[8] = 0x0;
	} else if (d10s > 0){
		array[0] = 0x0;
		array[1] = 0x81;
		array[2] = (d10s << 4) | d1s;
		array[3] = 0x0;
		array[4] = 0x0;
		array[5] = 0x0;
		array[6] = 0x0;
		array[7] = 0x0;
		array[8] = 0x0;
	} else {
		array[0] = 0x0;
		array[1] = 0x80;
		array[2] = (d1s << 4);
		array[3] = 0x0;
		array[4] = 0x0;
		array[5] = 0x0;
		array[6] = 0x0;
		array[7] = 0x0;
		array[8] = 0x0;
	}
}

void UnsignedIntToTIReal(unsigned char * array, unsigned int data){
	unsigned char d10000s = 0;
	unsigned char d1000s = 0;
	unsigned char d100s = 0;
	unsigned char d10s = 0;
	unsigned char d1s = 0;

	while (data >= 10000) {
		d10000s += 1;
		data -= 10000;
	}

	while (data >= 1000) {
		d1000s += 1;
		data -= 1000;
	}

	while (data >= 100) {
		d100s += 1;
		data -= 100;
	}

	while (data >= 10) {
		d10s += 1;
		data -= 10;
	}

	d1s = data;

	if (d10000s > 0) {
		array[0] = 0x0;
		array[1] = 0x84;
		array[2] = (d10000s << 4) | d1000s;
		array[3] = (d100s << 4) | d10s;
		array[4] = (d1s << 4);
		array[5] = 0x0;
		array[6] = 0x0;
		array[7] = 0x0;
		array[8] = 0x0;
	} else if(d1000s > 0) {
		array[0] = 0x0;
		array[1] = 0x83;
		array[2] = (d1000s << 4) | d100s;
		array[3] = (d10s << 4) | d1s;
		array[4] = 0x0;
		array[5] = 0x0;
		array[6] = 0x0;
		array[7] = 0x0;
		array[8] = 0x0;
	} else if (d100s > 0) {
		array[0] = 0x0;
		array[1] = 0x82;
		array[2] = (d100s << 4) | d10s;
		array[3] = (d1s << 4);
		array[4] = 0x0;
		array[5] = 0x0;
		array[6] = 0x0;
		array[7] = 0x0;
		array[8] = 0x0;
	} else if(d10s > 0) {
		array[0] = 0x0;
		array[1] = 0x81;
		array[2] = (d10s << 4) | d1s;
		array[3] = 0x0;
		array[4] = 0x0;
		array[5] = 0x0;
		array[6] = 0x0;
		array[7] = 0x0;
		array[8] = 0x0;
	} else {
		array[0] = 0x0;
		array[1] = 0x80;
		array[2] = (d1s << 4);
		array[3] = 0x0;
		array[4] = 0x0;
		array[5] = 0x0;
		array[6] = 0x0;
		array[7] = 0x0;
		array[8] = 0x0;
	}
}


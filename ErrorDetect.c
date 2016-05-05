#include "ErrorDetect.h"

	/*
	 * A function that puts an even parity bit on the MSB of a long.
	 */
	unsigned long EvenParitySetup(unsigned long data) {
		unsigned int xor = data & 0x1;
		int counter = 1;

		for(counter = 1; counter < 31; counter++) {
			xor = ((data >> counter) ^ xor);
		}

		if (xor & 0x1) {
			return data | (0x80000000);
		} else {
			return data & (0x7FFFFFFF);
		}
	}

	/*
	 * A function that checks for even parity on the MSB of a long.
	 */
	unsigned int EvenParityCheck(unsigned long data) {
		unsigned int xor = data & 0x1;
		int counter = 1;

		for(counter = 1; counter < 31; counter++) {
			xor = ((data >> counter) ^ xor);
		}

		if ((data >> 31) == (xor & 0x1)) {
			return 1;
		} else {
			return 0;
		}
	}

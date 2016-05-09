/*
 * PTP.c
 *
 *  Created on: May 9, 2016
 *      Author: Jonathan Lowe
 */

#include "PTP.h"
#include "debounce.h"
#include "Game.h"

extern SwitchDefine gPushButton;

void PTPDetectMaster(void) {
	while (READ_PTPZero && READ_PTPOne) {
		if(Debouncer(&gPushButton) == High) {
			masterSlave = MASTER;
			break;
		}
	}

	if (masterSlave == MASTER) {
		PTPSendByte(0x1);
	} else {
		PTPReceiveByte();
	}
}

void PTPSendReceive(void) {
	if(masterSlave == MASTER) {
		PTPSendByte(us.location.X);
		if (gameData & BIT5) {
			PTPSendByte(BIT7 | us.location.Y);
			gameData &= ~BIT5;
		} else {
			PTPSendByte(us.location.Y);
		}

		them.location.X = PTPReceiveByte();
		unsigned char t = PTPReceiveByte();
		them.location.Y = t & 0x7F;
		if (t & BIT7) {
			them.score += SCORE_AMOUNT;
		}
	} else {
		them.location.X = PTPReceiveByte();
		unsigned char t = PTPReceiveByte();
		them.location.Y = t & 0x7F;
		if (t & BIT7) {
			them.score += SCORE_AMOUNT;
		}

		PTPSendByte(us.location.X);
		if (gameData & BIT5) {
			PTPSendByte(BIT7 | us.location.Y);
			gameData &= ~BIT5;
		} else {
			PTPSendByte(us.location.Y);
		}
	}
}

/*
 * This initializes the port pins for communicating with the TI calculator.
 */
void PTPInitializePins(void) {

	// Sets the pins to input mode.
	SET_PTPOne_INPUT;
	SET_PTPZero_INPUT;

	// Sets the port pins for pullup resistors.
	SET_PTPOne_PULLEN;
	SET_PTPZero_PULLEN;

	// Sets the port pins for pullup resistors.
	SET_PTPOne_HIGH;
	SET_PTPZero_HIGH;
}

/*
 * This reinitializes the port pins for use in TISendByte and TIReceiveByte.
 */
void PTPResetPins(void) {

	// Sets the serial pins high; this also sets the resistors to pull up when enabled.
	SET_PTPOne_HIGH;
	SET_PTPZero_HIGH;

	// Sets the pins to input mode.
	SET_PTPOne_INPUT;
	SET_PTPZero_INPUT;

	// Sets the port pins for pullup resistors.
	SET_PTPOne_PULLEN;
	SET_PTPZero_PULLEN;
}

unsigned char PTPReceiveByte(void) {
	unsigned char data = 0;

	int i = 0;

	for (i = 0; i < 8; i++) {

		// Waits until one of the serial lines is pulled down.
		while (READ_PTPZero && READ_PTPOne)
			; // TODO timeout

		// If the line that corrisponds to a logic 1 is selected.
		if (READ_PTPZero && !READ_PTPOne) {

			// Pulls down the opposite line.
			SET_PTPZero_OUTPUT;
			SET_PTPZero_PULLDE;
			SET_PTPZero_LOW;

			// Waits until the first line is released.
			while (!READ_PTPOne)
				; // TODO timeout

			// Releases the line from above.
			PTPResetPins();

			// Adds data transmitted to be returned.
			data >>= 1;
			data |= 0x80;

			// If the line that corrisponds to a logic 0 is selected.
		} else if (!READ_PTPZero && READ_PTPOne) {

			// Pulls down the opposite line.
			SET_PTPOne_OUTPUT;
			SET_PTPOne_PULLDE;
			SET_PTPOne_LOW;

			// Waits until the first line is released.
			while (!READ_PTPZero)
				; // TODO timeout

			// Releases the line from above.
			PTPResetPins();

			// Adds data transmitted to be returned.
			data >>= 1;
		} else if(!READ_PTPZero && !READ_PTPOne) {
			while (1);
		} else {
			while(1);
		}
	}

	// Releases the line from above.
	PTPResetPins();

	// Returns the data received.
	return data;
}

void PTPSendByte(unsigned char data) {
	int i = 0;

	for (i = 0; i < 8; i++) {
		// Waits until both of the serial lines have been released.
		while (!READ_PTPZero || !READ_PTPOne)
			; // TODO timeout

		_delay_cycles(64);

		// If the data to be sent is a logic one, it sends a logic one.
		if (data & 0x1) {

			// Pulls down the line associated with a logic one.
			SET_PTPOne_OUTPUT;
			SET_PTPOne_PULLDE;
			SET_PTPOne_LOW;

			// Waits until the other line is pulled down.
			while (READ_PTPZero)
				; // TODO timeout
			_delay_cycles(16);

			// Releases the first line.
			PTPResetPins();

			// Waits until the other line is released.
			while (!READ_PTPZero)
				; // TODO timeout
			_delay_cycles(16);

			// Preps the next bit to be sent.
			data >>= 1;

			// Releases the line from above.
			PTPResetPins();

			// If the data to be sent is a logic one, it sends a logic zero.
		} else {

			// Pulls down the line associated with a logic zero.
			SET_PTPZero_OUTPUT;
			SET_PTPZero_PULLDE;
			SET_PTPZero_LOW;

			// Waits until the other line is pulled down.
			while (READ_PTPOne)
				; // TODO timeout
			_delay_cycles(16);

			// Releases the first line.
			PTPResetPins();

			// Waits until the other line is released.
			while (!READ_PTPOne)
				; // TODO timeout
			_delay_cycles(16);

			// Preps the next bit to be sent.
			data >>= 1;

			// Releases the line from above.
			PTPResetPins();
		}
	}

	// Releases the line from above.
	PTPResetPins();
}

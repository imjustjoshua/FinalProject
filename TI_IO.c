/*
 * TICom.c
 *
 *  Created on: Apr 23, 2016
 *      Author: Jonathan Lowe
 */

#include "TI_IO.h"
#include "TILinkProtocol.h"

/*
 * This initializes the port pins for communicating with the TI calculator.
 */
void TIInitializePins(void) {

	// Sets the pins to input mode.
	SET_TIOne_INPUT;
	SET_TIZero_INPUT;

	// Sets the port pins for pullup resistors.
	SET_TIOne_PULLEN;
	SET_TIZero_PULLEN;

	// Sets the port pins for pullup resistors.
	SET_TIOne_HIGH;
	SET_TIZero_HIGH;
}

/*
 * This reinitializes the port pins for use in TISendByte and TIReceiveByte.
 */
void TIResetPins(void) {

	// Sets the pins to input mode.
	SET_TIOne_INPUT;
	SET_TIZero_INPUT;

	// Sets the port pins for pullup resistors.
	SET_TIOne_PULLEN;
	SET_TIZero_PULLEN;

	// Sets the serial pins high; this also sets the resistors to pull up when enabled.
	SET_TIOne_HIGH;
	SET_TIZero_HIGH;
}

/*
 * This function sends a byte of data to a TI calculator LSB first.
 *
 * The TI calculators use this method for transmitting via a stero serial cable:
 * Both or the channels are initially high and are about ~3.3 volts (via pull up resistors).
 *
 * First, the sender pulls down one of the lines; the line determines whether the data transmitted is a 1 or 0.
 * Next, the receiver pulls down the other line in acknowledgement.
 * The first line is then released, then the other.
 *
 * The data is trasmitted LSB first.
 *
 * The tip of the cable (red wire) corresponds to a logic zero when pulled down first.
 * The ring of the cable (white wire) corresponds to a logic one when pulled down first.
 *
 */
void TISendByte(unsigned char data) {
	int i = 0;

	for (i = 0; i < 8; i++) {
		// Waits until both of the serial lines have been released.
		while (!READ_TIZero || !READ_TIOne)
			; // TODO timeout

		// If the data to be sent is a logic one, it sends a logic one.
		if (data & 0x1) {

			// Pulls down the line associated with a logic one.
			SET_TIOne_PULLDE;
			SET_TIOne_OUTPUT;
			SET_TIOne_LOW;

			// Waits until the other line is pulled down.
			while (READ_TIZero)
				; // TODO timeout

			// Releases the first line.
			TIResetPins();

			// Waits until the other line is released.
			while (!READ_TIZero)
				; // TODO timeout

			// Preps the next bit to be sent.
			data >>= 1;

			// If the data to be sent is a logic one, it sends a logic zero.
		} else {

			// Pulls down the line associated with a logic zero.
			SET_TIZero_PULLDE;
			SET_TIZero_OUTPUT;
			SET_TIZero_LOW;

			// Waits until the other line is pulled down.
			while (READ_TIOne)
				; // TODO timeout

			// Releases the first line.
			TIResetPins();

			// Waits until the other line is released.
			while (!READ_TIOne)
				; // TODO timeout

			// Preps the next bit to be sent.
			data >>= 1;

		}
	}
}

/*
 * This sends a packet to the calculator.
 *
 * Packets are organized in the following mannor.
 *
 * Offset | Length | Description
 * 0	  | 1 Byte | Machine ID
 * 1	  | 1 Byte | Command ID
 * 2	  | 2 Bytes| Length of data
 * 4	  | n Bytes| Data (if attached)
 * 4 + n  | 2 Bytes| Checksum (if data is attached)
 *
 * **All 2-byte integers are transmitted little-endian Intel-style; this includes the length of the data to be transmitted.
 *
 * The machine ID identifies the machine that is sending the packet.
 *
 * The command ID itentifies the the request or responce that the machine is sending. These are outlined in TILinkProtocol.h
 *
 * The checksum is a 16 bit value that verifies the integrity of the data. It is simply the sum of the data byte.
 *
 */
void TISendPacket(unsigned char * header, unsigned char * data,
		unsigned int dataLength) {
	int i = 0;

	// Sends the packet header to the Calculator. This consists of the machine ID, command ID, and length of data.
	for (i = 0; i < 4; i++) {
		TISendByte(header[i]);
		// TODO Check Error
	}

	// If the command ID in the header does not include data, it skipps the rest.
	if (header[1] == CID_CTS || header[1] == CID_VER || header[1] == CID_ACK
			|| header[1] == CID_ERR || header[1] == CID_RDY
			|| header[1] == CID_SCR || header[1] == CID_EOT) {
		return;
	}

	unsigned int checkSum = 0;

	// Sends the data array to the calculator.
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
 * This function Receives a byte of data to a TI calculator LSB first.
 *
 * The TI calculators use this method for transmitting via a stero serial cable:
 * Both or the channels are initially high and are about ~3.3 volts (via pull up resistors).
 *
 * First, the sender pulls down one of the lines; the line determines whether the data transmitted is a 1 or 0.
 * Next, the receiver pulls down the other line in acknowledgement.
 * The first line is then released, then the other.
 *
 * The data is trasmitted LSB first.
 *
 * The tip of the cable (red wire) corresponds to a logic zero when pulled down first.
 * The ring of the cable (white wire) corresponds to a logic one when pulled down first.
 */
unsigned char TIReceiveByte(void) {
	unsigned char data = 0;

	int i = 0;

	for (i = 0; i < 8; i++) {

		// Waits until one of the serial lines is pulled down.
		while (READ_TIZero && READ_TIOne)
			; // TODO timeout

		// If the line that corrisponds to a logic 1 is selected.
		if (READ_TIZero && !READ_TIOne) {

			// Pulls down the opposite line.
			SET_TIZero_OUTPUT;
			SET_TIZero_PULLDE;
			SET_TIZero_LOW;

			// Waits until the first line is released.
			while (!READ_TIOne)
				; // TODO timeout

			// Releases the line from above.
			TIResetPins();

			// Adds data transmitted to be returned.
			data >>= 1;
			data |= 0x80;

			// If the line that corrisponds to a logic 0 is selected.
		} else if (!READ_TIZero && READ_TIOne) {

			// Pulls down the opposite line.
			SET_TIOne_OUTPUT;
			SET_TIOne_PULLDE;
			SET_TIOne_LOW;

			// Waits until the first line is released.
			while (!READ_TIZero)
				; // TODO timeout

			// Releases the line from above.
			TIResetPins();

			// Adds data transmitted to be returned.
			data >>= 1;
		} else if(!READ_TIZero && !READ_TIOne) {
			while (1);
		} else {
			while(1);
		}
	}

	// Returns the data received.
	return data;
}

/*
 * This receives a packet from the calculator.
 *
 * Packets are organized in the following mannor.
 *
 * Offset | Length | Description
 * 0	  | 1 Byte | Machine ID
 * 1	  | 1 Byte | Command ID
 * 2	  | 2 Bytes| Length of data
 * 4	  | n Bytes| Data (if attached)
 * 4 + n  | 2 Bytes| Checksum (if data is attached)
 *
 * **All 2-byte integers are transmitted little-endian Intel-style; this includes the length of the data to be transmitted.
 *
 * The machine ID identifies the machine that is sending the packet.
 *
 * The command ID itentifies the the request or responce that the machine is sending. These are outlined in TILinkProtocol.h
 *
 * The checksum is a 16 bit value that verifies the integrity of the data. It is simply the sum of the data byte.
 *
 */
void TIReceivePacket(unsigned char * header, unsigned char * data,
		unsigned int * dataLength, unsigned int maxLength) {
	int i = 0;

	// Gets the header for the packet.
	for (i = 0; i < 4; i++) {
		header[i] = TIReceiveByte();
		// TODO timeout
	}

	// If the command ID in the header does not include data, it skipps the rest.
	if (header[1] == CID_CTS || header[1] == CID_VER || header[1] == CID_ACK
			|| header[1] == CID_ERR || header[1] == CID_RDY
			|| header[1] == CID_SCR || header[1] == CID_EOT) {
		return;
	}

	// Checks the length of the data.
	unsigned int length = (header[3] << 8) | header[2];

	// If the length is greater than the maximum length.
	if (length > maxLength) {
		// TODO Implement Error!
	}

	// Updates the length of the data to be received.
	*dataLength = length;
	unsigned int checkSum = 0;

	// Gets all of the data.
	for (i = 0; i < *dataLength; i++) {
		data[i] = TIReceiveByte();
		checkSum += data[i];
		// TODO timeout
	}

	// Gets the checksum.
	unsigned int checkSumReceived = TIReceiveByte();
	checkSumReceived |= TIReceiveByte() << 8;

	// If the checksums do not match.
	if (checkSumReceived != checkSum) {
		// TODO Implement Error!
	}
}

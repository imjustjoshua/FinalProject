/*
 * TISendList.c
 *
 *  Created on: Apr 25, 2016
 *      Author: Jonathan Lowe
 */

#include "TI_IO.h"
#include "TILinkProtocol.h"
#include "TISendData.h"
#include "TIVar.h"

/*
 * This function sends a list to the calculator.
 *
 * T
 *
 *
 * Variable headers consist of the following
 */
void TISendList(unsigned char * charList, unsigned int charListLength) {

	// Sets up the header and data length for the data array.
	unsigned int dataLength = 0x0D;
	unsigned char header[4] = {MID_TI83P84P, CID_RTS, dataLength & 0xFF, dataLength & 0xFF00};

	// Sets up the variable header to be sent.
	unsigned char data[63] = {0};
	// TODO add data either a variable or list

	// Sends a request to send a real list.
	TISendPacket(header, data, dataLength);

	// Gets acknowlegement of request
	TIReceivePacket(header, data, &dataLength, 63);
	if (header[1] != CID_ACK) {
		// TODO Add error
	}

	// Gets clear to send variable
	TIReceivePacket(header, data, &dataLength, 63);
	if (header[1] != CID_CTS) {
		// TODO Add error
	}

	header[1] = CID_ACK;
	// Sends acknowlegement of CTS.
	TISendPacket(header, data, dataLength);


	// TODO Sets up data to be transferred.
	header[1] = CID_ACK;

	// Sends variable Data.
	TISendPacket(header, data, dataLength);

	// Gets acknowlegement of data
	TIReceivePacket(header, data, &dataLength, 63);
	if (header[1] != CID_CTS) {
		// TODO Add error
	}

	// Sets up EOT header.
	header[1] = CID_EOT;

	// Sends end of transmission.
	TISendPacket(header, data, dataLength);
}

/*
 * This function sends a variable to the calculator.
 */
void TISendVar(unsigned char charData, unsigned char targetVar) {
	// Sets up the header and data length for the data array.
	unsigned int dataLength = 0x0B;
	unsigned char header[4] = {MID_CTI82, CID_RTS, dataLength & 0xFF, dataLength & 0xFF00};

	// Sets up the variable header to be sent.
	unsigned char data[13] = {0};
	data[0] = 0x09;
	data[3] = 0x41; // targetVar;// TODO Change

	// Sends a request to send a real list.
	TISendPacket(header, data, dataLength);

	// Gets acknowlegement of request
	TIReceivePacket(header, data, &dataLength, 63);
	if (header[1] != CID_ACK) {
		// TODO Add error
	}

	// Gets clear to send variable
	TIReceivePacket(header, data, &dataLength, 63);
	if (header[1] != CID_CTS) {
		// TODO Add error
	}

	header[0] = MID_CTI82;
	header[1] = CID_ACK;
	header[2] = 0x0;
	header[3] = 0x0;

	// Sends acknowlegement of CTS.
	TISendPacket(header, data, dataLength);

	// Sets up data to be transferred.
	header[0] = MID_CTI82;
	header[1] = CID_DATA;
	header[2] = 0x0A;
	header[3] = 0x0;
	dataLength = 9;
	UnsignedCharToTIReal(data, charData);

	// Sends variable data.
	TISendPacket(header, data, dataLength);

	// Gets acknowlegement of data
	TIReceivePacket(header, data, &dataLength, 63);
	if (header[1] != CID_CTS) {
		// TODO Add error
	}

	// Sets up EOT header.
	header[0] = MID_CTI82;
	header[1] = CID_ACK;
	header[2] = 0x0;
	header[3] = 0x0;

	// Sends end of transmission.
	TISendPacket(header, data, dataLength);

}

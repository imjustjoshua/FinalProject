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
 *
 * IMPLEMENT! maybe...
 */
void TISendList(unsigned char * charList, unsigned int charListLength) {
}

/*
 * This function sends a list of 7 unsigned chars when the calculator funs the GetCalc() function.
 */
void TISendListGetCalc(unsigned char * charList) {
	// Sets up some variables/arrays that will be used in transmission.
	unsigned char header[4] = {0};
	unsigned char data[65] = {0};
	unsigned int dataLength = 0x0B;

	// Receives the request for the target variable.
	TIReceivePacket(header, data, &dataLength, 20);

	// Sets up for the acknowledgment.
	header[0] = MID_TI83;
	header[1] = CID_ACK;
	header[2] = 0x0;
	header[3] = 0x0;
	dataLength = 0;

	// Sends acknowledgment.
	TISendPacket(header, data, dataLength);

	// Sets up the header for sending the variable header.
	header[0] = MID_TI83;
	header[1] = CID_VAR;
	header[2] = 0x0B;
	header[3] = 0x00;

	// Sets up the variable header.
	data[0] = 0x41;
	data[1] = 0x00;
	data[2] = 0x01;
	data[3] = 0x5D;
	data[4] = 0x00;
	data[5] = 0x00;
	data[6] = 0x00;
	data[7] = 0x00;
	data[8] = 0x00;
	data[9] = 0x00;
	data[10] = 0x00;

	dataLength = 0x0B;

	// Sends the variable header back.
	TISendPacket(header, data, dataLength);

	// The calculator acknowledges the variable header.
	TIReceivePacket(header, data, &dataLength, 20);

	// The calculator says that it is ready to receive.
	TIReceivePacket(header, data, &dataLength, 20);

	// Sets up header for acknowlegement.
	header[0] = MID_TI83;
	header[1] = CID_ACK;
	header[2] = 0x00;
	header[3] = 0x00;
	dataLength = 0;

	// Sends ackownlegement.
	TISendPacket(header, data, dataLength);

	// Sets up data to be sent.
	header[0] = MID_TI83;
	header[1] = CID_DATA;
	header[2] = 0x41;
	header[3] = 0x00;

	dataLength = 0x41;

	// Converts the data to be sent into a TI real number.
	data[0] = 0x07;
	data[1] = 0x00;

	int i = 0;
	for (i = 0; i < 7; i++) {
		UnsignedCharToTIReal((data + 9*i + 2), charList[i]);
	}

	// Sends the data.
	TISendPacket(header, data, dataLength);

	// Receives the acknowlegement.
	TIReceivePacket(header, data, &dataLength, 20);
}

/*
 * This function sends a variable to the calculator.
 */
void TISendVar(unsigned char charData, unsigned char targetVar) {
	// Sets up the header and data length for the data array.
	unsigned int dataLength = 0x0B;
	unsigned char header[4] = {MID_CTI83, CID_RTS, dataLength & 0xFF, dataLength & 0xFF00};

	// Sets up the variable header to be sent.
	unsigned char data[11] = {0};
	data[0] = 0x09;
	data[3] = targetVar;

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

	header[0] = MID_CTI83;
	header[1] = CID_ACK;
	header[2] = 0x0;
	header[3] = 0x0;

	// Sends acknowlegement of CTS.
	TISendPacket(header, data, dataLength);

	// Sets up data to be transferred.
	header[0] = MID_CTI83;
	header[1] = CID_DATA;
	header[2] = 0x09;
	header[3] = 0x00;
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
	header[0] = MID_CTI83;
	header[1] = CID_EOT;
	header[2] = 0x0;
	header[3] = 0x0;

	// Sends end of transmission.
	TISendPacket(header, data, dataLength);
}

/*
 * This function sends a variable when the calculator funs the GetCalc() function.
 */
void TISendVarGetCalc(unsigned char charData){

	// Sets up some variables/arrays that will be used in transmission.
	unsigned char header[4] = {0};
	unsigned char data[11] = {0};
	unsigned int dataLength = 0x0B;

	// Receives the request for the target variable.
	TIReceivePacket(header, data, &dataLength, 20);

	// Sets up for the acknowledgment.
	header[0] = MID_TI83;
	header[1] = CID_ACK;
	header[2] = 0x0;
	header[3] = 0x0;
	dataLength = 0;

	// Sends acknowledgment.
	TISendPacket(header, data, dataLength);

	// Sets up the header for sending the variable header.
	header[0] = MID_TI83;
	header[1] = CID_VAR;
	header[2] = 0x0B;
	header[3] = 0x00;

	// Sets up the variable header.
	data[0] = 0x09;
	dataLength = 0x0B;

	// Sends the variable header back.
	TISendPacket(header, data, dataLength);

	// The calculator acknowledges the variable header.
	TIReceivePacket(header, data, &dataLength, 20);

	// The calculator says that it is ready to receive.
	TIReceivePacket(header, data, &dataLength, 20);

	// Sets up header for acknowlegement.
	header[0] = MID_TI83;
	header[1] = CID_ACK;
	header[2] = 0x00;
	header[3] = 0x00;
	dataLength = 0;

	// Sends ackownlegement.
	TISendPacket(header, data, dataLength);

	// Sets up data to be sent.
	header[0] = MID_TI83;
	header[1] = CID_DATA;
	header[2] = 0x09;
	header[3] = 0x00;

	dataLength = 9;

	// Converts the data to be sent into a TI real number.
	UnsignedCharToTIReal(data, charData);

	// Sends the data.
	TISendPacket(header, data, dataLength);

	// Receives the acknowlegement.
	TIReceivePacket(header, data, &dataLength, 20);
}

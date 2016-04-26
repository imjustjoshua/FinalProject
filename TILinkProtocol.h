/*
 * TILinkProtocol.h
 *
 *  Created on: Apr 25, 2016
 *      Author: Jonathan Lowe
 */

#ifndef TILINKPROTOCOL_H_
#define TILINKPROTOCOL_H_

/*
 * These are define statements for header of the data packets.
 */

// MachineIDs for a data packet

#define MID_CTI82 		0x02	// Computer sending to TI-82 data
#define MID_CTI83		0x03	// Computer sending to TI-83 data
#define MID_CTI83P84P	0x23	// Computer sending to TI-83+/TI84+ data
#define MID_TI83P84P	0x73	// TI-83+/TI84+ sending
#define MID_TI82		0x82	// TI-82 sending
#define MID_TI83		0x83	// TI-83 sending

/*
 * CommandIDs for a data packet.
 *
 * Silent - for silent transmissions only.
 */

#define CID_VAR			0x06	// Variable header
#define CID_CTS			0x09	// Clear to send
#define CID_DATA		0x15	// Data packet
#define CID_VER			0x2D	// Silent - Request versions
#define CID_SKIP_EXIT	0x36	// A 5byte reject code
#define CID_ACK			0x56	// Acknowledge
#define CID_ERR			0x5A	// Checksum Error send again
#define CID_RDY			0x68	// Check Ready
#define CID_SCR			0x6D	// Silent - Request Screenshot
#define CID_DEL			0x88	// Silent - Delete Variable
#define CID_EOT			0x92	// End of Transmission
#define CID_REQ			0xA2	// Silent - Request Variable
#define CID_RTS			0xC9	// Silent - Request to Send Variable

// Reject Codes for the CID_SKIP_EXIT CommandID.
#define REJ_EXIT		0x01	// Exit - The entire tranmission has been cancelled.
#define REJ_SKIP		0x02	// Skip - The current variable has been skipped.
#define REJ_OOM			0x03	// Out Of Memory (Silent transmissions only) - The reveiving calculator is out of memory.

/*
 * Variable header definitions
 */

// Variable Type IDs
#define TID_REALN		0x00	// Real Number
#define TID_REALL		0x01	// Real List
#define TID_MATRIX		0x02	// Matrix
#define TID_YVAR		0x03	// Y-Variable
#define TID_STR			0x04	// String
#define	TID_PROG		0x05	// Program
#define TID_EDITPROG	0x06	// Edit-locked Program
#define TID_PIC			0x07	// Picture
#define TID_GDB			0x08	// GDB
#define TID_WINSET		0x0B	// Window Settings (See note below)
#define TID_COMPN		0x0C	// Complex Number
#define TID_COMPL		0x0D	// Complex List
// There are more if you want them. I am not typing them out right now.

#endif /* TILINKPROTOCOL_H_ */

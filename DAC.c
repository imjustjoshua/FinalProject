#include "DAC.h"
#include "SPI.h"

extern int length100;
extern unsigned int array100[224];
extern int length300;
extern unsigned int array300[224];
extern int length500;
extern unsigned int array500[224];
extern int length700;
extern unsigned int array700[32];

DAC aDAC, bDAC, cDAC, dDAC;
extern DAC* arrayDAC[4] = {};

// Configures DAC structs for all of the DACs on the header board.
// Also it puts pointers to the all the DAC structs into an array.
void ConfigureDAC(void){
//	aDAC.DACAddress = DAC_A;
//	aDAC.CurrentArrayIndex = 0;
//	aDAC.ArrayLength = length100;
//	aDAC.ArrayValuesPtr = (unsigned int *) array100;
//
//	bDAC.DACAddress = DAC_B;
//	bDAC.CurrentArrayIndex = 0;
//	bDAC.ArrayLength = length300;
//	bDAC.ArrayValuesPtr = (unsigned int *) array300;
//
//	cDAC.DACAddress = DAC_C;
//	cDAC.CurrentArrayIndex = 0;
//	cDAC.ArrayLength = length500;
//	cDAC.ArrayValuesPtr = (unsigned int *) array500;

	dDAC.DACAddress = DAC_D;
	dDAC.CurrentArrayIndex = 0;
	dDAC.ArrayLength = length300;
	dDAC.ArrayValuesPtr = (unsigned int *) array300;

//	arrayDAC[0] = &aDAC;
//	arrayDAC[1] = &bDAC;
//	arrayDAC[2] = &cDAC;
	arrayDAC[0] = &dDAC;
}

void UpdateDACs(){
	// TURN_ON_LED1;

	int i = 0;
	for (i = 0; i < 1; i++) {
		// Starts the writing process
		DAC_SEND_ENABLE;

		// Retrieves the address, array index, array data, and array length from the DAC object.
		int address = arrayDAC[i]->DACAddress;
		int CurrentArrayIndex = arrayDAC[i]->CurrentArrayIndex;
		unsigned int data = arrayDAC[i]->ArrayValuesPtr[CurrentArrayIndex] ;
		int ArrayLength = arrayDAC[i]->ArrayLength;

		// Sends the opcode, address, and data to the DAC
		SPISendByte(WRITE_DAC | address);
		SPISendByte(data >> 8);
		SPISendByte(data);

		// Ends the writing process
		DAC_SEND_DISABLE;

		// Incraments the array pointer
		CurrentArrayIndex++;
		if (CurrentArrayIndex == ArrayLength) {
			arrayDAC[i]->CurrentArrayIndex = 0;
		} else {
			arrayDAC[i]->CurrentArrayIndex = CurrentArrayIndex;
		}
	}

	// Updates all of the DACs at once, so that the waves stay in phase.
	DAC_SEND_ENABLE;

	SPISendByte(UPDATE_DAC | ALL_DACS);
	SPISendByte(0x0);
	SPISendByte(0x0);

	DAC_SEND_DISABLE;

	// TURN_OFF_LED1;
}

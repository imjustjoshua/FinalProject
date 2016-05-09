#include "DAC.h"

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
	aDAC.DACAddress = DAC_A;
	aDAC.CurrentArrayIndex = 0;
	aDAC.ArrayLength = length100;
	aDAC.ArrayValuesPtr = (unsigned int *) array100;

	bDAC.DACAddress = DAC_B;
	bDAC.CurrentArrayIndex = 0;
	bDAC.ArrayLength = length300;
	bDAC.ArrayValuesPtr = (unsigned int *) array300;

	cDAC.DACAddress = DAC_C;
	cDAC.CurrentArrayIndex = 0;
	cDAC.ArrayLength = length500;
	cDAC.ArrayValuesPtr = (unsigned int *) array500;

	dDAC.DACAddress = DAC_D;
	dDAC.CurrentArrayIndex = 0;
	dDAC.ArrayLength = length700;
	dDAC.ArrayValuesPtr = (unsigned int *) array700;

	arrayDAC[0] = &aDAC;
	arrayDAC[1] = &bDAC;
	arrayDAC[2] = &cDAC;
	arrayDAC[3] = &dDAC;
}


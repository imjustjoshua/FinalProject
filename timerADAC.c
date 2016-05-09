#include "timerADAC.h"

#include "DAC.h"
#include "SPI.h"
//#include "LED.h"

extern DAC* arrayDAC[4];

void ConfigureTimerA(void) {
	// Configures Timer_A -
	// TASSEL_2:System master clock - MC_1:Count up - ID_0:Divides by 1
	TACTL = TASSEL_2 | ID_0 | MC_1 | TACLR;

	// Configures Timer_A Capture/Compare register to enable interrupts
	TACCTL0 = CCIE;
	TACCR0 = 714; // Timer A is triggered at a frequency of 22400 hz
}

#pragma vector = TIMER0_A0_VECTOR
// Timer a interrupt service routine.
__interrupt void TimerA0_routine(void) {
	// TURN_ON_LED1;

	int i = 0;
	for (i = 0; i < 4; i++) {
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

/*
 * The code below is an alternate, faster way to
 * produce the the desired waves in the ISR without DAC structs.
 */

//extern const unsigned int array100[224];
//extern const unsigned int array300[224];
//extern const unsigned int array500[224];
//extern const unsigned int array700[32];
//
//unsigned int arrayPosition = 0;
//unsigned int arrayData = 0;
//
//#pragma vector = TIMER0_A0_VECTOR
//// Timer a interrupt service routine.
//__interrupt void TimerA0_routine(void) {
//	// TURN_ON_LED1;
//
//	arrayData = array100[arrayPosition];
//
//	DAC_SEND_ENABLE;
//
//	SPISendByte(WRITE_UPDATE_DAC | DAC_A);
//	SPISendByte(arrayData >> 8);
//	SPISendByte(arrayData);
//
//	DAC_SEND_DISABLE;
//
//	arrayData = array300[arrayPosition];
//
//	DAC_SEND_ENABLE;
//
//	SPISendByte(WRITE_DAC | DAC_B);
//	SPISendByte(arrayData >> 8);
//	SPISendByte(arrayData);
//
//	DAC_SEND_DISABLE;
//
//	arrayData = array500[arrayPosition];
//
//	DAC_SEND_ENABLE;
//
//	SPISendByte(WRITE_DAC | DAC_C);
//	SPISendByte(arrayData >> 8);
//	SPISendByte(arrayData);
//
//	DAC_SEND_DISABLE;
//
//	arrayData = array700[arrayPosition & 0x1f];
//
//	DAC_SEND_ENABLE;
//
//	SPISendByte(WRITE_UPDATE_ALL_DAC | DAC_D);
//	SPISendByte(arrayData >> 8);
//	SPISendByte(arrayData);
//
//	DAC_SEND_DISABLE;
//
//	arrayPosition++;
//
//	if (arrayPosition == 224) arrayPosition = 0;
//
//	// TURN_OFF_LED1;
//}

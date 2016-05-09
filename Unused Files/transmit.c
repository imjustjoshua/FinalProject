#include "transmit.h"
#include "configure.h"
#include "ErrorDetect.h"

void InitializeTransmit(void) {
	SET_TR_AS_AN_OUTPUT;
	SET_TX_DATA_AS_AN_OUTPUT;
}

//This routine manages the actual transmitter and is called every 500uS by a periodic interrupt.
void Xmit(TransmitterData* TData) {

	enum XmitClockPhase Phase;

	if (TData->Transmit_Clock_Phase == CLKLow) {
		TData->Transmit_Clock_Phase = CLKHigh;
	} else {
		TData->Transmit_Clock_Phase = CLKLow;
	}

	Phase = TData->Transmit_Clock_Phase;

	enum Transmit_States nextState = TData->Transmitter_State;

	// Now, begin the transmitter state machine.
	switch (TData->Transmitter_State) {

	case StartBit:
		switch (Phase) {
		case CLKLow:
			if (TData->Start_Data) {
				SET_TX_DATA_LOW;
			} else {
				SET_TX_DATA_HIGH;
			}
			break;
		case CLKHigh:
			if (TData->Start_Data) {
				SET_TX_DATA_HIGH;
				TData->Start_Data = 0;
			} else {
				SET_TX_DATA_LOW;

				nextState = NormalXmit;

				// Sets up some variables for next transmission.
				TData->Transmit_Data = TData->Transmit_Data_Buffer; // //EvenParitySetup(TData->Transmit_Data_Buffer);
				TData->Bits_Remaining = BITS_IN_TRANSMISSION;
				TData->InterwordTimeout = INTERWORD_DELAY;
				break;
			}
		}
		break;

	case NormalXmit:
		switch (Phase) {
		case CLKLow:
			if (TData->Transmit_Data & 0x80000000) {
				SET_TX_DATA_LOW;
			} else {
				SET_TX_DATA_HIGH;
			}
			break;

		case CLKHigh:
			if (TData->Transmit_Data& 0x80000000) {
				SET_TX_DATA_HIGH;
			} else {
				SET_TX_DATA_LOW;
			}

			TData->Bits_Remaining -= 1;
			TData->Transmit_Data <<= 1;

			if (!TData->Bits_Remaining) {
				nextState = InterWord;
			}
			break;
		}

		break;
	case InterWord:
		switch (Phase) {
		case CLKLow:
			SET_TX_DATA_LOW;
			TData->InterwordTimeout -= 1;

			if (!TData->InterwordTimeout) {
				nextState = StartBit;
				TData->Start_Data = 0x1;
			}
			break;
		case CLKHigh:
			TData->InterwordTimeout -= 1;

			if (!TData->InterwordTimeout) {
				nextState = StartBit;
				TData->Start_Data = 0x1;
			}
			break;
		}

		break;
	default:
		TData->Transmitter_State = StartBit;
		//Other intitialization here.....
		break;
	}

	TData->Transmitter_State = nextState;
}

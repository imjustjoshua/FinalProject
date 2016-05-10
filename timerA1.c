#include "timerA1.h"

#include "UART.h"

//#include "ArrayValues.h"
void ConfigureTimerA1(void)
{

	//Set the length the timer should run for
	TA1CCR0 = 1667;
	//Enable CCIE Flag
	TA1CCTL0 |= CCIE;

	//Set up the timer (Subsystem Master clock, divided by 1, in up mode, clear)
	TA1CTL = TASSEL_2|ID_0|MC_1|TACLR;
//	SET_LED1_AS_AN_OUTPUT;
//	SET_LED2_AS_AN_OUTPUT;
//	TURN_OFF_LED1;
//	TURN_OFF_LED2;

}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void)
{
//	TURN_ON_LED1;
	switch (indx){
		case 0:
//			TURN_ON_LED2;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			if(sendBuffer&0x1!=0)
				TURN_ON_UART;
			else TURN_OFF_UART;
			sendBuffer>>=1;
			indx++;
		break;
		case 9:
			indx++;
		default:
			TURN_ON_UART;
//			TURN_OFF_LED2;
		break;
	}
//	TURN_OFF_LED1;
}

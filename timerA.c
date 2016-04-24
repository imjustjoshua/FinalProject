#include "timerA.h"
#include <msp430.h>

extern unsigned int g250usTimer;

// Configures timerA.
void ConfigureTimerA(void) {
	/*
	 * Configures Timer_A----------------------------------------------
	 * TASSEL_2:System master clock - MC_1:Count up - ID_0:Divides by 8
	 */
	TACTL = TASSEL_2 | ID_3 | MC_1 | OUTMOD_7 | TACLR;

	// Configures Timer_A Capture/Compare register to enable interrupts
	TACCTL0 = CCIE;
	TACCR0 = 500;// 2000 ms is what was initially had. // Timer A is triggered at a frequency of 1 kHz or once every 1 ms
}

// Clears the timer so that the timer interrupt is no longer triggered.
void DeConfigureTimerA(void) {
	TACTL = TACLR;
}

#pragma vector = TIMER0_A0_VECTOR
// Timer a interrupt service routine.
__interrupt void TimerA0_routine(void) {
	// Increments the timer for the software timers.
	g250usTimer++;
}

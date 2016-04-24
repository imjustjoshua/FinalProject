#include "PWM.h"
#include "LEDDisplay.h"
extern unsigned int g250usTimer;

// This function updates the PWMData struct's time stamps for the PWM
void PWMWaitTime(PWMData *wait, measurements *angles) {

	//tilt runs from 0 to 9000, aka 0 to 90 degrees
	unsigned int tilt = angles->tilt;

	/*
	 * Updates the time stamps for the 0th, 1st, and 2nd neighbor LED to the direction of tilt.
	 * The more tilted it is, the longer the LED will stay on.
	 */
	if (tilt < TILT_THRESHOLD) {
		wait->NTime0 = 1 + g250usTimer;
		wait->NTime1 = 1 + g250usTimer;
		wait->NTime2 = 1 + g250usTimer;
	} else if (tilt < 1500) {
		wait->NTime0 = 3 + g250usTimer;
		wait->NTime1 = 2 + g250usTimer;
		wait->NTime2 = 1 + g250usTimer;
	} else if (tilt < 3000) {
		wait->NTime0 = 6 + g250usTimer;
		wait->NTime1 = 2 + g250usTimer;
		wait->NTime2 = 1 + g250usTimer;
	} else if (tilt < 4500) {
		wait->NTime0 = 10 + g250usTimer;
		wait->NTime1 = 3 + g250usTimer;
		wait->NTime2 = 1 + g250usTimer;
	} else if (tilt < 6000) {
		wait->NTime0 = 20 + g250usTimer;
		wait->NTime1 = 5 + g250usTimer;
		wait->NTime2 = 1 + g250usTimer;
	} else if (tilt < 7500) {
		wait->NTime0 = 30 + g250usTimer;
		wait->NTime1 = 8 + g250usTimer;
		wait->NTime2 = 1 + g250usTimer;
	} else if (tilt <= 9000) {
		wait->NTime0 = 40 + g250usTimer;
		wait->NTime1 = 10 + g250usTimer;
		wait->NTime2 = 1 + g250usTimer;
	}

	// Updates the PWMPeriod time stamp
	wait->PWMPeriodTime = wait->PWMPeriod + g250usTimer;
}

// Updates the LED neighbors to point directly in the direction of tilt.
void PointDirection(measurements *measurement, PWMData *wait) {

	unsigned int angle = measurement->angle;

	if ((angle < 2250) || (33750 <= angle && angle < 36000)) {
		// Direction N
		wait->N0 = LED_N;
		wait->N1 = LED_NW | LED_NE;
		wait->N2 = LED_W | LED_E;
	} else if (2250 <= angle && angle < 6750) {
		// Direction NE
		wait->N0 = LED_NE;
		wait->N1 = LED_N | LED_E;
		wait->N2 = LED_NW | LED_SE;
	} else if (6750 <= angle && angle < 11250) {
		// Direction E
		wait->N0 = LED_E;
		wait->N1 = LED_NE | LED_SE;
		wait->N2 = LED_N | LED_S;
	} else if (11250 <= angle && angle < 15750) {
		// Direction SE
		wait->N0 = LED_SE;
		wait->N1 = LED_E | LED_S;
		wait->N2 = LED_NE | LED_SW;
	} else if (15750 <= angle && angle < 20250) {
		// Direction S
		wait->N0 = LED_S;
		wait->N1 = LED_SW | LED_SE;
		wait->N2 = LED_W | LED_E;
	} else if (20250 <= angle && angle < 24750) {
		// Direction SW
		wait->N0 = LED_SW;
		wait->N1 = LED_W | LED_S;
		wait->N2 = LED_NW | LED_SE;
	} else if (24750 <= angle && angle < 29250) {
		// Direction W
		wait->N0 = LED_W;
		wait->N1 = LED_NW | LED_SW;
		wait->N2 = LED_N | LED_S;
	} else if (29250 <= angle && angle < 33750) {
		// Direction NW
		wait->N0 = LED_NW;
		wait->N1 = LED_N | LED_W;
		wait->N2 = LED_NE | LED_SW;
	}
}

/*
 * This is how the PWM FSM works:
 *
 * State/Input/Timer	= Short Hand : Description
 * -----------------------------------------------
 *
 * States---------
 * PWMInitialize 		= IN		 : LEDs are turned on
 * PWM2ndNOff			= 2O		 : LEDs 2nd neighbors are turned on
 * PWM1ndNOff 			= 1O		 : LEDs 1nd neighbors are turned on
 * PWM0stNOff 			= 0O		 : LEDs 0th neighbors are turned on
 * PWMWaitPWMPeriod		= WP		 : Waits for the end of the PWM Period
 * PWMAllLEDs			= AL		 : All of the LEDs are turned on
 *
 * Present State		= PS		 : The button's present state
 * Next State			= NS		 : The button's next state
 *
 * Inputs/Timer---
 * TimeStampX			= TX		 : The time stamp for x
 * g250usTimer	 		= TI		 : The global 250us timer
 * PQMPeriodTimeStamp	= TP		 : The PWM period time stamp
 * Tilt					= TT		 : The tilt angle
 * Tilt Threshold		= TH		 : The tilt threshold for level
 *
 * Not applicable		= NA		 : It should not happen
 *
 * FSM Table for the PWM.
 *
 *						  T0 != TP									   T0 == TP
 *			   TT < TH				 TT >= TH			    TT < TH				  TT >= TH
 * PS  | TI == TX   TI != TX | TI == TX    TI != TX | TI == TX   TI != TX | TI == TX    TI != TX |
 * -----------------------------------------------------------------------------------------------
 * IN  |    AL        AL	 |    2O          2O	|    AL        AL	  |    2O          2O	 |
 * 2O  |    1O        2O 	 |    1O          2O	|    1O        2O 	  |    1O          2O	 |
 * 1O  |    0O        1O	 |    0O		  1O	|    0O        1O	  |    0O		   1O	 |
 * 0O  |    WP        0O	 |    WP	      0O	|    IN        0O	  |    IN	       0O	 |
 * WP  |    IN        WP	 |    IN          WP	|    IN        WP	  |    IN          WP	 |
 * AL  |    IN        AL	 |    IN          AL	|    IN        AL	  |    IN          AL	 |
 *
 */

// This function Operates the PWM FSM
void PWM(PWMData *wait, measurements *angles) {
	// Gets the current state and initializes the next state
	PWMState CurrentState = wait->PWMState;
	PWMState NextState = CurrentState;

	/*
	 * Next, based on the input values and the current state, determine the next state.
	 */
	switch (CurrentState) {
	case PWMInitialize:
		// Updates the time stamps for the 0th, 1st, and 2nd neighbor LEDs
		PWMWaitTime(wait, angles);

		// If the tilt angle is less than the threshold, it goes to the PWMAll State.
		if (angles->tilt < TILT_THRESHOLD) {
			NextState = PWMAllLEDs;

			// It turns on all of the LEDs
			UpdateLEDDisplay(LED_ALL);
		} else {
			// Otherwise it updates which LED is the 0th, 1st, and 2nd neighbor LEDs
			PointDirection(angles, wait);

			// Then it lights up the 0th, 1st, and 2nd neighbor LEDs and goes to the PWM2ndNOff state.
			UpdateLEDDisplay(wait->N0 | wait->N1 | wait->N2);

			NextState = PWM2ndNOff;
		}
		break;

	case PWM2ndNOff:
		/*
		 * Waits until the the global timer reaches the NTime2 (Neighbor2)
		 * timestamp and then turns of the N2 LEDs and goes to the PWM1stNOff state.
		 */
		if (g250usTimer == wait->NTime2) {
			UpdateLEDDisplay(wait->N0 | wait->N1);
			NextState = PWM1stNOff;
		}

		break;

	case PWM1stNOff:
		/*
		 * Waits until the the global timer reaches the NTime1 (Neighbor1)
		 * timestamp and then turns of the N1 LEDs and goes to the PWM0thNOff state.
		 */
		if (g250usTimer == wait->NTime1) {
			UpdateLEDDisplay(wait->N0);
			NextState = PWM0thNOff;
		}
		break;

	case PWM0thNOff:
		/*
		 * Waits until the the global timer reaches the NTime0 (Neighbor0)
		 * timestamp and then if the the NTimer0 timestamp does not match the
		 * PWMWaitPeriod timestamp, it turns of the N0 LEDs and goes to the PWMWaitPeriod state.
		 * Otherwise, it goes to the PWMInitialize state.
		 */
		if (g250usTimer == wait->NTime0) {
			UpdateLEDDisplay(LED_NONE);
			if (wait->PWMPeriodTime == wait->NTime0) {
				NextState = PWMInitialize;
			} else {
				NextState = PWMWaitPWMPeriod;
			}
		}
		break;

	case PWMWaitPWMPeriod:
		/*
		 * Waits until the the global timer reaches the PWMPeriodTime
		 * timestamp and then goes to the PWMInitialize state.
		 */
		if (g250usTimer == wait->PWMPeriodTime) {
			NextState = PWMInitialize;
		}
		break;

	case PWMAllLEDs:
		/* If the tilt angle is no longer less than the threshold, it goes to the Initialize state.
		 * Otherwise it waits until the the global timer reaches the one of the neighbor
		 * timestamps and then goes to the PWMWaitPWMPeriod state.
		 */
		if (g250usTimer == wait->NTime2) {
			UpdateLEDDisplay(LED_NONE);
			NextState = PWMWaitPWMPeriod;
		}
		break;

	default:
		// Should never happen. It automatically goes to the PWMInitialize state.
		NextState = PWMInitialize;
	}

	// Updates the FSM State.
	wait->PWMState = NextState;
}

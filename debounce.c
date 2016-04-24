#include "debounce.h"

extern unsigned int g250usTimer;

// Function that initializes the switch
void InitializeSwitch(SwitchDefine *Switch, char *SwitchPort,
		char *SwitchPortDIR, char *SwitchPortREN, char *SwitchPortOUT,
		unsigned char SwitchBit, unsigned int HighThresholdTime,
		unsigned int LowThresholdTime, SwitchStatus ActiveState) {

	// Copies the relevent input values to the Switch Struct
	Switch->SwitchPort = SwitchPort;
	Switch->SwitchPortDIR = SwitchPortDIR;
	Switch->SwitchPortREN = SwitchPortREN;
	Switch->SwitchPortOUT = SwitchPortOUT;
	Switch->SwitchPortBit = SwitchBit;
	Switch->TimeStamp = 0;
	Switch->HighThresholdTime = HighThresholdTime;
	Switch->LowThresholdTime = LowThresholdTime;
	Switch->ActiveState = ActiveState;
	Switch->ValidatedState = Low;
	Switch->CurrentState = DbExpectHigh;

	// Initializes the port pins associated with the button.
	if (ActiveState == High) {
		*SwitchPortDIR &= ~SwitchBit; 	// Sets the button as an input
		*SwitchPortREN |= SwitchBit; 	// Enables pull-up/down resistor
		*SwitchPortOUT &= ~SwitchBit; 	// Sets the resistor to pull-down
	} else {
		*SwitchPortDIR &= ~SwitchBit; 	// Sets the button as an input
		*SwitchPortREN |= SwitchBit; 	// Enables pull-up/down resistor
		*SwitchPortOUT |= SwitchBit; 	// Sets the resistor to pull-up
	}

}

// Reinitializes the port pin for the switch based on a switch struct.
void ReInitializeSwitchPin(SwitchDefine *Switch) {
	unsigned char SwitchBit = Switch->SwitchPortBit;

	// Initializes the port pins associated with the button.
	if (Switch->ActiveState == High) {
		*Switch->SwitchPortDIR &= ~SwitchBit;	// Sets the button as an input
		*Switch->SwitchPortREN |= SwitchBit;	// Enables pull-up/down resistor
		*Switch->SwitchPortOUT &= ~SwitchBit;	// Sets the resistor to pull-down
	} else {
		*Switch->SwitchPortDIR &= ~SwitchBit;	// Sets the button as an input
		*Switch->SwitchPortREN |= SwitchBit;	// Enables pull-up/down resistor
		*Switch->SwitchPortOUT |= SwitchBit;	// Sets the resistor to pull-up
	}
}

// Gets the Switch's State either a Logic 1 (High) or 0 (Low).
SwitchStatus GetSwitch(SwitchDefine *Switch) {
	if (*Switch->SwitchPort & Switch->SwitchPortBit) {
		return High;
	} else {
		return Low;
	}
}

/*
 * Button Debouncing FSM
 *
 * State/Input/Timer	= Short Hand : Description
 * ---------------------------------
 *
 * States---------
 * DbExpectHigh 		= EH		 : Button is inactive
 * DbValidateHigh 		= VH		 : Button is inactive but receiving an input that could make it active
 * DbExpectLow 			= EL		 : Button is active
 * DbValidateLow 		= VL		 : Button is active but receiving an input that could make it inactive
 * Present State		= PS		 : The button's present state
 * Next State			= NS		 : The button's next state
 *
 * Inputs/Timer---
 * SwitchState 			= I			 : The Switch's State
 * g1msTimer	 		= TS		 : The global 1ms timer
 * TimeStamp + Period 	= TP		 : The time required to validate a input High or Low
 *
 * The FSM for debouncing acts as follows:
 *
 * Initially the FSM is in the EH state. The EH state is when the button is not being pressed and waiting for a press.
 * It remains there until the input enters the active state. When the input enters the active state, the FSM enters the VH state.
 * The VH state validates the input is not a button bounce by checking to see if the input remains active for a designated period of time.
 * If it does not remain active for the requisite amount of time it returns to the EH state. If it does remain active for the requisite
 * amount of time, it transitions to the EL state. The EL state is when the button is being pressed and waiting till it is not pressed.
 * It remains there until the input enters the inactive state. When the input enters the inactive state, the FSM enters the VL state.
 * The VL state validates the button release is not a button bounce by checking to see if the input remains inactive for a designated period of time.
 * If it does not remain inactive for the requisite amount of time it returns to the EL state. If it does remain inactive for the requisite
 * amount of time, it transitions to the EH state.
 *
 * FSM Table for the Button Debounce.
 *
 *          NS I = LOW           NS I = LOW		   State In LEDs
 * PS | TS != TP  TS == TP | TS != TP  TS == TP |  RED     Green |
 * -------------------------------------------------------------
 * EH |    EH        EH    |    VH        VH    |   0        0   |
 * VH |    EH        EH    |    VH        EL    |   0        1   |
 * EL |    VL        VL    |    EL        EL    |   1        0   |
 * VL |    VL        EH    |    EL        EL    |   1        1   |
 *
 */

/*
 * Returns the updates the current switch state and returns the current validated state of the switch.
 * It returns High if the button is active and Low if the button is inactive.
 */
SwitchStatus Debouncer(SwitchDefine *Switch) {
	// First, determine the current inputs, the current state, active state, and validated state.
	SwitchStatus CurrentSwitchReading = GetSwitch(Switch);
	DbState NextState = Switch->CurrentState;
	SwitchStatus SwitchActiveState = Switch->ActiveState;
	SwitchStatus ValidatedState = Switch->ValidatedState;

	/*
	 * Next, based on the input values and the current state, determine the next state. It also updates the timestamp
	 * whether the switch is active, and the total number of completed button presses.
	 */
	switch (Switch->CurrentState) {
	case DbExpectHigh:
		// If the current input is active, it goes to the DbValidateHigh and updates the timeStamp. Otherwise it stays in it's current state.
		if (CurrentSwitchReading == SwitchActiveState) {
			NextState = DbValidateHigh;
			Switch->TimeStamp = g250usTimer + Switch->HighThresholdTime;
		}
		break;
	case DbValidateHigh:
		/*
		 * Checks to see if the Switch is active. If it is not it returns to DbExpectLow state.
		 * Otherwise it checks if the the required amount of time has passed.
		 * If it has, it goes to the DbExpectLow state, otherwise it remains in its current state.
		 */
		if (CurrentSwitchReading == SwitchActiveState) {
			if (Switch->TimeStamp == g250usTimer) {
				NextState = DbExpectLow;
				ValidatedState = High;
			}
		} else {
			NextState = DbExpectHigh;
		}
		break;
	case DbExpectLow:
		// If the current input is inactive it goes to the DbValidateLow and updates the timeStamp. Otherwise it stays in it's current state.
		if (CurrentSwitchReading != SwitchActiveState) {
			NextState = DbValidateLow;
			Switch->TimeStamp = g250usTimer + Switch->LowThresholdTime;
		}
		break;
	case DbValidateLow:
		/*
		 * Checks to see if the Switch is inactive. If it is not it returns to DbExpectHigh state.
		 * Otherwise it checks if the the required amount of time has passed.
		 * If it has, it goes to the DbExpectHigh state, otherwise it remains in its current state.
		 */
		if (CurrentSwitchReading != SwitchActiveState) {
			if (Switch->TimeStamp == g250usTimer) {
				NextState = DbExpectHigh;
				ValidatedState = Low;
			}
		} else {
			NextState = DbExpectLow;
		}
		break;
	default:
		NextState = DbExpectHigh;
	}

	// Finally, update the current state with the next state and updates the switch's validated state.
	Switch->CurrentState = NextState;
	Switch->ValidatedState = ValidatedState;

	// Returns the Switch's curent validated state (Either High or Low).
	return ValidatedState;
}

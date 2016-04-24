#ifndef FSM_DEBOUNCE_H_
#define FSM_DEBOUNCE_H_

#include <msp430.h>

// Definitions for debounce Times for the P1.3 button (ms)
#define HIGH_THRESHOLD 40
#define LOW_THRESHOLD 80

// Type Definitions
typedef enum {
	DbExpectHigh, DbValidateHigh, DbExpectLow, DbValidateLow
} DbState;

typedef enum {
	Low, High
} SwitchStatus;

/*
 * This structure should encapsulate all of the information needed for
 * a switch.  As such, You will NEED TO insert more definitions in this struct.
 * For example, you should include values for your High/Low threshold delay times.
 */
typedef struct {
	DbState CurrentState;			// Current state of the FSM
	char * SwitchPort;				// Input port associated with switch
	char * SwitchPortDIR;			// Port for setting the direction of the Switch as an input
	char * SwitchPortREN;			// Port for enabling the pull-up/pull-down resistor for the Switch
	char * SwitchPortOUT;			// Port for setting the pull-up/pull-down resistor to pull-up/pull-down
	unsigned char SwitchPortBit;	// Port pin associated with switch
	unsigned int TimeStamp;			// Time stamp used to time how long until the input is validated
	unsigned int HighThresholdTime;	// The amount of time in ms to register a valid high
	unsigned int LowThresholdTime;	// The amount of time in ms to register a valid low
	SwitchStatus ActiveState;		// Indicates whether the button is active high or low
	SwitchStatus ValidatedState;	// The switch's current active state
} SwitchDefine;

// Function Prototypes
void InitializeSwitch(SwitchDefine *Switch, char *SwitchPort,
		char *SwitchPortDIR, char *SwitchPortREN, char *SwitchPortOUT,
		unsigned char SwitchBit, unsigned int HighThresholdTime,
		unsigned int LowThresholdTime, SwitchStatus ActiveState);

//This function returns the instantaneous value of the selected switch
SwitchStatus GetSwitch(SwitchDefine *Switch);

//This function debounces a switch input
SwitchStatus Debouncer(SwitchDefine *Switch);

#endif

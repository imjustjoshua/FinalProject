#include <msp430.h>
#include "SPI.h"
#include "filter.h"
#include "ADC.h"
#include "CORDIC.h"
#include "debounce.h"
#include "timerA.h"
#include "calibrate.h"

#include "TI_IO.h"
#include "TISendData.h"

#include "Game.h"
#include "PTP.h"

// Function Prototypes
void ConfigureClockModule();
void InitializeGlobalVariables(void);

SwitchDefine gPushButton;	// Structure to contain all info for the P1.3 button.
unsigned int g250usTimer;		// Global timer

volatile unsigned int bufferPointer;
volatile unsigned int circularBuffer[3][8];
volatile unsigned int runningSum[3];
volatile unsigned int average[3];			// x, y, z Different from samples.
volatile unsigned int sample[8];			// Z, Y, X !!!!!!!!!!!!!!!The ADC Multi channel works this way!!!!!!!!!!!!!!!!!!!!!!
volatile unsigned int calibratedMAX[3];		// x, y, z
volatile unsigned int calibratedMIN[3];		// x, y, z
volatile unsigned int calibratedZero[3];	// x, y, z

coordinates xyzCoordinates;
measurements angles;

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	ConfigureClockModule();

	// Initializes the game variables
	initializeGame();

	// Initializes the port pins for the TI communication.
	TIInitializePins();

	// Initializes the port pins used for the P2P communicaition.
	PTPInitializePins();

	// Configures the ADC
	ConfigureADC();

	// Configures the port pins for SPI
	ConfigureSPI();

	// Initializes global variables
	InitializeGlobalVariables();

	// Interrupts Globally Enabled
	_BIS_SR(GIE);

	// Runs Calibration
	Calibrate();

	// Detects which sends first
	PTPDetectMaster();

	// Loop forever
	while (1) {
		// Gets new samples and passes them through a circular buffer
		GetFilteredSample();

		// Turns the readings into coordinates using CORDIC
		setCoordinates(&xyzCoordinates, average[0] - calibratedZero[0], average[1] - calibratedZero[1], average[2] - calibratedZero[2]);

		// Takes the coordinates and turns them into the tilt and x-y plane angles.
		angles = getDisplaySetting(&xyzCoordinates);

		updateLocationOnTilt();
		targetCheck();

		// Communicates with the other MSP and get their location and scored.
		PTPSendReceive();

		// Checks for the end game condition
		if (us.score >= SCORE_GOAL || them.score >= SCORE_GOAL) {
			// Sends if you won or not.
			if (us.score > them.score) {
				us.location.Y = 1;
			} else {
				us.location.Y = 0;
			}
			updateDisplay(0x0);
		}

		updateDisplay(0x1);
	}
}

void ConfigureClockModule() {
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}

// Initializes the global variables
void InitializeGlobalVariables(void) {
	bufferPointer = 0;
	g250usTimer = 0;

	masterSlave = SLAVE;
	int i;
	int j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 8; j++) {
			circularBuffer[i][j] = 0;
		}
		runningSum[i] = 0;
		average[i] = 0;
		sample[i] = 0;
		calibratedMAX[i] = 0;
		calibratedMIN[i] = 0;
		calibratedZero[i] = 0;
	}

	setCoordinates(&xyzCoordinates, 0, 0, 0);
	angles.angle = 0;
	angles.tilt = 0;

	InitializeSwitch(&gPushButton, (char *) &P1IN, (char *) &P1DIR,
				(char *) &P1REN, (char *) &P1OUT, (unsigned char) BIT3,
				LOW_THRESHOLD, HIGH_THRESHOLD, Low);
}

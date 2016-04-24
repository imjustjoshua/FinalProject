#include <msp430.h>
#include "SPI.h"
#include "filter.h"
#include "ADC.h"
#include "LEDDisplay.h"
#include "CORDIC.h"
#include "debounce.h"
#include "timerA.h"
#include "calibrate.h"
#include "PWM.h"

// Function Prototypes
void ConfigureClockModule();
void InitializeGlobalVariables(void);

SwitchDefine gPushButton;	// Structure to contain all info for the P1.3 button.
unsigned int g250usTimer;		// Global timer

volatile unsigned int bufferPointer;
volatile unsigned int circularBuffer[3][8];
volatile unsigned int runningSum[3];
volatile unsigned int average[3];			// x, y, z Different from samples.
volatile unsigned int sample[3];			// Z, Y, X !!!!!!!!!!!!!!!The ADC Multi channel works this way!!!!!!!!!!!!!!!!!!!!!!
volatile unsigned int calibratedMAX[3];		// x, y, z
volatile unsigned int calibratedMIN[3];		// x, y, z
volatile unsigned int calibratedZero[3];	// x, y, z

coordinates xyzCoordinates;
measurements angles;
PWMData DisplayPWM;

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	ConfigureClockModule();

	// Configures the ADC
	ConfigureADC();

	// Configures the LED display.
	ConfigureLEDDisplay();

	// Configures the port pins for SPI
	ConfigureSPI();

	// Initializes global variables
	InitializeGlobalVariables();

	// Interrupts Globally Enabled
	_BIS_SR(GIE);

	// Runs Calibration
	Calibrate();

	// Loop forever
	while (1) {
		// Gets new samples and passes them through a circular buffer
		GetSamples();

		// Turns the readings into coordinates using CORDIC
		setCoordinates(&xyzCoordinates, average[0] - calibratedZero[0], average[1] - calibratedZero[1], average[2] - calibratedZero[2]);

		// Takes the coordinates and turns them into the tilt and x-y plane angles.
		angles = getDisplaySetting(&xyzCoordinates);

		// Runs a PWM FSM for the LED display
		PWM(&DisplayPWM, &angles);
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

	DisplayPWM.PWMPeriod = 40;
	DisplayPWM.PWMState = PWMInitialize;
	DisplayPWM.NTime0 = 0;
	DisplayPWM.NTime1 = 0;
	DisplayPWM.NTime2 = 0;
	DisplayPWM.PWMPeriodTime = 0;
	DisplayPWM.N0 = 0;
	DisplayPWM.N1 = 0;
	DisplayPWM.N2 = 0;
}

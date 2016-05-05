#include "calibrate.h"
#include "ADC.h"
#include "LEDDisplay.h"
#include "debounce.h"
#include "timerA.h"

// Function declarations.
void WaitButtonPress(void);

// External variables used
extern unsigned int calibratedMAX[3]; 		// X, Y, Z for all.
extern unsigned int calibratedMIN[3];
volatile unsigned int calibratedZero[3];
extern volatile unsigned int average[3];
extern SwitchDefine gPushButton;

// This function calibrates the accelorometer
void Calibrate(void) {

	// Configures Timers
	ConfigureTimerA();

	// Populates the sample buffers
	GetFilteredSample();

	/*
	 * Determine Xmax (+X direction): Turn on North LED, and wait for pushbutton to be pushed then read sample
	 */
	UpdateLEDDisplay(LED_N);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMAX[0] = average[0];

	/*
	 * Determine Xmin (-X direction): Turn on South LED, and wait for pushbutton to be pushed then read sample
	 */
	UpdateLEDDisplay(LED_S);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMIN[0] = average[0];

	/*
	 * Determine Ymax (+Y direction): Turn on West LED, and wait for pushbutton to be pushed then read sample
	 */
	UpdateLEDDisplay(LED_W);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMAX[1] = average[1];

	/*
	 * Determine Ymin (-Y direction): Turn on East LED, and wait for pushbutton to be pushed then read sample
	 */
	UpdateLEDDisplay(LED_E);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMIN[1] = average[1];

	/*
	 * Determine Zmax (+Z direction): Turn on North, South, East, West LEDs, and
	 * wait for pushbutton to be pushed then read sample
	 */
	UpdateLEDDisplay(LED_N | LED_S | LED_E | LED_W);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMAX[2] = average[2];

	/*
	 * Determine Zmin (-Z direction): Turn on Northeast, Northwest, Southeast,
	 * Southwest LEDs, and wait for pushbutton to be pushed then read sample
	 */
	UpdateLEDDisplay(LED_NW | LED_SW | LED_SE | LED_NE);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMIN[2] = average[2];

	// Deconfigures the timer so the interrupt stops running and taking up time.
	//DeConfigureTimerA();

	UpdateLEDDisplay(0x0);

	int i;

	for (i = 0; i < 3; i++) {
		calibratedZero[i] = (calibratedMAX[i] + calibratedMIN[i]) >> 1;
	}
}

// This function waits until the pushbutton is pressed and released.
void WaitButtonPress(void) {

	while (Debouncer(&gPushButton) != High);
	while (Debouncer(&gPushButton) != Low);

}

#include "calibrate.h"
#include "ADC.h"
#include "debounce.h"
#include "timerA.h"
#include "TISendData.h"

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

	// List for sending to the TI Calc.
	unsigned char listData[7] = {1, 0, 0, 0, 9, 9, 0};


	/*
	 * Determine Xmax (+X direction): Turn on North LED, and wait for pushbutton to be pushed then read sample
	 */
	TISendListGetCalc(listData);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMAX[0] = average[0];

	/*
	 * Determine Xmin (-X direction): Turn on South LED, and wait for pushbutton to be pushed then read sample
	 */
	listData[0] = 2;
	TISendListGetCalc(listData);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMIN[0] = average[0];

	/*
	 * Determine Ymax (+Y direction): Turn on West LED, and wait for pushbutton to be pushed then read sample
	 */
	listData[0] = 3;
	TISendListGetCalc(listData);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMAX[1] = average[1];

	/*
	 * Determine Ymin (-Y direction): Turn on East LED, and wait for pushbutton to be pushed then read sample
	 */
	listData[0] = 4;
	TISendListGetCalc(listData);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMIN[1] = average[1];

	/*
	 * Determine Zmax (+Z direction): Turn on North, South, East, West LEDs, and
	 * wait for pushbutton to be pushed then read sample
	 */
	listData[0] = 5;
	TISendListGetCalc(listData);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMAX[2] = average[2];

	/*
	 * Determine Zmin (-Z direction): Turn on Northeast, Northwest, Southeast,
	 * Southwest LEDs, and wait for pushbutton to be pushed then read sample
	 */
	listData[0] = 6;
	TISendListGetCalc(listData);
	WaitButtonPress();
	GetFilteredSample();
	GetFilteredSample();
	calibratedMIN[2] = average[2];

	listData[0] = 0;
	TISendListGetCalc(listData);

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

#include "SPI.h"
#include "LEDDisplay.h"
#include "PWM.h"

// Configures the port pins necessary for the sending info to the Display.
void ConfigureLEDDisplay(void) {
	ConfigureSPI();
}

/* Updates the LEDs in the display.
 * #define statements are used for specifying the LEDs to display.
 */
void UpdateLEDDisplay(unsigned char data) {
	SPISendByte(data);

	SET_XLAT;
	RESET_XLAT;
}

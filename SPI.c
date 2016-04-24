#include "SPI.h"

// This configures the output pins to
//    communicate with the LED display chip
void ConfigureSPI(void) {

	// Setting up the pins.
	SET_MOSI_DIR;
	SET_SCK_DIR;
	SET_XLAT_DIR;
	SET_BLANK_DIR;

	// Setting the initial values of the outputs.
	RESET_MOSI;
	RESET_SCK;
	RESET_XLAT;
	RESET_BLANK;
}

// This function is used to send an
//    arbitrary byte to the LED display
void SPISendByte(unsigned char byte_value) {

	int i = 0;

	for (i = 0; i < 8; i++) {
		if (byte_value & 0x80) {
			SET_MOSI;
		} else {
			RESET_MOSI;
		}

		byte_value <<= 1;

		SET_SCK;
		RESET_SCK;
	}

}

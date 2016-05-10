#include "SPI.h"

void ConfigureSPI(void){

	UCA0CTL1 |= UCSWRST;

	// Initializing registers
	UCA0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC;
	UCA0BR1 = 0; // Divides clock by 1
	UCA0BR0 = 1;
	UCA0CTL1 |= UCSSEL_2; // Selects SMCLK as the source

	// Configures SPI Port Pins
	SET_MOSI_DIR;
	SET_MISO_DIR;
	SET_SCK_DIR;
	SET_DAC_SELECT_DIR;
	SET_CLR_BIT_DIR;

	CLR_DISABLE;
	// Gives the USI control of the SCK and MOSI pins
	P1SEL2 = MOSI_BIT | SCK_BIT;
	P1SEL = MOSI_BIT | SCK_BIT;

	UCA0CTL1 &= ~UCSWRST;
}

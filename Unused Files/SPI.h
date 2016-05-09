#ifndef PROJECT9_JL4GE_JYG4WX_SPI_H_
#define PROJECT9_JL4GE_JYG4WX_SPI_H_

#include <msp430.h>

#define MOSI_BIT 			BIT7 // Uses port P1OUT for all down.
#define SCK_BIT				BIT5
#define XLAT_BIT			BIT0 // Uses port P2OUT
#define BLANK_BIT			BIT4 // Uses port P1OUT

// Sets the corrisponding directions for the input and output.
#define SET_MOSI_DIR		P1DIR |= MOSI_BIT
#define SET_SCK_DIR			P1DIR |= SCK_BIT
#define SET_XLAT_DIR		P2DIR |= XLAT_BIT
#define SET_BLANK_DIR		P1DIR |= BLANK_BIT

// Set output define statements.
#define SET_MOSI			P1OUT |= MOSI_BIT
#define RESET_MOSI			P1OUT &= ~MOSI_BIT
#define TOGGLE_MOSI			P1OUT ^= MOSI_BIT

#define SET_SCK				P1OUT |= SCK_BIT
#define RESET_SCK			P1OUT &= ~SCK_BIT
#define TOGGLE_SCK			P1OUT ^= SCK_BIT

#define SET_XLAT			P2OUT |= XLAT_BIT
#define RESET_XLAT			P2OUT &= ~XLAT_BIT
#define ENABLE_XLAT			SET_XLAT
#define DISABLE_XLAT		RESET_XLAT

#define SET_BLANK			P1OUT |= BLANK_BIT
#define RESET_BLANK			P1OUT &= ~BLANK_BIT

// Function Prototypesvoid ConfigureSPI(void);
void SPISendByte(unsigned char byte_value);
void ConfigureSPI(void);

#endif /* PROJECT9_JL4GE_JYG4WX_SPI_H_ */

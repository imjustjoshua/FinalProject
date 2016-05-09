#ifndef SPI_H_
#define SPI_H_

#include <msp430.h>

#define MOSI_BIT 					BIT2 // Uses port P1OUT \/.
#define MISO_BIT					BIT1
#define SCK_BIT						BIT4
#define DAC_SELECT_BIT				BIT3
#define CLR_BIT						BIT5

#define SPI_BUSY					UCBUSY & UCA0STAT
#define SPI_SEND_DATA_REGISTER		UCA0TXBUF

// Sets the corrisponding directions for the inputs and outputs.
#define SET_MOSI_DIR		P1DIR |= MOSI_BIT
#define SET_MISO_DIR		P1DIR &= ~MISO_BIT
#define SET_SCK_DIR			P1DIR |= SCK_BIT
#define SET_DAC_SELECT_DIR	P1DIR |= DAC_SELECT_BIT
#define SET_CLR_BIT_DIR		P1DIR |= CLR_BIT

// Clear pin commands
#define SET_CLR				P1OUT |= CLR_BIT
#define RESET_CLR			P1OUT &= ~CLR_BIT
#define CLR_EN				RESET_CLR
#define CLR_DISABLE			SET_CLR

// DAC Chip enable commands
#define SET_DAC_SELECT		P1OUT |= DAC_SELECT_BIT
#define RESET_DAC_SELECT	P1OUT &= ~DAC_SELECT_BIT
#define DAC_SEND_ENABLE		RESET_DAC_SELECT
#define DAC_SEND_DISABLE	SET_DAC_SELECT

// Set output define statements.
#define SET_MOSI			P1OUT |= MOSI_BIT
#define RESET_MOSI			P1OUT &= ~MOSI_BIT
#define TOGGLE_MOSI			P1OUT ^= MOSI_BIT

#define SET_SCK				P1OUT |= SCK_BIT
#define RESET_SCK			P1OUT &= ~SCK_BIT
#define TOGGLE_SCK			P1OUT ^= SCK_BIT

#define SPISendByte( byte_value ) SPI_SEND_DATA_REGISTER = byte_value; while(SPI_BUSY);

// Function Prototypes
void ConfigureSPI(void);

#endif /* SPI_H_ */

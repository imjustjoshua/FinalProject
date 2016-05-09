#include "ADC.h"
#include "filter.h"

// External variables used
extern volatile unsigned int sample[7];

// A function that configures the ADC to read 3 inputs
void ConfigureADC(void)
{
	/*
	 * ADC10 Control register 1
	 * INCH_2 = choose analog input A2 as the last input
	 * ADC10DIV_7 = divide ADC clock by 8
	 * CONSEQ_1 = reads from a Sequence-of-channels
	 * ADC10SSEL_3 = chooses SMCLK as the clock source
	 *
	 * ADC10 Control register 0
	 * SREF_0 = Choose Vcc reference for analog inputs
	 * SREF_1 = Choose VREF reference for analog inputs
	 * REF2_5V = Choose VREF = 2.5v
	 * REFON = Turns on the VREF
	 * MSC = the first rising edge of the SHI signal triggers the sampling timer, but further
	 * 			sample-and-conversions are performed automatically as soon as the prior conversion is completed
	 * ADC10SHT_2 = 16 × ADC10CLKs: 8us
	 * ADC10ON = turns the ADC10 on
	 */
	ADC10CTL1 = (INCH_6 | ADC10DIV_7 | ADC10SSEL_3 | CONSEQ_1);
	ADC10CTL0 = (SREF_1 | MSC | REF2_5V | REFON | ADC10SHT_2 | ADC10ON);

	// Enables analog input pins
	ADC10AE0 |= (ADC10_A0_BIT | ADC10_A1_BIT | ADC10_A2_BIT);

	// Specifies the total number of samples to send using DTC
	ADC10DTC1 = 0x3;

	_delay_cycles(1000);			// Wait for the ADC to adjust and stabilize
	ADC10CTL0 |=  (ENC | ADC10SC);	// Enable and begin sampling and conversion
}

// Gets samples from each of the ADC input pins and runs the filter function.
void GetSamples(void) {
	// Polls the busy bit to check if it is busy
	while (ADC10CTL1 & BUSY);

	// Gives the DTC a memory address to send the data.
	ADC10SA = (int) &(sample);

	// Enables the next conversion
	ADC10CTL0 |= ENC + ADC10SC;

	// Runs samples thruough a circular buffer
	filter();
}

// Gets 8 samples - enough to refresh the average to a new input.
void GetFilteredSample(void) {
	int i = 0;

	for (i = 0; i < 8; i++) {
		GetSamples();
	}
}

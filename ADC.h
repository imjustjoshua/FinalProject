#ifndef ADC_H
#define ADC_H
#include <msp430.h>

// Use the following to select either analog channel 7 or the temperature sensor
#define ADC10_A7 1
#define ADC10_TEMP_SENSOR 0
#define ADC10_MODE ADC10_A7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 ADC10 A7
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   GPIO      :  P1.7
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define ADC10_A0_BIT	BIT0	// Port pin bit location for ADC10 A7
#define ADC10_A1_BIT	BIT1	// Port pin bit location for ADC10 A7
#define ADC10_A2_BIT	BIT2	// Port pin bit location for ADC10 A7

// The following statements are used for defining the sample size array.
#define BUFFER_SIZE 8 // This must be a power of 2

// Prototypes
void ConfigureADC(void);
void GetSamples(void);
void GetFilteredSample(void);

#endif

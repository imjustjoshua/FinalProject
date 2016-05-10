#ifndef DAC_H_
#define DAC_H_

/*
 * DAC Preforms this way
 * 1. The chip enable is driven low
 * 2. The data is sent in the following order: Opcode-Address-Data
 * 3. The chip enable is driven high
 * 4. The DAC executes the command
 *
 */

/* DAC Operation Codes/Commands
 *
 * Write operations write to the DAC's input register.
 * Update operations puts the data in the DAC's input register to its outputting register.
 *
 * WRITE_UPDATE_ALL_DAC writes only to the selected DAC and updates all of the DACS.
 */
#define WRITE_DAC				0x00
#define UPDATE_DAC				0x10
#define WRITE_UPDATE_ALL_DAC	0x20
#define WRITE_UPDATE_DAC		0x30
#define POWER_DOWN_DAC			0x40
#define NO_OPERATION			0xf0

// DAC Addresses
#define DAC_A					0x0
#define DAC_B					0x1
#define DAC_C					0x2
#define DAC_D					0x3
#define ALL_DACS				0xf

// DAC structure for keeping track of DAC variables
typedef struct {
	int DACAddress;
	int CurrentArrayIndex;
	int ArrayLength;
	unsigned int * ArrayValuesPtr;
} DAC;

// Function prototypes
void ConfigureDAC(void);
void UpdateDACs();
#endif

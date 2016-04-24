#include <msp430.h>

extern volatile unsigned int bufferPointer;
extern volatile unsigned int circularBuffer[3][8];
extern volatile unsigned int runningSum[3];
extern volatile unsigned int average[3];
extern volatile unsigned int sample[3];

// Takes the 3 samples and puts them into circular buffers and calculates their average
void filter(void) {

	// Adds to the running sum and updates the buffer
	runningSum[0] += sample[2];
	runningSum[0] -= circularBuffer[0][bufferPointer];
	circularBuffer[0][bufferPointer] = sample[2];

	// Divides the runningSum by 8 to get the average
	average[0] = runningSum[0] >> 3;

	// Adds to the running sum and updates the buffer
	runningSum[1] += sample[1];
	runningSum[1] -= circularBuffer[1][bufferPointer];
	circularBuffer[1][bufferPointer] = sample[1];

	// Divides the runningSum by 8 to get the average
	average[1] = runningSum[1] >> 3;

	// Adds to the running sum and updates the buffer
	runningSum[2] += sample[0];
	runningSum[2] -= circularBuffer[2][bufferPointer];
	circularBuffer[2][bufferPointer] = sample[0];

	// Divides the runningSum by 8 to get the average
	average[2] = runningSum[2] >> 3;

	// bufferPointer = (bufferPointer + 1) % 8;
	bufferPointer = (bufferPointer + 1) & 0x7;

}



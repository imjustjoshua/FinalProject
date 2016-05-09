#ifndef PROJECT9_JL4GE_JYG4WX_PWM_H_
#define PROJECT9_JL4GE_JYG4WX_PWM_H_

#include "CORDIC.h"

typedef enum {
	PWMInitialize, PWM2ndNOff, PWM1stNOff, PWM0thNOff, PWMWaitPWMPeriod, PWMAllLEDs
} PWMState;

typedef struct {
	unsigned int NTime0;				// PWM time stamp for Neighbor X
	unsigned int NTime1;
	unsigned int NTime2;
	unsigned int PWMPeriod;
	unsigned int PWMPeriodTime;
	unsigned char N0;				//
	unsigned char N1;
	unsigned char N2;

	PWMState PWMState;

} PWMData;

void PWMWaitTime(PWMData *wait, measurements *angles);
void PointDirection(measurements *measurement, PWMData *wait);
void PWM(PWMData *wait, measurements *angles);

#define TILT_THRESHOLD 500 // 5 degrees

#endif /* PROJECT9_JL4GE_JYG4WX_PWM_H_ */

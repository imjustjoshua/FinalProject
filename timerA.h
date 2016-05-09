#ifndef TIMER_A_H
#define TIMER_A_H

#include <msp430.h>
#include <msp430.h>
//#include "transmit.h"
//#include "receive.h"

#define TACCR0_VALUE 499

// Prototypes
void ConfigureTimerA(void);
void DeConfigureTimerA(void);
void ConfigureTimerA(void);
void BCSplus_initial(void);
void Timer0_A3_initial(void);
void Timer1_A3_initial(void);


#endif

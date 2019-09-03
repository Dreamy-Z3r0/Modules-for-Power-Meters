/*
 * TIMER.h
 *
 *  Created on: Jul 15, 2019
 *      Author: eeit2
 */

#ifndef DRIVER_TIMER_TIMER_H_
#define DRIVER_TIMER_TIMER_H_

#include "../UART/UART.h"

typedef struct
{
    int COUNT;
} COUNTER_typedef;

extern COUNTER_typedef COUNTER;


void TIMER_0_init(void);
void TIMER_0_stop(void);

#endif /* DRIVER_TIMER_TIMER_H_ */

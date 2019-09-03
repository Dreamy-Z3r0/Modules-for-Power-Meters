/*
 * TIMER.c
 *
 *  Created on: Jul 22, 2019
 *      Author: eeit2
 */

#include "TIMER.h"

unsigned char* txData;                        // UART internal variable for TX
unsigned char* rxBuffer;                     // Received UART character

COUNTER_typedef COUNTER =
{
 .COUNT = 0,
};

void TIMER_0_init(void)
{
    COUNTER.COUNT = 0;

#ifndef TIMER_FIRST_RUN
#define TIMER_FIRST_RUN 1
    TA0CTL |= TACLR;        // Clear configuration for Timer_A0

    TA0CTL |=  TASSEL1;     // Set timer clock source to SMCLK (= 16.384MHz)
    TA0CTL &= ~TASSEL0;

    TA0CTL |= (ID1 | ID0);  // Set input divider for clock source to 8 => CLOCK = SMCLK/8 = 2.048MHz

    TA0CTL |=  MC1;     // Timer_A0 operates in continuous mode
    TA0CTL &= ~MC0;

    TA0CTL |= TAIE;     // Enable Timer_A interrupt
#else
    TA0CTL |= MC1;      // Start Timer_A0 is setting operation mode.
#endif
}

void TIMER_0_stop(void)
{
    TA0CTL &= ~(MC1 | MC0);     // Stop Timer_A0
}

//void TIMER_1_init(void)
//{
//    TA1CTL |= TACLR;
//    TA1CTL = TASSEL_2 | ID_2 | MC_1;
//    TA1CCR0 = 20000;
//    TA1CCTL0 |= CCIE | CM_0;
//    TA1CTL |= TAIE;
//}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(TA0IV, TA0IV_TAIFG))
    {
    case TA0IV_NONE:
        break;
    case TA0IV_TACCR1:
        break;
    case TA0IV_TACCR2:
        break;
    case TA0IV_TAIFG:
        COUNTER.COUNT++;
        break;
    default:
        break;
    }
}

//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=TIMER1_A0_VECTOR
//__interrupt void TA1_ISR(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TA1_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//    if (!(UCA0IFG & UCBUSY))
//    {
//        while (!(UCA0IFG & UCRXIFG))
//            ;
//        *rxBuffer = UCA0RXBUF;
//        *rxBuffer++;
////        TA1R = 0;
//    }
//}

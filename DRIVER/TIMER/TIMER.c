/*
 * TIMER.c
 *
 *  Created on: Jul 22, 2019
 *      Author: eeit2
 */
#include<msp430.h>
#include "TIMER.h"

unsigned char* txData;                        // UART internal variable for TX
unsigned char* rxBuffer;                     // Received UART character

void set_SMCLK()
{
    CSCTL0 = 0;  //Internal resistor
    P1DIR |= BIT1;
    P1SEL1 |= BIT1; // SMCLK Pin Function
}

void TIMER_init(void)
{

    // Setup TA0
    TA0CTL |= TACLR;
    TA0CTL = TASSEL_2 | ID_2 | MC_2;
    TA0R = 20000;
    TA0CCTL0 |= CCIE; // enable interrupt
    TA0CTL |= TAIE;

}

void TIMER_1_init(void)
{
    TA1CTL |= TACLR;
    TA1CTL = TASSEL_2 | ID_2 | MC_1;
    TA1CCR0 = 20000;
    TA1CCTL0 |= CCIE | CM_0;
    TA1CTL |= TAIE;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    SD24CCTL0 |= SD24IE;
    SD24CCTL0 |= SD24SC;                      // Start conversion

    TA0CCTL0 &= ~CCIE;
    TA0CTL &= ~TAIE;

    __bic_SR_register_on_exit(LPM0_bits);       // Wake up from LPM0
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TA1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TA1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (!(UCA0IFG & UCBUSY))
    {
        while (!(UCA0IFG & UCRXIFG))
            ;
        *rxBuffer = UCA0RXBUF;
        *rxBuffer++;
//        TA1R = 0;
    }

    __bic_SR_register_on_exit(LPM0_bits);       // Wake up from LPM0
}

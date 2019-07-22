/*
 * TIMER.c
 *
 *  Created on: Jul 15, 2019
 *      Author: eeit2
 */


#include "DRIVER/TIMER/TIMER.h"
#include "DRIVER/UART/UART.h"


unsigned char* txData;                        // UART internal variable for TX
unsigned char* rxBuffer;                     // Received UART character

void set_SMCLK(void)
{
       CSCTL0 = 0;  //Internal resistor
       CSCTL1 |= DIVS__2; // SMCLK = DCO/2
       P1DIR =|= BIT1;
       P1SEL1 |= BIT1; // SMCLK Pin Function
}

void TIMER_0_init(unsigned int i){

    /*
     * TASSEL_2: timer A clock source: SMCLK = 16.384 Mhz
     * MC_1: timer A mode control: Up mode -> Timer counts to TAxCCR0
     * ID_1: timer A input divider: 1 -> f_clock_source/2
     * f_clock = 1 MHz -> t_pulse = 100ms
     */

    // Setup TA0
    TA0CTL = TASSEL_2 | ID_3 | MC_1;
    TA0CCR0 = 50000; // ((1million cycles/2)/1s * 1s/1000ms) -> t_pulse = 25ms
    TA0CCTL0 = CCIE; // enable interrupt

}

void TIMER_init(unsigned int a){

    TA0CTL = TASSEL_2 | ID_3 | MC_1;

    // Setup TA1 for UART
    TA1CCR0 = 20000;   // t = t_pulse * 20000 = 10ms -
    TA1CCTL0 = CCIE;   // enable Capture/compare interrupt
    TA1CTL = TASSEL_2 | ID_3;    // SMCLK/8, Up Mode

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
    while (1)
    {
        SD24CCTL2 |= SD24SC;                      // Start conversion
        UCA0TXBUF = txData;
        TA0R = 0;
    }
}

// interrupt for TIMER1 //
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TA1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) TA1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if(!(UCA0IFG & UCBUSY))
    {
        while(!(UCA0IFG & UCRXIFG));
        rxBuffer = UCA0RXBUF;
        TA1R = 0;
    }
    //TA1CTL &= ~MC_1;  /* Timer A mode control: 0 - stop  - >  timer1 dis-anable*/
}

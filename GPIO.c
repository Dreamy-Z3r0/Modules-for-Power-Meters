/*
 * GPIO.c
 *
 *  Created on: July 15, 2019
 *      Author: Z3r0
 *
 *  Hardware description:
 *      LED on pin P1.5 : RUN
 *      LED on pin P1.4 : Error indicator
 *      * LEDs are active-LOW.
 *
 *  Software description:
 *      GPIO_init(..) : initiate GPIO function
 *      GPIO_operation(..) : choose desired LED to work with, as well as desired operation
 *      set_MCLK(..) : set up MCLK for delay functions
 *      delay_sec(..) : delay the whole program for a set duration (in seconds)
 *      delay_ms(..) : delay the whole program for a set duration (in milliseconds)
 */

#include "GPIO.h"

void GPIO_init(void)
{
    P1SEL0 &= ~(BIT4 | BIT5);   // Set function for pins P1.4 and P1.5 to GPIO
    P1SEL1 &= ~(BIT4 | BIT5);

    P1DIR |= (BIT4 | BIT5);     // Set P1.4 and P1.5 as outputs

    P1OUT |= (BIT4 | BIT5);     // Initial output value for both P1.4 and P1.5 is HIGH: both LEDs are off
}

void GPIO_operation(uint8_t led, uint8_t operation)
{
    /*
     * led : variable to choose which LED to work with
     *          led = 0x00 : LED for RUN operation
     *          led = 0x01 : LED indicates error
     *
     * operation : variable to choose which operation is done with chosen LED
     *          operation = 0x00 : LED is off
     *          operation = 0x01 : LED is on
     *          operation = 0x02 : toggle LED
     */

    if (LED_RUN == led)
    {
        if (LED_OFF == operation)           P1OUT |=  BIT5;
        else if (LED_ON == operation)       P1OUT &= ~BIT5;
        else if (LED_TOGGLE == operation)   P1OUT ^=  BIT5;
    }
    else if (LED_ERROR == led)
    {
        if (LED_OFF == operation)           P1OUT |=  BIT4;
        else if (LED_ON == operation)       P1OUT &= ~BIT4;
        else if (LED_TOGGLE == operation)   P1OUT ^=  BIT4;
    }
}

void set_MCLK(void)
{
    // Desired MCLK is 16.384MHz and __delay_cycles(..) uses MCLK as clock source
    CSCTL1 = DIVM_0;
}

void delay_sec(uint32_t duration)
{
    set_MCLK();     // Ensure MCLK = 16.384MHz

    do
    {
        __delay_cycles(16384000);   // delay 1 s
        duration--;
    }   while (duration > 0);
}

void delay_ms(uint32_t duration)
{
    set_MCLK();     // Ensure MCLK = 16.384MHz

    do
    {
        __delay_cycles(16384);   // delay 1 ms
        duration--;
    }   while (duration > 0);
}




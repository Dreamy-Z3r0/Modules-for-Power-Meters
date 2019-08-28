/*
 * UART.c
 *
 *  Created on: January 9, 2019
 *      Author: Administrator
 */

#include "UART.h"

void UART_init()
{
    P1SEL1 &= ~(BIT2 | BIT3); // configure P1.2 and P1.3 for UART communication (RXD and TXD pins, respectively)
    P1SEL0 |=  (BIT2 | BIT3);

    // UART specific configuration
    UCA0CTLW0 |= UCSWRST;    // reset state enabling
    UCA0CTLW0 |= (UCSSEL0 | UCSSEL1);   // choose SMCLK = 16.384MHz as UART clock source
    UCA0BR1 = 0;            // configure baud rate to 115200
    UCA0BR0 = 142;
    UCA0MCTLW = 0x2200;     // UCBRS0 = 0x22 and oversampling mode is disabled
    UCA0CTL1 &= ~UCSWRST;    // disable reset state

    // Enable UART interrupt
     UCA0IE |= UCRXIE;
}

void UART_write_buff(uint8_t *buff)
{
    while (UCA0STATW & UCBUSY)
        ;
    while (!(UCA0IFG & UCTXIFG))
        ;
    UCA0TXBUF = *buff;
}

void UART_input_String(uint8_t *buff, uint8_t buff_length)
{
    int index;
    for (index = 0; index < buff_length - 1; index++)
    {
        UART_write_buff(buff);
        buff++;
    }
}

void UART_input_number(float number)
{
    int flag = 0;   // if number < 0, flag = 1
    if (0 > number)
        flag = 1;

    /* Get string length */
    int power = 1;  // Length of unsigned number
    while (0 != (unsigned int) ((number * 100) / pow(10, power)))
    {
        power++;
    }
    flag = flag + power;        // Length of the signed number

    /* Convert number to string */
    // It is agreed that the sender sends a number multiplied by 100 and neglects the floating point.
    uint8_t *string = (uint8_t*) malloc(flag * sizeof(uint8_t));
    uint8_t *location = string;
    uint8_t *location_free = string;

    if (0 > number)
    {
        number = -number;   // acquires unsigned number
        *location = '-';
        location++;
    }

    itoa((unsigned int) (number * 100), power, location, 10);

    /* Sends converted number via UART */
    UART_input_String(string, flag+1);

    free(location_free);
}

void itoa(unsigned int number, int number_length, uint8_t *string, int base)
{
    if (10 == base)     // Number is desired to be sent as decimal number
    {
        int order = number_length - 1;   // used for acquiring most left number character

        int x,
            max = 10;   // Desired max is 10^order
        if (0 == order)
            max = 1;    // max = 10^0 = 1
        else
        {
            for (x = 1; x < order; x++)     // max = 10^order
            {
                max = max * 10;
            }
        }

        int tmp_num;
        while (0 < number_length)     // next character 'till the end of string (index = 0 for positive number, and index = 1 for negative number)
        {
            tmp_num = number / max;         // acquires left most number character of unsigned input number
            number = number - tmp_num * max;    // remove left most character of unsigned input number
            *string = tmp_num + '0';            // converts left most number to character
            max = max / 10;
            string++;
            number_length--;
        }
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG: break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}

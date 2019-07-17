#include "msp430.h"
#include <stdint.h>
#include <math.h>



void UART_init(void){

    // Enable UART Connection
    P1SEL0 |=   BIT2|BIT3;
    P1SEL1 &= ~(BIT2|BIT3);

    // UART specific configuration
    UCA0CTL1 |= UCSWRST;    // reset state enabling
    UCA0CTL1 |= UCSSEL_2;   // choose SMCLK = 16.384MHz as UART clock source

    UCA0BR0   = 142;                    // 115200 baud, cai dat clock baud rate
    UCA0BR1   = 0;
    UCA0MCTLW = 0x2200;                 // 16.384MHz/115200 = 142.22 (See UG)

    UCA0CTL1 &= ~UCSWRST;    // disable reset state

    // Enable UART interrupt
    UCA0IE |= UCTXIE;
    UCA0IE |= UCRXIE;
}

void UART_write_buff(uint8_t *buff, uint8_t len){
    uint8_t i;
    for(i = 0; i < len; i++)
    {
        while(UCA0STATW & UCBUSY);
        while (!(UCA0IFG & UCTXIFG));   // USCI_A0 TX buffer ready?
        UCA0TXBUF = buff[i];
    }
}

void write_string(uint8_t *str){
    while(*str != '\0'){
        UART_write_buff(str,1);
        str++;
    }
}

void write_num(float x){
    long num = x*100, isNegative = 0;
    if(num < 0){
        isNegative = 1;
        num = -num;
    }




    uint8_t str[100];
    uint8_t i = 0;

    // Convert integer to string
    while(num != 0)
    {
        uint8_t rem = num%10; // obtain the last digit
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0'; // convert the number to ASCII
        num = num/10;
    }
    if(isNegative) str[i++] = '-';
    str[i] = '\0';      // end the string

    // Reverse the string
    uint8_t final[100];
    uint8_t begin, end, count = 0;
    while(str[count] != '\0') count++;
    end = count - 1;

    for(begin = 0; begin < count; begin++){
        final[begin] = str[end];
        end--;
    }
    final[begin] = '\0';    // end the string

    write_string(final);
}

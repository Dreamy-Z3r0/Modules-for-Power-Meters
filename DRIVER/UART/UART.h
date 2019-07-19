/*
 * UART.h
 *
 *  Created on: 9 thg 1, 2019
 *      Author: Administrator
 */

#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#ifndef DRIVER_UART_UART_H_
#define DRIVER_UART_UART_H_

void UART_init();
void UART_write_buff(uint8_t *buff);
void UART_input_String(uint8_t *buff, uint8_t buff_length);
void UART_input_number(float number_in);
void itoa(unsigned int number, int number_length, uint8_t *string, int base);

#endif /* DRIVER_UART_UART_H_ */

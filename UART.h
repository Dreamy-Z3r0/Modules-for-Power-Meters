/*
 * UART.h
 *
 *  Created on: Jul 15, 2019
 *      Author: USER
 */

#include "msp430.h"
#include <stdint.h>

#ifndef DRIVER_UART_UART_H_
#define DRIVER_UART_UART_H_

void UART_init(void);
void UART_write_buff(uint8_t *buff, uint8_t len);
void write_string(uint8_t *str);  // Write the string to the buffer
void write_num(float x);   // Converts an integer into string and writes it to the buffer

#endif /* DRIVER_UART_UART_H_ */

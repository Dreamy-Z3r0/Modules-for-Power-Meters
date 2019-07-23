/*
 * FLASH.h
 *
 *  Created on: Jul 19, 2019
 *      Author: eeit2
 */

#include <msp430.h>
#include <stdint.h>
#include <math.h>

#ifndef DRIVER_FLASH_FLASH_H_
#define DRIVER_FLASH_FLASH_H_

// Temperature Info
#define TEMP_ADDR 0x1060
#define TEMP_VREF       TEMP_ADDR + 8       // Reference Voltage
#define TEMP_TCSENSOR   TEMP_VREF + 4       //
#define V_OFFSET        TEMP_TCSENSOR + 4   // Offset Voltage

void FLASH_init(void);
void FLASH_write_seg(uint16_t write_addr, uint8_t *value, uint8_t write_length);
void FLASH_read_seg(uint16_t read_addr, uint8_t *value ,uint8_t read_length);
void FLASH_erase_seg(uint16_t erase_addr);

#endif /* DRIVER_FLASH_FLASH_H_ */

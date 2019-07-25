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
/*
 * 1 address points to an 8 bit value
 * To store a 16 bit value, 2 addresses are required
 *
 */
#define TEMP_K          0xF800
#define TEMP_VREF       TEMP_K + 2          // Reference Voltage
#define TEMP_TCSENSOR   TEMP_VREF + 2       //
#define TEMP_VOFFSET    TEMP_TCSENSOR + 2   // Offset Voltage



void FLASH_init(void);
void FLASH_write_seg(uint16_t write_addr, uint16_t *value, uint8_t write_length);
void FLASH_read_seg(uint16_t read_addr, uint16_t *value, uint8_t read_length);
void FLASH_erase_seg(uint16_t erase_addr);
void FLASH_update_temp(void);

#endif /* DRIVER_FLASH_FLASH_H_ */

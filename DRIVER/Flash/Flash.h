/*
 * FLASH.h
 *
 *  Created on: Jul 19, 2019
 *      Author: eeit2
 */

#ifndef DRIVER_FLASH_FLASH_H_
#define DRIVER_FLASH_FLASH_H_

#include "DRIVER/PROCESS/PROCESS.h"

void Flash_init(void);
void write_segA(char write_result, uint8_t write_size);
void read_segA(char read_result, uint8_t read_size);

#endif /* DRIVER_FLASH_FLASH_H_ */


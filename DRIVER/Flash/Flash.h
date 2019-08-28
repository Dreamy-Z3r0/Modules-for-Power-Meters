/*
 * FLASH.h
 *
 *  Created on: Jul 19, 2019
 *      Author: eeit2
 */

#include "DRIVER/PROCESS/PROCESS.h"


#ifndef DRIVER_FLASH_FLASH_H_
#define DRIVER_FLASH_FLASH_H_

#define FLASH_SIZE 24
#define FLASH_ADDR_SETTING        0xEC00
#define FLAH_ADDR_TEMP            0xF000


typedef struct
{
uint8_t temperature[FLASH_SIZE];
}FLASH_typedef;

extern FLASH_typedef FLASH;


void Flash_init(void);

void setting_infor(void);
void update_infor(void);

void write_segA(char write_result, uint8_t write_size);
void read_segA(char read_result, uint8_t read_size);


void store_result(uint16_t temperature_addr);
void update_result(uint16_t temperature_addr);

#endif /* DRIVER_FLASH_FLASH_H_ */

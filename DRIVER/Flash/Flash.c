/*
 * FLASH.c
 *
 *  Created on: Jul 19, 2019
 *      Author: eeit2
 */

#include "DRIVER/FLASH/FLASH.h"
FLASH_typedef FLASH;

unsigned char write_result;                     // Bit value for segment A
unsigned char* read_result;


// Define prototype

void Flash_init(void);


void write_segA(char *write_result, uint8_t write_size);
void read_segA( char *read_result, uint8_t read_size);

void store_result(uint16_t temperature_addr);
void update_result(uint16_t temperature_addr);


// Set up FLASH

void Flash_init(void)
{
    FCTL2 |= FWKEY + FSSEL_2 + FN0;             // FCTL2 password
                                                // SMCLK/2 for timing generator
                                                // Flash control clock divider -> divisor = 1
    write_result = 0;                           // Initialize value
    while(1)
    {
        write_segA(write_result++);             // Write segment A, increment value
    }
}


// Write in FLASH

void write_segA(char *write_result, uint8_t write_size)
{
    unsigned char* flash_ptr;              // Create pointer
    unsigned int i;

    flash_ptr = (char*) write_addr;            // Initialize flash pointer
    FCTL1 = FWKEY + ERASE;                 // Set erase bit
    FCTL3 = FWKEY;                         // Clear lock bit
    *flash_ptr = 0;                        // Dummy write for erase Flash segment

    FCTL1 = FWKEY + WRT;                   // Write bit for write operation

    for (i=0; i<write_size; i++)
    {
        *flash_ptr++ = write_result;       // Write value to flash
    }

    FCTL1 = FWKEY;                         // Break write mode
    FCTL3 = FWKEY + LOCKSEG;

}


// Read from FLASH

void read_segA(char *read_result, uint8_t read_size)
{
    unsigned char* flash_ptr;              // Flash pointer
    unsigned int i;

    for(i=0; i<read_size; i++)
    {
       *read_result = *flash_ptr++;
    }
}



void update_result()
{
    read_result(temperature_addr, FLASH.temperature, 24);
}

void store_result()
{
    write_result(temperature_addr, FLASH.temperature, 24);
}


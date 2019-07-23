/*
 * FLASH.c
 *
 *  Created on: Jul 19, 2019
 *      Author: eeit2
 *
 *  Updated on: July 23, 2019
 *      Author: thebeztpersonthateverlivedinthisworldinpastpresentandfutureandthereisnothingyoucansayaboutitthatcanchangetheveryfactofit.Mymereexistencedisproveswhateveryouhavetosayaboutit:P
 *
 *  Description:
 *      The FLASH Library consists of operations done to the FLASH Memory.
 *      Related information are placed in the same 1KB segments for better
 *      data handling. (e.g. when the whole segment of temperature needs to
 *      be overwritten, only that specific segment needs to be erased,
 *      and other data is not touched)
 *
 *  Software description:
 *      FLASH_init(..) : initiate FLASH function
 *      FLASH_write_seg(..) : write a value to a segment in the FLASH Memory
 *      FLASH_read_seg(..) : reads a value from a segment in the FLASH Memory
 *      FLASH_erase_seg(..) : erases a whole segment in the FLASH Memory
 */
#include "DRIVER/FLASH/FLASH.h"

void FLASH_init(void)
{
    /*
     * FWKEY: (0xA500h) Flash key for write
     * FSSEL_1: MCLK for Flash Timing Generator. MCLK = 16.384MHz
     * FNx bits: Divisor is equals to 101000b (40)
     *
     * MCLK/Divisor = 16.384MHz/40 = 409.6kHz
     *
     * Flash clock will run at ~409.6kHz. Datasheet recommends 257kHz - 476kHz
     */

    FCTL2 = FWKEY | FSSEL_2 | FN3 | FN5;
}

void FLASH_write_seg(uint16_t write_addr, uint8_t *value, uint8_t write_length)
{
    uint16_t *Flash_ptr;                    // Flash pointer
    Flash_ptr = (uint16_t *)write_addr;     // Initialize Flash pointer

    /*
     Clear Flash
    FCTL1 = FWKEY | ERASE;
    while(FCTL3 & BUSY);

    if(FCTL3 & LOCKSEG) {                   // If Info Seg is still locked
        FCTL3 = FWKEY | LOCKSEG;            // Clear LOCKSEG bit
    }
     */
    FCTL3 = FWKEY;                          // Clear LOCK, to unlock flash memory for flash writing
    FCTL1 = FWKEY | WRT;                    // Set WRT, to enable Write operation

    while(write_length--)
    {
        *Flash_ptr++ = *value++;            // Write value to flash
        while(FCTL3 & BUSY);
    }

    FCTL1 = FWKEY;                          // Clear WRT, to disable Write operation
    FCTL3 = FWKEY | LOCK;                   // Set LOCK

}

void FLASH_read_seg(uint16_t read_addr, uint8_t *value ,uint8_t read_length)
{
    uint16_t *Flash_ptr;                    // Flash pointer
    Flash_ptr = (uint16_t *)read_addr;      // Initialize Flash pointer

    while(read_length--)
    {
       *value++ = *Flash_ptr++;             // Store flash to value
    }
}

void FLASH_erase_seg(uint16_t erase_addr)
{
    uint16_t *Flash_ptr;                    // Flash pointer
    Flash_ptr = (uint16_t *)erase_addr;     // Initialize Flash pointer
    FCTL3 = FWKEY;                          // Clear LOCK, to unlock flash memory erase operation
    FCTL1 = FWKEY | ERASE;                  // Set ERASE, to enable Segment Erase operation

    *Flash_ptr = 0;                         // Dummy Write, to initiate Erase operation
    while(FCTL3 & BUSY);

    FCTL1 = FWKEY;                          // Clear ERASE, to disable Segment Erase operation
    FCTL3 = FWKEY | LOCK;                   // Set LOCK

}

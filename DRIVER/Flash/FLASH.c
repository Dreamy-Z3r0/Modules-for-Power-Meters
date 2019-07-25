/*
 * FLASH.c
 *
 *  Created on: Jul 19, 2019
 *      Author: eeit2
 *
 *  Updated on: July 25, 2019
 *      Author: thebeztpersonthateverlivedinthisworldinpastpresentandfutureandthereisnothingyoucansayaboutitthatcanchangetheveryfactofit.Mymereexistencedisproveswhateveryouhavetosayaboutit:P
 *
 *  Description:
 *      The FLASH Library consists of operations done to the FLASH Memory.
 *      Related information are placed in the same 1KB segments for better
 *      data handling. (e.g. when the whole segment of temperature needs to
 *      be overwritten, only that specific segment needs to be erased,
 *      and other data is not touched) This is done due to the fact that the
 *      smallest amount of flash that can be erased is one segment.
 *
 *      Note:   The flash write is not yet optimized because of two reasons.
 *              1) every time a piece of data is written to the FLASH memory,
 *              the CPU is held and everything else stops.
 *              2) the whole write operation switches on and off every
 *              character, as opposed to being switched on for the whole
 *              write operation. Consequently, slowing the whole process.
 *
 *
 *  Software description:
 *      FLASH_init(..) : initiate FLASH function
 *      FLASH_write_seg(..) : write a value to a segment in the FLASH Memory
 *      FLASH_read_seg(..) : reads a value from a segment in the FLASH Memory
 *      FLASH_erase_seg(..) : erases a whole segment in the FLASH Memory
 *      FLASH_update_temp(..) : updates value temperature
 */
#include "../FLASH/FLASH.h"

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

void FLASH_update_temp(void)
{
    /* Place read temperature values from SD24 into corresponding positions in array
     * temp_values[0] = Kelvin Temperature  // Address: 0xF800
     * temp_values[1] = Reference Voltage   // Address: 0xF802
     * temp_values[2] = TC Sensor           // Address: 0xF804
     * temp_values[3] = Offset Voltage      // Address: 0xF806
     */
    uint16_t temp_values[4] = {0, 0, 0, 0};

    FLASH_write_seg(TEMP_K, temp_values, 4);
}

void FLASH_write_seg(uint16_t write_addr, uint16_t *value, uint8_t write_length)
{
    uint16_t *Flash_ptr;                    // Flash pointer
    Flash_ptr = (uint16_t *)write_addr;     // Initialize Flash pointer

    if(FCTL3 & LOCKSEG)                     // If Info Seg is still locked
    {
        FCTL3 = FWKEY | LOCKSEG;            // Clear LOCKSEG bit
    }
    // Erase the segment
    FCTL1 = FWKEY | ERASE;                  // Set ERASE, to enable Segment Erase operation
    *Flash_ptr = 0;                         // Dummy Write, to initiate Erase operation
    while(FCTL3 & BUSY);
    // Write to the segment
    FCTL1 = FWKEY | WRT;                    // Set WRT, to enable Write operation
    while(write_length--)
    {
        *Flash_ptr++ = (*value++);          // Write value to flash
        while(FCTL3 & BUSY);
    }

    FCTL1 = FWKEY;                          // Clear WRT, to disable Write operation
    FCTL3 = FWKEY | LOCK | LOCKSEG;         // Set LOCK

}

void FLASH_read_seg(uint16_t read_addr, uint16_t *value, uint8_t read_length)
{

    uint16_t *Flash_ptr;                    // Flash pointer
    Flash_ptr = (uint16_t *)read_addr;      // Initialize Flash pointer

    while(read_length--)
    {
        (*value++) = *Flash_ptr++;          // Store flash to value
    }
}

void FLASH_erase_seg(uint16_t erase_addr)
{
    uint16_t *Flash_ptr;                    // Flash pointer
    Flash_ptr = (uint16_t *)erase_addr;     // Initialize Flash pointer
    if(FCTL3 & LOCKSEG)                     // If Info Seg is stil locked
    {
        FCTL3 = FWKEY | LOCKSEG;            // Clear LOCKSEG bit
    }
    FCTL1 = FWKEY | ERASE;                  // Set ERASE, to enable Segment Erase operation

    *Flash_ptr = 0;                         // Dummy Write, to initiate Erase operation
    while(FCTL3 & BUSY);

    FCTL1 = FWKEY;                          // Clear ERASE, to disable Segment Erase operation
    FCTL3 = FWKEY | LOCK | LOCKSEG;                   // Set LOCK

}

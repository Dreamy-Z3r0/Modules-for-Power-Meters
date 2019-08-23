/*
 * ANALYZE.h
 *
 *  Created on: August 23, 2019
 *      Author: Z3r0
 *
 *  Software description:
 *      RAW_DATA_PROCESSING(..): Convert from raw ADC data of voltage and current input to their real values.
 *                               Calculate internal temperature of MSP430i2041 microcontroller in Kelvin, degree Celsius, and degree Fahrenheit.
 *      FREQUENCY(..): Calculate frequency of the input AC signals.
 *      RMS_VALUES(..): Calculate V_rms and I_rms of the measured system.
 *      POWER_AND_ENERGY(..): Calculate apparent power, active power, power factor of the measured system.
 *                            Calculate power consumption on the whole system.
 *      ANALYZE_LL(..): Function that calls all above functions to perform first-hand analysis of the measured system.
 */

#include "../PROCESS/PROCESS.h"

#ifndef DRIVER_ANALYZE_ANALYZE_H_
#define DRIVER_ANALYZE_ANALYZE_H_

#define SAMPLING_RATE 4000.0
#define VALUE(RAW, OFFSET, REFERENCE) (((RAW * REFERENCE) / 0x7FFF) - OFFSET)
#define DELTA_T(duration) ((2*duration)/3600)

uint8_t INDEX = 0;

void RAW_DATA_PROCESSING(void);
void FREQUENCY(void);
void RMS_VALUES(void);
void POWER_AND_ENERGY(void);

void ANALYZE_LL(void);

#endif /* DRIVER_ANALYZE_ANALYZE_H_ */

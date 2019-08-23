/*
 * SD24.h
 *
 *  Created on: August 9, 2019
 *      Author: Z3r0
 *
 *  Hardware description:
 *      Channel 0 (A0.0+ and A0.0-): Voltage input channel, grouped with channel 1;
 *                                      470.7mV (peak) for 1-phase AC voltage of 220Vr.m.s.,
 *                                      855.8mV (peak) for 3-phase AV voltage of 400Vr.m.s.
 *      Channel 1 (A1.0+ and A1.0-): Current input, master channel for measurement.
 *
 *  Software desciption:
 *      SD24_init(..): Initiate SD24 Sigma Delta 24 Function on MSP430i2041,
 *                     including function configuration on first run,
 *                     and/or starting conversion of second run onwards.
 *      SD24_stop(..): Stop any running SD24 function.
 */

#include "../PROCESS/PROCESS.h"

#ifndef DRIVER_SD24_SD24_H_
#define DRIVER_SD24_SD24_H_

#define ARRAY_SIZE     90

#define ACTIVE_STATE   0x01
#define INACTIVE_STATE 0x00

#define ZERO_CONDITION_LOW(input)   (((0x8000 - 819) <= input) & (input <= 0x8000))
#define ZERO_CONDITION_HIGH(input)  ((0x8000 <= input) & ((0x8000 + 819) <= input))

typedef struct
{
    double_t Vref;
    double_t frequency;
    double_t V_rms;
    double_t I_rms;
    double_t THD;
    double_t voltage[ARRAY_SIZE];
    double_t current[ARRAY_SIZE];
    double_t power_real;
    double_t power_active;
    double_t power_factor;
    double_t energy;
    double_t temperature[3];
} Processing_Result;

typedef struct
{
    uint16_t voltage[ARRAY_SIZE];
    uint16_t current[ARRAY_SIZE];
    uint16_t temperature;
} Measured_Data;

typedef struct
{
    uint8_t INDEX;
    uint8_t ConversionFlag;
    uint8_t GAIN_CHN_V;
    uint8_t GAIN_CHN_I;
    uint8_t GAIN_CHN_TEM;
    uint8_t END_POINT;
    double_t SamplingDuration;
    double_t TemperatureOffset;
    double_t TCsensor;

    Measured_Data sampling;
    Processing_Result processed;
} Power_Meter_typedef;

extern Power_Meter_typedef POWER_METER;

void SD24_init(void);
void SD24_stop(void);

#endif /* DRIVER_SD24_SD24_H_ */

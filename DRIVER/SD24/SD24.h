/*
 * SD24.h
 *
 *  Created on: August 9, 2019
 *      Author: Z3r0
 *
 *  Hardware description:
 *      Channel 0 (A0.0+ and A0.0-): Voltage input channel, grouped with channel 1;
 *                                      470.7mV (peak) for 1-phase AC voltage of 220Vr.m.s.,
 *                                      855.8mV (peak) for 3-phase AV voltage of 400V r.m.s.
 *      Channel 1 (A1.0+ and A1.0-): Current input, master channel for measurement.
 *
 *  Software desciption:
 *      SD24_init(..): Initiate SD24 Sigma Delta 24 Function on MSP430i2041,
 *                     including function configuration on first run,
 *                     and/or starting conversion of second run onwards.
 *      Channel_0_init(..): Configure SD24 channel 0 for input voltage measurement.
 *                          This function is called by SD24_init(..) only.
 *      Channel_1_init(..): Configure SD24 channel 1 for input current measurement.
 *                          This function is called by SD24_init(..) only.
 *      Zero_Padding(..): Acquire measurement values of one single AC period (~80 samples for 50-Hz AC input)
 *                        and perform zero padding solution up to 256 samples for Total Harmonic Distorsion calculation.
 *                        This function is automatically called after SD24 finishes sampling input voltage/current.
 *      Raw_Data_Processing(..): Perform zero padding technique for sampled data.
 *                               This function is a sub-function of Zero_Padding(..) and must be called by Zero_Padding(..) only.
 */

#include "../PROCESS/PROCESS.h"

#ifndef DRIVER_SD24_SD24_H_
#define DRIVER_SD24_SD24_H_

#define Vref = 1.2;

uint8_t INDEX = 0, ConversionFlag = 0;
uint8_t FIRST_RUN = 1;

int data_Channel_0[256],
    data_Channel_1[256];

uint8_t GAIN_Channel_0, GAIN_Channel_1;

void SD24_init(void);
void Channel_0_init(void);
void Channel_1_init(void);
void Zero_Padding(void);
void Raw_Data_Processing(int Raw_Data[]);

#endif /* DRIVER_SD24_SD24_H_ */

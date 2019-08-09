/*
 * SD24.c
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

#include "SD24.h"

void SD24_init(void)
{
    if (1 == FIRST_RUN)
    {
        FIRST_RUN = 0;
        SD24CTL |= SD24REFS;    // Internal reference chosen, Vref = 1.2V

        Channel_0_init(void);   // Initiate configurations for SD24 channel 0
        Channel_1_init(void);   // Initiate configurations for SD24 channel 1

        __delay_cycles(3277);   // MCLK = 16.384MHz by default is used for this delay function. Total delay time = 224us (since ~200us is required)
    }
    else
        SD24CCTL1 |= SD24SC;    // Start conversions for second run onwards
}

void Channel_0_init(void)
{
    SD24CCTL0 &= ~SD24SNGL;     // Continuous conversion mode
    SD24CCTL0 |=  SD24GRP;      // Multiple channel mode, Channel 0 is a follower.

    SD24CCTL0 &= ~(SD24OSR1 | SD24OSR0);    // Ensure Oversampling Ratio = 256

    SD24CCTL0 &= ~SD24LSBTOG;   // Disable toggling SD24 Least-Significant Bits Access
    SD24CCTL0 &= ~SD24LSBACC;   // Data read from Most-Significant Bits

    SD24CCTL0 &= ~SD24DF;       // Offset binary mode chosen

    SD24INCTL0 &= ~(SD24GAIN2 | SD24GAIN1 | SD24GAIN0);     // Pre-amplifier gain = 1
    GAIN_Channel_0 = 1;

    SD24INCTL0 &= ~(SD24INCH2 | SD24INCH1 | SD24INCH0);     // Choose A0.0 as input source
}

void Channel_1_init(void)
{
    SD24CCTL1 &= ~SD24SNGL;     // Continuous conversion mode
    SD24CCTL1 &= ~SD24GRP;      // Multiple channel mode, Channel 1 is the master.

    SD24CCTL1 &= ~(SD24OSR1 | SD24OSR0);    // Ensure Oversampling Ratio = 256

    SD24CCTL1 &= ~SD24LSBTOG; // Disable toggling SD24 Least-Significant Bits Access
    SD24CCTL1 &= ~SD24LSBACC;   // Data read from Most-Significant Bits

    SD24CCTL1 &= ~SD24DF;       // Offset binary mode chosen

    SD24INCTL1 &= ~(SD24GAIN2 | SD24GAIN1 | SD24GAIN0);    // Pre-amplifier gain = 1
    GAIN_Channel_1 = 1;

    SD24INCTL1 &= ~(SD24INCH2 | SD24INCH1 | SD24INCH0);    // Choose A0.0 as input source

    SD24CCTL1 |= (SD24SC | SD24IE);     // Enable interrupt and start conversions
}

void Zero_Padding(void)
{
    Raw_Data_Processing(data_Channel_0);
    Raw_Data_Processing(data_Channel_1);
}

void Raw_Data_Processing(int Raw_Data[])
{
    uint8_t sign;   // Determine reliable zero point of sampled signal
    uint8_t FIRST_ZERO = 0;     // Detecting first zero from sampled data

    INDEX = 0;
    uint8_t START_POINT = 0;
    uint8_t END_POINT = START_POINT + 70;

    while (0 == FIRST_ZERO)     // Detect first zero of the input signal
    {
        if ((0x8000 <= data_AC[START_POINT]) & (data_AC[START_POINT] <= (819 + 0x8000)))
        {
            sign = 1;
            INDEX = START_POINT + 70;
            FIRST_ZERO = 1;
        }
        else if (((0x8000 - 819) <= data_AC[START_POINT]) & (data_AC[START_POINT] <= 0x8000))
        {
            sign = 0;
            INDEX = START_POINT + 70;
            FIRST_ZERO = 1;
        }
        else
            START_POINT++;
    }

    switch (sign)       // Detect third zero of input signal
    {
    case 1:
        do
        {
            if ((0x8000 <= Raw_Data[INDEX]) & (Raw_Data[INDEX] <= (819 + 0x8000)))
            {
                END_POINT = INDEX - 1;
                zero_Position = 1;
            }
            else
                INDEX++;
        }
        while (1 > zero_Position);
        break;
    case 0:
        do
        {
            if (((0x8000 - 819) <= Raw_Data[INDEX]) & (Raw_Data[INDEX] <= 0x8000))
            {
                END_POINT = INDEX - 1;
                zero_Position = 1;
            }
            else
                INDEX++;
        }
        while (1 > zero_Position);
        break;
    default:
        break;
    }

    for (INDEX = 0; INDEX < 256; INDEX++)   // Start the sampled data at the first zero and perform zero padding from the third zero.
                                            // The unsigned values of one cycle are changed to signed values.
    {
        if ((INDEX + START_POINT) > END_POINT)
            Raw_Data[INDEX] = 0;
        else
            Raw_Data[INDEX] = Raw_Data[INDEX + START_POINT] - 0x8000;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD24_VECTOR
__interrupt void SD24_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD24_VECTOR))) SD24_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(SD24IV, SD24IV_SD24MEM1))
    {
    case SD24IV_NONE:
        break;
    case SD24IV_SD24OVIFG:
        break;
    case SD24IV_SD24MEM0:
        break;
    case SD24IV_SD24MEM1:
        data_Channel_0[INDEX] = SD24MEM0;
        data_Channel_1[INDEX++] = SD24MEM1;

        if (160 == INDEX)
        {
            SD24CCTL1 &= ~SD24SC;
            ConversionFlag = 1;
            __bic_SR_register_on_exit(LPM0_bits);   // Wake up from LPM0

            Zero_Padding();
        }

        break;
    case SD24IV_SD24MEM2:
        break;
    case SD24IV_SD24MEM3:
        break;
    default:
        break;
    }
}

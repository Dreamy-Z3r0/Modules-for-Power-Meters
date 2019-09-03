/*
 * SD24.c
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
 */

#include "SD24.h"

Power_Meter_typedef POWER_METER =
{
 .INDEX = 0,
 .FIRST_RUN = 2,

 .processed.Vref = 1.2,
 .processed.energy = 0,

 .ConversionFlag = INACTIVE_STATE,
 .ResultReady = INACTIVE_STATE,

 .GAIN_CHN_V = 1,
 .GAIN_CHN_I = 1,
 .GAIN_CHN_TEM = 1,

 .TemperatureOffset = 0,
 .TCsensor = 2.158,
};

void SD24_init(void)
{
    POWER_METER.INDEX = 0;
    POWER_METER.ConversionFlag = ACTIVE_STATE;

    if (2 == POWER_METER.FIRST_RUN)
    {
        POWER_METER.FIRST_RUN = 0;
        SD24CTL |= SD24REFS;    // Internal reference chosen, Vref = 1.2V

        /* Initiate configurations for SD24 Internal Temperature Sensor channel 2 */
        SD24CCTL2 |= SD24SNGL;     // Single conversion mode
        SD24CCTL2 &= ~SD24GRP;     // Single channel mode

        SD24CCTL2 &= ~(SD24OSR1 | SD24OSR0);  // Ensure Oversampling Ratio = 256

        SD24CCTL2 &= ~SD24LSBTOG; // Disable toggling SD24 Least-Significant Bits Access
        SD24CCTL2 &= ~SD24LSBACC;   // Data read from Most-Significant Bits

        SD24CCTL2 |= SD24DF;        // Two's complement mode chosen

        SD24INCTL2 &= ~(SD24GAIN2 | SD24GAIN1 | SD24GAIN0); // Pre-amplifier gain = 1
        POWER_METER.GAIN_CHN_TEM = 1;

        SD24INCTL2 |= (SD24INCH2 | SD24INCH1); // Choose A0.6 as input source - internal temperature sensor
        SD24INCTL2 &= ~SD24INCH0;

        SD24CCTL2 |= (SD24SC | SD24IE); // Enable interrupt and start conversions
        /* End of Channel 2 initiallization */

        /* Initiate configurations for SD24 channel 0 */
        SD24CCTL0 &= ~SD24SNGL;     // Continuous conversion mode
        SD24CCTL0 |= SD24GRP; // Multiple channel mode, Channel 0 is a follower.

        SD24CCTL0 &= ~(SD24OSR1 | SD24OSR0);  // Ensure Oversampling Ratio = 256

        SD24CCTL0 &= ~SD24LSBTOG; // Disable toggling SD24 Least-Significant Bits Access
        SD24CCTL0 &= ~SD24LSBACC;   // Data read from Most-Significant Bits

        SD24CCTL0 &= ~SD24DF;       // Offset binary mode chosen

        SD24INCTL0 &= ~(SD24GAIN0 | SD24GAIN1); // Pre-amplifier gain = 16
        SD24INCTL0 |= SD24GAIN2;
        POWER_METER.GAIN_CHN_V = 16;

        SD24INCTL0 &= ~(SD24INCH2 | SD24INCH1 | SD24INCH0); // Choose A0.0 as input source
        /* End of Channel 0 initiallization */

        /* Initiate configurations for SD24 channel 1 */
        SD24CCTL1 &= ~SD24SNGL;     // Continuous conversion mode
        SD24CCTL1 &= ~SD24GRP; // Multiple channel mode, Channel 1 is the master.

        SD24CCTL1 &= ~(SD24OSR1 | SD24OSR0);  // Ensure Oversampling Ratio = 256

        SD24CCTL1 &= ~SD24LSBTOG; // Disable toggling SD24 Least-Significant Bits Access
        SD24CCTL1 &= ~SD24LSBACC;   // Data read from Most-Significant Bits

        SD24CCTL1 &= ~SD24DF;       // Offset binary mode chosen

        SD24INCTL1 &= ~(SD24GAIN2 | SD24GAIN1 | SD24GAIN0); // Pre-amplifier gain = 1
        POWER_METER.GAIN_CHN_I = 1;

        SD24INCTL1 &= ~(SD24INCH2 | SD24INCH1 | SD24INCH0); // Choose A0.0 as input source

        SD24CCTL1 |= (SD24SC | SD24IE); // Enable interrupt and start conversions
        SD24CCTL2 |= SD24SC;
        /* End of Channel 1 initiallization */

        __delay_cycles(3277); // MCLK = 16.384MHz by default is used for this delay function. Total delay time = 200.01us (since ~200us is required)
    }
    else
        SD24CCTL1 |= SD24SC;    // Start conversions for second run onwards
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
        POWER_METER.sampling.voltage[POWER_METER.INDEX] = SD24MEM0;
        POWER_METER.sampling.current[POWER_METER.INDEX] = SD24MEM1;

        POWER_METER.ResultReady = ACTIVE_STATE;
        break;
    case SD24IV_SD24MEM2:
        POWER_METER.sampling.temperature = SD24MEM2;
        break;
    case SD24IV_SD24MEM3:
        break;
    default:
        break;
    }
}

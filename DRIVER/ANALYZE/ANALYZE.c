/*
 * ANALYZE.c
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

#include "ANALYZE.h"

void RAW_DATA_PROCESSING(void)
{
    for (INDEX = 0; INDEX <= POWER_METER.END_POINT; INDEX++)
    {
        POWER_METER.processed.voltage[INDEX] = VALUE(POWER_METER.sampling.voltage, 0, POWER_METER.processed.Vref);
        POWER_METER.processed.current[INDEX] = VALUE(POWER_METER.sampling.current, 0, POWER_METER.processed.Vref) / 0.0005;
    }

    POWER_METER.processed.temperature[0] = (VALUE(POWER_METER.sampling.temperature, 0, POWER_METER.processed.Vref) - POWER_METER.TemperatureOffset) / POWER_METER.TCsensor;
    POWER_METER.processed.temperature[1] = POWER_METER.processed.temperature[0] - 273;
    POWER_METER.processed.temperature[2] = 1.8 * POWER_METER.processed.temperature[1] + 32;
}

void FREQUENCY(void)
{
    POWER_METER.processed.frequency = SAMPLING_RATE / (POWER_METER.END_POINT + 1);
}

void RMS_VALUES(void)
{
    POWER_METER.processed.V_rms = 0;
    POWER_METER.processed.I_rms = 0;

    for (INDEX = 0; INDEX <= POWER_METER.END_POINT; INDEX++)
    {
        POWER_METER.processed.V_rms += pow(POWER_METER.processed.voltage[INDEX], 2);
        POWER_METER.processed.I_rms += pow(POWER_METER.processed.current[INDEX], 2);
    }

    POWER_METER.processed.V_rms = POWER_METER.GAIN_CHN_V * sqrt((1.0/(POWER_METER.END_POINT + 1)) * POWER_METER.processed.V_rms);
    POWER_METER.processed.I_rms = POWER_METER.GAIN_CHN_I * sqrt((1.0/(POWER_METER.END_POINT + 1)) * POWER_METER.processed.I_rms);
}

void POWER_AND_ENERGY(void)
{
    POWER_METER.processed.power_real = POWER_METER.processed.V_rms * POWER_METER.processed.I_rms;

    for (INDEX = 0; INDEX <= POWER_METER.END_POINT; INDEX++)
    {
        POWER_METER.processed.power_active = POWER_METER.processed.voltage[INDEX] * POWER_METER.processed.current[INDEX];
    }

    POWER_METER.processed.power_active *= (POWER_METER.GAIN_CHN_V * POWER_METER.GAIN_CHN_I * (1.0 / (POWER_METER.END_POINT + 1)));

    POWER_METER.processed.power_factor = POWER_METER.processed.power_active / POWER_METER.processed.power_real;

    POWER_METER.processed.energy += (POWER_METER.processed.power_active * DELTA_T(POWER_METER.SamplingDuration));
}

void ANALYZE_LL(void)
{
    RAW_DATA_PROCESSING();
    FREQUENCY();
    RMS_VALUES();
    POWER_AND_ENERGY();
}

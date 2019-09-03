/*
 * ANALYZE.c
 *
 *  Created on: August 23, 2019
 *      Author: Z3r0
 *
 *  Software description:
 *      ENDPOINT_BREAKCASE(..): Process to be done at the end of 1 sampled AC cycle, called by SAMPLING_PROCESS(..) only.
 *      SAMPLING_PROCESS(..): Analysis process whenever there is a new SD24 conversion result.
 *      RAW_DATA_PROCESSING(..): Convert from raw ADC data of voltage and current input to their real values.
 *                               Calculate internal temperature of MSP430i2041 microcontroller in Kelvin, degree Celsius, and degree Fahrenheit.
 *      FREQUENCY(..): Calculate frequency of the input AC signals.
 *      RMS_VALUES(..): Calculate V_rms and I_rms of the measured system.
 *      POWER_AND_ENERGY(..): Calculate apparent power, active power, power factor of the measured system.
 *                            Calculate power consumption on the whole system.
 *      ANALYZE_LL(..): Function that calls all above functions to perform first-hand analysis of the measured system.
 */

#include "ANALYZE.h"

void ENDPOINT_BREAKCASE(void)
{
    SD24CCTL1 &= ~SD24SC;

    //TIMER_0_stop();
    //POWER_METER.SamplingDuration = (COUNTER.COUNT * 0.004) + (TA0R / 16384000.0);

    //TIMER_0_init();

    POWER_METER.END_POINT = POWER_METER.INDEX;

    POWER_METER.ConversionFlag = INACTIVE_STATE;

    for (POWER_METER.INDEX = 0; POWER_METER.INDEX < POWER_METER.END_POINT; POWER_METER.INDEX++)
    {
        Printf(POWER_METER.sampling.voltage[POWER_METER.INDEX]);
    }

    SD24CCTL2 |= SD24SC;
}

void SAMPLING_PROCESS(void)
{
    POWER_METER.ResultReady = INACTIVE_STATE;
    GPIO_operation(LED_RUN, LED_TOGGLE);

    if (0 == POWER_METER.INDEX)
    {
        switch (POWER_METER.FIRST_RUN)
        {
        case 0:     // 0 == FIRST_RUN   -> SIGN not assigned
            if (ZERO_CONDITION_HIGH(POWER_METER.sampling.voltage[POWER_METER.INDEX])) // Check zero condition
            {
                //TIMER_0_init();         // Start Timer

                POWER_METER.SIGN = 1;
                POWER_METER.INDEX++;

                POWER_METER.FIRST_RUN = 1;
            }
            else if (ZERO_CONDITION_LOW(POWER_METER.sampling.voltage[POWER_METER.INDEX])) // Check zero condition
            {
                //TIMER_0_init();         // Start Timer

                POWER_METER.SIGN = 0;
                POWER_METER.INDEX++;

                POWER_METER.FIRST_RUN = 1;
            }
            break;  // End of case 0, switch (FIRST_RUN)
        case 1:     // 1 == FIRST_RUN   -> SIGN assigned
            switch (POWER_METER.SIGN)
            {
            case 0:     // 0 == SIGN
                if (ZERO_CONDITION_LOW(POWER_METER.sampling.voltage[POWER_METER.INDEX])) // Check zero condition
                {
                    //TIMER_0_init();         // Start Timer
                    POWER_METER.INDEX++;
                }
                break;  // End of case 0, switch(SIGN)
            case 1:     // 1 == SIGN
                if (ZERO_CONDITION_HIGH(POWER_METER.sampling.voltage[POWER_METER.INDEX])) // Check zero condition
                {
                    //TIMER_0_init();         // Start Timer
                    POWER_METER.INDEX++;
                }
                break;  // End of case 1, switch(SIGN)
            default:    // default case, never occurs
                break;  // End of default case, switch(SIGN)
            }
            break;
        default:    // default case, never occurs
            break;  // End of default, switch (FIRST_RUN)

        }
    }
    else if (70 <= POWER_METER.INDEX)
    {
        switch (POWER_METER.SIGN)
        {
        case 0:     // 0 == SIGN
            if (ZERO_CONDITION_LOW(POWER_METER.sampling.voltage[POWER_METER.INDEX])) // Check zero condition
                ENDPOINT_BREAKCASE();
            else
                POWER_METER.INDEX++;
            break;  // End of case 0, switch(SIGN)
        case 1:     // 1 == SIGN
            if (ZERO_CONDITION_HIGH(POWER_METER.sampling.voltage[POWER_METER.INDEX])) // Check zero condition
                ENDPOINT_BREAKCASE();
            else
                POWER_METER.INDEX++;
            break;  // End of case 1, switch(SIGN)
        default:    // default case, never occurs
            break;  // End of default case, switch(SIGN)
        }
    }
    else    // 0 < INDEX < 70
        POWER_METER.INDEX++;
}

double_t VALUE(uint16_t RAW, double_t OFFSET)
{
    return (((RAW * POWER_METER.processed.Vref) / 0x7FFF) - OFFSET);
}

void RAW_DATA_PROCESSING(void)
{
    for (POWER_METER.INDEX = 0; POWER_METER.INDEX <= POWER_METER.END_POINT; POWER_METER.INDEX++)
    {
        POWER_METER.processed.voltage[POWER_METER.INDEX] = VALUE(POWER_METER.sampling.voltage[POWER_METER.INDEX], 0) * 661;
        POWER_METER.processed.current[POWER_METER.INDEX] = VALUE(POWER_METER.sampling.current[POWER_METER.INDEX], 0) / 0.0005;
    }

    POWER_METER.processed.temperature[0] = (POWER_METER.sampling.temperature * 1200.0) / 70711;
    POWER_METER.processed.temperature[1] = POWER_METER.processed.temperature[0] - 273;
    POWER_METER.processed.temperature[2] = 32 + (1.8 * POWER_METER.processed.temperature[1]);
}

void FREQUENCY(void)
{
    POWER_METER.processed.frequency = SAMPLING_RATE / (POWER_METER.END_POINT + 1);
}

void RMS_VALUES(void)
{
    POWER_METER.processed.V_rms = 0;
    POWER_METER.processed.I_rms = 0;

    for (POWER_METER.INDEX = 0; POWER_METER.INDEX <= POWER_METER.END_POINT; POWER_METER.INDEX++)
    {
        POWER_METER.processed.V_rms += pow(POWER_METER.processed.voltage[POWER_METER.INDEX], 2);
        POWER_METER.processed.I_rms += pow(POWER_METER.processed.current[POWER_METER.INDEX], 2);
    }

    POWER_METER.processed.V_rms = (POWER_METER.GAIN_CHN_V * sqrt((1.0/(POWER_METER.END_POINT + 1)) * POWER_METER.processed.V_rms));
    POWER_METER.processed.I_rms = (POWER_METER.GAIN_CHN_I * sqrt((1.0/(POWER_METER.END_POINT + 1)) * POWER_METER.processed.I_rms));
}

void POWER_AND_ENERGY(void)
{
    POWER_METER.processed.power_real = POWER_METER.processed.V_rms * POWER_METER.processed.I_rms;

    for (POWER_METER.INDEX = 0; POWER_METER.INDEX <= POWER_METER.END_POINT; POWER_METER.INDEX++)
    {
        POWER_METER.processed.power_active = POWER_METER.processed.voltage[POWER_METER.INDEX] * POWER_METER.processed.current[POWER_METER.INDEX];
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

/*
 * PROCESS.c
 *
 *  Created on: July 15, 2019
 *      Author: Z3r0
 */

#include "PROCESS.h"

void OPERATION(void)
{
    GPIO_init();
    UART_init();
    SD24_init();

    __bis_SR_register(GIE);

    while (1)
    {
        if (INACTIVE_STATE == POWER_METER.ConversionFlag)
        {
            GPIO_operation(LED_RUN, LED_ON);
//            ANALYZE_LL();
//
//            UART_input_String(" f = ", sizeof(" f = "));
//            UART_input_number(POWER_METER.processed.frequency);
//            UART_input_String("Hz ||", sizeof("Hz ||"));
//
//            UART_input_String(" U = ", sizeof(" U = "));
//            UART_input_number(POWER_METER.processed.V_rms);
//            UART_input_String("V ||", sizeof("V ||"));
//
//            UART_input_String(" I = ", sizeof(" I = "));
//            UART_input_number(POWER_METER.processed.I_rms);
//            UART_input_String("A ||", sizeof("A ||"));
//
//            UART_input_String(" P = ", sizeof(" P = "));
//            UART_input_number(POWER_METER.processed.power_real);
//            UART_input_String("W ||", sizeof("W ||"));
//
//            UART_input_String(" PF = ", sizeof(" PF = "));
//            UART_input_number(POWER_METER.processed.power_factor);
//            UART_input_String("%", sizeof("%"));
//
//            UART_input_String(" E = ", sizeof(" E = "));
//            UART_input_number(POWER_METER.processed.energy);
//            UART_input_String("Wh ||", sizeof("Wh ||"));
//            UART_input_String(NLCR, 3);
//
//            UART_input_String(" T = ", sizeof(" T = "));
//            UART_input_number(POWER_METER.processed.temperature[1]);
//            UART_input_String("degC ||", sizeof("degC ||"));
//
//            GPIO_operation(LED_RUN, LED_OFF);

//            POWER_METER.SamplingDuration *= (2.0/3);
//
//            do
//            {
//                POWER_METER.ProcessDuration = (COUNTER.COUNT * 0.004) + (TA0R / 16384000.0);
//            } while (POWER_METER.ProcessDuration < POWER_METER.SamplingDuration);
//
//            TIMER_0_stop();

            __delay_cycles(218453);

            SD24_init();
        }
        else if (ACTIVE_STATE == POWER_METER.ResultReady)
        {
            POWER_METER.ResultReady = INACTIVE_STATE;
            SAMPLING_PROCESS();

//            POWER_METER.INDEX++;
//            if (ARRAY_SIZE == POWER_METER.INDEX)
//            {
//                SD24CCTL1 &= ~SD24SC;
//
//                for (POWER_METER.INDEX = 0; POWER_METER.INDEX < ARRAY_SIZE; POWER_METER.INDEX++)
//                {
//                    Printf(POWER_METER.sampling.voltage[POWER_METER.INDEX]);
//                }
//
//                SD24_init();
//            }
        }
    }
}


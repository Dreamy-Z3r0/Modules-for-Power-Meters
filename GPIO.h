/*
 * GPIO.h
 *
 *  Created on: July 15, 2019
 *      Author: Z3r0
 *
 *  Hardware description:
 *      LED on pin P1.5 : RUN
 *      LED on pin P1.4 : Error indicator
 *      * LEDs are active-LOW.
 *
 *  Software description:
 *      GPIO_init(..) : initiate GPIO function
 *      GPIO_operation(..) : choose desired LED to work with, as well as desired operation
 *      set_MCLK(..) : set up MCLK for delay functions
 *      delay_sec(..) : delay the whole program for a set duration (in seconds)
 *      delay_ms(..) : delay the whole program for a set duration (in milliseconds)
 */

#ifndef DRIVER_GPIO_GPIO_H_
#define DRIVER_GPIO_GPIO_H_

#define LED_RUN     0x00
#define LED_ERROR   0x01

#define LED_OFF     0x00
#define LED_ON      0x01
#define LED_TOGGLE  0x02

void GPIO_init(void);
void GPIO_operation(uint8_t led, uint8_t operation);

void set_MCLK(void);
void delay_sec(uint32_t duration)
void delay_ms(uint32_t duration);

#endif /* DRIVER_GPIO_GPIO_H_ */

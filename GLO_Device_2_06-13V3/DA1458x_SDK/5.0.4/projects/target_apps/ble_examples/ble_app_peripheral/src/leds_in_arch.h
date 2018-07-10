#ifndef LEDS_IN_ARCH_H
#define LEDS_IN_ARCH_H



#include "user_periph_setup.h"       // peripheral configuration



#define LED_POWER_EN_PORT     GPIO_PORT_1
#define LED_POWER_EN_PIN      GPIO_PIN_2

//#define LED_LBO_PORT     GPIO_PORT_1
//#define LED_LBO_PIN      GPIO_PIN_1

#define USE_IN_DEC_CHECK 1//0

#if USE_IN_DEC_CHECK
#define IN_DEC_PORT     GPIO_PORT_1//GPIO_PORT_2
#define IN_DEC_PIN      GPIO_PIN_3//GPIO_PIN_4
#endif



void LED_Power_Control(char led_power_state);





#endif



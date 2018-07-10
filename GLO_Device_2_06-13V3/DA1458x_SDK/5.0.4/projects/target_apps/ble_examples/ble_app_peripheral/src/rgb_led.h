#ifndef RGB_LED
#define RGB_LED
#define NONECONNECTBOARD 1
#include "gpio.h"

#if (NONECONNECTBOARD)
	#define GPIO_LEDR_PORT     GPIO_PORT_0
	#define GPIO_LEDR_PIN      GPIO_PIN_4//GPIO_PIN_4

	#define GPIO_LEDG_PORT     GPIO_PORT_0
	#define GPIO_LEDG_PIN      GPIO_PIN_5

	#define GPIO_LEDB_PORT     GPIO_PORT_2
	#define GPIO_LEDB_PIN      GPIO_PIN_1

#else
	#define GPIO_LEDR_PORT     GPIO_PORT_0
	#define GPIO_LEDR_PIN      GPIO_PIN_5//GPIO_PIN_4

	#define GPIO_LEDW_PORT     GPIO_PORT_0
	#define GPIO_LEDW_PIN      GPIO_PIN_4

	#define GPIO_LEDB_PORT     GPIO_PORT_2
	#define GPIO_LEDB_PIN      GPIO_PIN_1

#endif

typedef enum
{
	BLACK,
	RED,
//	GREEN,
	BLUE,
	WHITE
}_LED_COLOR;



void RGB_LED_GPIO_reservations(void);
void set_rgb_led_pad_functions(void);
void set_led_color(char color);


#endif



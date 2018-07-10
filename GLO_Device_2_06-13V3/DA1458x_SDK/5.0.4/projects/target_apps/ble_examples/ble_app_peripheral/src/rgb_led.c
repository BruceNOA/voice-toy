#include "rgb_led.h"



void RGB_LED_GPIO_reservations(void)
{
	RESERVE_GPIO(LEDR, GPIO_LEDR_PORT, GPIO_LEDR_PIN, PID_GPIO);
#if (NONECONNECTBOARD)
	RESERVE_GPIO(LEDG, GPIO_LEDG_PORT, GPIO_LEDG_PIN, PID_GPIO);
#else 
	RESERVE_GPIO(LEDW, GPIO_LEDW_PORT, GPIO_LEDW_PIN, PID_GPIO);
#endif
	RESERVE_GPIO(LEDB, GPIO_LEDB_PORT, GPIO_LEDB_PIN, PID_GPIO);
}

void set_rgb_led_pad_functions(void)        // set gpio port function mode
{
	GPIO_ConfigurePin(GPIO_LEDR_PORT, GPIO_LEDR_PIN, OUTPUT, PID_GPIO, false);
#if (NONECONNECTBOARD)
	GPIO_ConfigurePin(GPIO_LEDG_PORT, GPIO_LEDG_PIN, OUTPUT, PID_GPIO, false);
#else 
	GPIO_ConfigurePin(GPIO_LEDW_PORT, GPIO_LEDW_PIN, OUTPUT, PID_GPIO, false);
#endif
	GPIO_ConfigurePin(GPIO_LEDB_PORT, GPIO_LEDB_PIN, OUTPUT, PID_GPIO, false);
}
#if (NONECONNECTBOARD)
void set_led_color(char color)
{
	static char color_old;
	if (color != color_old)
	{
		switch(color)
		{
			case BLACK:
				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetInactive(GPIO_LEDG_PORT, GPIO_LEDG_PIN);
				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
				break;
			case RED:
				GPIO_SetActive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetInactive(GPIO_LEDG_PORT, GPIO_LEDG_PIN);
				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
				break;
//			case GREEN:
//				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
//				GPIO_SetActive(GPIO_LEDG_PORT, GPIO_LEDG_PIN);
//				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
//				break;
			case BLUE:
				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetInactive(GPIO_LEDG_PORT, GPIO_LEDG_PIN);
				GPIO_SetActive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
				break;
			case WHITE:
				GPIO_SetActive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetActive(GPIO_LEDG_PORT, GPIO_LEDG_PIN);
				GPIO_SetActive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);		
				break;
			default:
				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetInactive(GPIO_LEDG_PORT, GPIO_LEDG_PIN);
				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
				break;
		}
		color_old = color;
	}
	
}
#else
void set_led_color(char color)
{
	static char color_old;
	if (color != color_old)
	{
		switch(color)
		{
			case BLACK:
				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetInactive(GPIO_LEDW_PORT, GPIO_LEDW_PIN);
				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
				break;
			case RED:
				GPIO_SetActive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetInactive(GPIO_LEDW_PORT, GPIO_LEDW_PIN);
				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
				break;
//			case GREEN:
//				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
//				GPIO_SetActive(GPIO_LEDW_PORT, GPIO_LEDW_PIN);
//				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
//				break;
			case BLUE:
				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetInactive(GPIO_LEDW_PORT, GPIO_LEDW_PIN);
				GPIO_SetActive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
				break;
			case WHITE:
				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetActive(GPIO_LEDW_PORT, GPIO_LEDW_PIN);
				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);		
				break;
			default:
				GPIO_SetInactive(GPIO_LEDR_PORT, GPIO_LEDR_PIN);
				GPIO_SetInactive(GPIO_LEDW_PORT, GPIO_LEDW_PIN);
				GPIO_SetInactive(GPIO_LEDB_PORT, GPIO_LEDB_PIN);
				break;
		}
		color_old = color;
	}
	
}
#endif	





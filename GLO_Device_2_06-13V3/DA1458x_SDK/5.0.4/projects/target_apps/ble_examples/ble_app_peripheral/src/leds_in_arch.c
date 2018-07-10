#include "leds_in_arch.h"
#include "user_flash.h"


//char battery_low_power_flg = 0;
char in_dec_flg = 0;
char led_power_on_state = 0;

/* Control the LED power */
void LED_Power_Control(char led_power_state)
{
	static char led_power_state_old = OFF;
	if (led_power_state == led_power_state_old)
	{
		return;
	}
	
	if (ON == led_power_state)
	{

			GPIO_SetActive(LED_POWER_EN_PORT, LED_POWER_EN_PIN);
			led_power_on_state = 1;
			save_usage_data(led_power_state);
	}
	else
	{
		if (led_power_on_state)
		{
			GPIO_SetInactive(LED_POWER_EN_PORT, LED_POWER_EN_PIN);	
			led_power_on_state = 0;
			save_usage_data(led_power_state);	
		}
	}
	
	led_power_state_old = led_power_state;
}





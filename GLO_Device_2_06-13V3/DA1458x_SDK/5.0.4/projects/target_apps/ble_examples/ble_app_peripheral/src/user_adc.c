#include "user_adc.h"


/* Get the average battery_voltage adc value  */
uint16_t adc_get_battery_voltage_sample(void)
{
	uint16_t adc_sample = 0;
	uint16_t adc_sample_avg = 0;
	static uint8_t adc_sample_cnt = 0;
	static uint16_t adc_sample_sum = 0;
	
	GPIO_SetActive(BATT_ADC_CTRL_PORT, BATT_ADC_CTRL_PIN);
	
	adc_calibrate();
	
	adc_init(GP_ADC_SE, GP_ADC_SIGN, GP_ADC_ATTN3X);
  adc_usDelay(20);
	adc_enable_channel(ADC_CHANNEL_P00);
  adc_sample = adc_get_sample();
	adc_disable();
	GPIO_SetInactive(BATT_ADC_CTRL_PORT, BATT_ADC_CTRL_PIN);
	
	adc_sample_sum += adc_sample;
	adc_sample_cnt++;
	if (adc_sample_cnt >= ADC_SAMPLE_TIMES)
	{
		adc_sample_avg = adc_sample_sum / adc_sample_cnt;
		adc_sample_sum = 0;
		adc_sample_cnt = 0;
		return adc_sample_avg;
	}
	else
	{
		return ADC_SAMPLE_VALUE_INVALID;
	}
}

/* Calculating the battery voltage according to adc value  */
u16 get_battery_voltage(void)
{
	u16 adc_value;
uint16_t battery_voltage_value = 4200;
//uint16_t battery_voltage_value_raw;
u16 batt_temp = 4200;
	adc_value = adc_get_battery_voltage_sample();
	if (adc_value != 0xffff)
	{
		batt_temp = (u32)adc_value * 3600 / 1023 / 3 * 4;
		battery_voltage_value = batt_temp;//(u32)adc_value * 3600 / 1023 / 3 * 4;
	}
	
	return battery_voltage_value;
	
}

extern char led_power_on_state;
extern char _dev_sta;
u8 old_battery_level = 101;
/* Calculating the battery level according to the battery voltage using battery discharge curve*/
u8 Get_Battery_Level(u16 voltage)
{
	u8 percent;
	u16 vol;
	float temp1;
	
	#if 0
	vol = voltage / 10;
	#else
	if (led_power_on_state == 1) vol = voltage / 10 + 4;
	else vol = voltage / 10 + 4;
	#endif
	
	if(vol >= 420)
	{
		percent = 100;
	}
	else if( vol >= 397 && vol < 420 )
	{
		temp1 =( (vol - 397) * 10 ) / (420-397) + 0.5;
		percent = (u8)temp1 + 90;
	}
	else if(vol >= 387 && vol < 397)
	{
		temp1 =( (vol - 387) * 10 ) / (397-387) + 0.5;
		percent = (u8)temp1 + 80;
	}
	else if(vol >= 379 && vol < 387)
	{
		temp1 =( (vol - 379) * 10 ) / (387-379) + 0.5;
		percent = (u8)temp1 + 70;
	}
	else if(vol >= 373 && vol < 379)
	{
		temp1 =( (vol - 373) * 10 ) / (379-373) + 0.5;
		percent = (u8)temp1 +  60;
	}
	else if(vol >= 368 && vol < 373)
	{
		temp1 =( (vol - 368) * 10 ) / (373-368) + 0.5;
		percent = (u8)temp1 + 50;
	}
	else if(vol >= 342 && vol < 373)
	{
		temp1 =( (vol - 342) * 45 ) / (373-342) + 0.1;
		percent = (u8)temp1 + 5;
	}
	else if(vol >= 300 && vol < 342)
	{
		temp1 =( (vol - 300) * 5 ) / (342-300) + 0.1;
		percent = (u8)temp1 ;
	}	
	else
	{
		percent = 0;
	}
	if (_dev_sta != DEVICE_CHARGING)
	{
		if( percent <= old_battery_level )
		{
			old_battery_level = percent;
		}
		else if(old_battery_level != 101)
		{
			percent = old_battery_level;
		}
	}
	else 
	{
		if( percent >= old_battery_level )
		{
			old_battery_level = percent;
		}
		else if(old_battery_level != 101)
		{
			percent = old_battery_level;
		}
		else
		{
			old_battery_level = percent;
		}
	}
	return percent;
}



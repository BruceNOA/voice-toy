#ifndef USER_ADC_H
#define USER_ADC_H

#include "user_periph_setup.h"       // peripheral configuration
#include "adc.h" 

#define BATT_ADC_CTRL_PORT     GPIO_PORT_2
#define BATT_ADC_CTRL_PIN      GPIO_PIN_2

#define BATT_ADC_PORT     GPIO_PORT_0
#define BATT_ADC_PIN      GPIO_PIN_0

#define CURRENT_ADC_PORT     GPIO_PORT_0
#define CURRENT_ADC_PIN      GPIO_PIN_1



#define ADC_SAMPLE_TIMES 4
#define ADC_SAMPLE_VALUE_INVALID 0xffff

#define CURRENT_SENSOR_ENABLE 0

void adc_usDelay(uint32_t nof_us);
uint16_t adc_get_battery_voltage_sample(void);

u16 get_battery_voltage(void);
u8 Get_Battery_Level(u16 voltage);


#endif



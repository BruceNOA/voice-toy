/**
 ****************************************************************************************
 *
 * @file user_periph_setup.c
 *
 * @brief Peripherals setup and initialization.
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration
#include "user_periph_setup.h"       // peripheral configuration
#include "global_io.h"
#include "gpio.h"
#include "uart.h"                    // UART initialization


#include "user_uart2.h"
#include "app_task.h"
#include "app_easy_timer.h"
#include "rgb_led.h"
#include "user_custs1_impl.h"

#include "user_peripheral.h"
#include "rtc.h"
#include "user_flash.h"
#include "user_adc.h"
#include "leds_in_arch.h"

/**
 ****************************************************************************************
 * @brief Each application reserves its own GPIOs here.
 *
 * @return void
 ****************************************************************************************
 */

#ifdef CFG_DEVELOPMENT_DEBUG


void GPIO_reservations(void)
{
/*
* Globally reserved GPIOs reservation
*/

/*
* Application specific GPIOs reservation. Used only in Development mode (#if DEVELOPMENT_DEBUG)

i.e.
    RESERVE_GPIO(DESCRIPTIVE_NAME, GPIO_PORT_0, GPIO_PIN_1, PID_GPIO);    //Reserve P_01 as Generic Purpose I/O
*/
#ifdef CFG_PRINTF_UART2
    RESERVE_GPIO(UART2_TX, UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, PID_UART2_TX);
    RESERVE_GPIO(UART2_RX, UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, PID_UART2_RX);
#endif
	
		#if USE_UART_PRINT
		RESERVE_GPIO(UART2_TX, UART2_GPIO_PORT, UART2_TX_PIN, PID_UART2_TX);
    RESERVE_GPIO(UART2_RX, UART2_GPIO_PORT, UART2_RX_PIN, PID_UART2_RX);
		#endif
	
		#if 1
		RGB_LED_GPIO_reservations();
		#else
    RESERVE_GPIO(LED, GPIO_LED_PORT, GPIO_LED_PIN, PID_GPIO);
		#endif
	
		#if (BLE_SPOTA_RECEIVER)
    #if !defined(__DA14583__)
    RESERVE_GPIO(SPI_EN,  SPI_EN_GPIO_PORT,  SPI_EN_GPIO_PIN,  PID_SPI_EN);
    RESERVE_GPIO(SPI_CLK, SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN, PID_SPI_CLK);
    RESERVE_GPIO(SPI_DO,  SPI_DO_GPIO_PORT,  SPI_DO_GPIO_PIN,  PID_SPI_DO);
    RESERVE_GPIO(SPI_DI,  SPI_DI_GPIO_PORT,  SPI_DI_GPIO_PIN,  PID_SPI_DI);
    #endif
		#endif
}
#endif // CFG_DEVELOPMENT_DEBUG


void set_pad_functions(void)        // set gpio port function mode
{
#ifdef CFG_PRINTF_UART2
    GPIO_ConfigurePin(UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, OUTPUT, PID_UART2_TX, false);
    GPIO_ConfigurePin(UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, INPUT, PID_UART2_RX, false);
#endif
	
		#if USE_UART_PRINT
		//Init pads
    GPIO_ConfigurePin(UART2_GPIO_PORT, UART2_TX_PIN, OUTPUT, PID_UART2_TX, false);
    GPIO_ConfigurePin(UART2_GPIO_PORT, UART2_RX_PIN, INPUT, PID_UART2_RX, false);
		#endif
	
	
		#if 1
		set_rgb_led_pad_functions();
		#else
    GPIO_ConfigurePin(GPIO_LED_PORT, GPIO_LED_PIN, OUTPUT, PID_GPIO, false);
		#endif
		
		#if (BLE_SPOTA_RECEIVER)
    GPIO_ConfigurePin(SPI_EN_GPIO_PORT,  SPI_EN_GPIO_PIN,  OUTPUT, PID_SPI_EN,  true);
    GPIO_ConfigurePin(SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN, OUTPUT, PID_SPI_CLK, false);
    GPIO_ConfigurePin(SPI_DO_GPIO_PORT,  SPI_DO_GPIO_PIN,  OUTPUT, PID_SPI_DO,  false);
    GPIO_ConfigurePin(SPI_DI_GPIO_PORT,  SPI_DI_GPIO_PIN,  INPUT,  PID_SPI_DI,  false);
		
		#endif
}



void periph_init(void)
{
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));

    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);

    //rom patch
    patch_func();

    //Init pads
    set_pad_functions();

#ifdef CFG_PRINTF_UART2
    SetBits16(CLK_PER_REG, UART2_ENABLE, 1);
    uart2_init(UART_BAUDRATE_115K2, 3);
#endif

		

		#if 1
		RESERVE_GPIO(BUTTON, GPIO_BUTTON_PORT, GPIO_BUTTON_PIN, PID_GPIO);
		RESERVE_GPIO(BATT_ADC_CTRL, BATT_ADC_CTRL_PORT, BATT_ADC_CTRL_PIN, PID_GPIO);
		RESERVE_GPIO(BATT_ADC, BATT_ADC_PORT, BATT_ADC_PIN, PID_ADC);
		
		#if CURRENT_SENSOR_ENABLE
		RESERVE_GPIO(CURR_ADC, CURRENT_ADC_PORT, CURRENT_ADC_PIN, PID_ADC);
		#endif
		
		RESERVE_GPIO(LED_PWR_CTRL, LED_POWER_EN_PORT, LED_POWER_EN_PIN, PID_GPIO);
		
		#if USE_IN_DEC_CHECK
		RESERVE_GPIO(LED_IN_DEC_CTRL, IN_DEC_PORT, IN_DEC_PIN, PID_GPIO);
		#endif
		
		RESERVE_GPIO(POWER_CHECK, GPIO_CHARGE_PORT, GPIO_CHARGE_PIN, PID_GPIO);
		RESERVE_GPIO(CHARGE_CHECK, GPIO_PG_PORT, GPIO_PG_PIN, PID_GPIO);
		
		RTC_GPIO_reservations();
		#endif
		
	
		#if 1
		GPIO_ConfigurePin(GPIO_BUTTON_PORT, GPIO_BUTTON_PIN, INPUT_PULLUP, PID_GPIO, false);
		GPIO_ConfigurePin(BATT_ADC_CTRL_PORT, BATT_ADC_CTRL_PIN, OUTPUT, PID_GPIO, false);
		GPIO_ConfigurePin(BATT_ADC_PORT, BATT_ADC_PIN, INPUT, PID_ADC, false);
		
		#if CURRENT_SENSOR_ENABLE
		GPIO_ConfigurePin(CURRENT_ADC_PORT, CURRENT_ADC_PIN, INPUT, PID_ADC, false);
		#endif
		
		GPIO_ConfigurePin(LED_POWER_EN_PORT, LED_POWER_EN_PIN, OUTPUT, PID_GPIO, false);
		
		#if USE_IN_DEC_CHECK
		GPIO_ConfigurePin(IN_DEC_PORT, IN_DEC_PIN, INPUT_PULLUP, PID_GPIO, false);
		#endif
		
		GPIO_ConfigurePin(GPIO_CHARGE_PORT, GPIO_CHARGE_PIN, INPUT_PULLUP, PID_GPIO, false);
		GPIO_ConfigurePin(GPIO_PG_PORT, GPIO_PG_PIN, INPUT_PULLUP, PID_GPIO, false);
		
		set_rtc_pad_functions();
		#endif
		
		/* UART Init */
		#if USE_UART_PRINT
		// Initialize UART component
    SetBits16(CLK_PER_REG, UART2_ENABLE, 1);      // enable  clock for UART 2
    uart2_init(UART2_BAUDRATE, UART2_DATALENGTH);
		#endif
	
		// Enable the pads
    SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
		
		RTC_Config();
		
		uart2_print_string("periph_init end\r\n");
		
		#if 0
		spi_flash_test();
		#endif
		
}

void charging_display(void)
{
	static char charge_led_flg = 0;
	if (!charge_led_flg)
	{
		charge_led_flg = 1;
		set_led_color(WHITE);
	}
	else
	{
		charge_led_flg = 0;
		set_led_color(BLACK);
	}
}

void count_down_state_display(void)
{
	static char blue_led_flg = 0;
	if (!blue_led_flg)
	{
		blue_led_flg = 1;
		set_led_color(BLUE);
	}
	else
	{
		blue_led_flg = 0;
		set_led_color(BLACK);
	}
}

void pairing_state_display(void)
{
	static char pairing_led_flg = 0;
	if (!pairing_led_flg)
	{
		pairing_led_flg = 1;
		set_led_color(BLUE);
	}
	else
	{
		pairing_led_flg = 0;
		set_led_color(BLACK);
	}
}

ke_msg_id_t timer_user;
long user_time_cnt = 0;

_USAGE_DATA _usage_data_;

char power_off_flg = 0;
int charger_in_delay = 0;
int charger_out_delay = 0;

int charge_delay = 0;
int full_delay = 0;

char _100ms_flg = 0;
char _250ms_flg = 0;
char _500ms_flg = 0;
char _1s_flg = 0;

char _blue_on = 0;
char ble_sta = 0;
char ble_sta_old = 0;

int button_press_debounce_time_cnt = 0;
int button_release_debounce_time_cnt = 0;
char button_state = 0;
char button_state_old = 0;
char button_value = 0;
char button_press = 0;
int button_press_cnt = 0;
int button_long_press_cnt = 0;
char long_press_flg = 0;
#define TIME_BETWEEN_CLICKS 10

extern uint16_t battery_level_value;
uint16_t current_sensor_value;
uint16_t current_sensor_value_sum = 0;
uint8_t  current_sensor_value_cnt = 0;
uint16_t current_sensor_value_avg;

_DEVICE_STATE device_state;

char _dev_sta_pre = DEVICE_POWER_OFF;
char _dev_sta = DEVICE_POWER_OFF;
char _leds_in_arch_state = OFF;
char _sys_sta_leds_color = BLACK;

char  _device_in_use_step = 0;
long  _device_in_use_time = 0;
int   _device_count_down_cnt = 0;
void device_in_use_polling(void)
{
	switch(_device_in_use_step)
	{
		case 0:
			_sys_sta_leds_color = BLUE;
			_device_in_use_time++;
			if (_device_in_use_time >= DEVICE_COUNT_DOWN_TIME_MAX * 20)// 1s = 50ms * 20
			{
				_device_in_use_time = 0;
				_device_count_down_cnt = 0;// reset the counter for next step
				_device_in_use_step++;
			}
			break;
		case 1:
			_device_in_use_time++;
			if (_device_in_use_time >= 5)// 250ms = 50ms * 5
			{
				_device_in_use_time = 0;
				if (_sys_sta_leds_color == BLUE)
				{
					_sys_sta_leds_color = BLACK;
				}
				else //if (_sys_sta_leds_color == BLACK)
				{
					_sys_sta_leds_color = BLUE;
				}
				_device_count_down_cnt++;
				if (_device_count_down_cnt >= DEVICE_COUNT_DOWN_TIME * 4) //DEVICE_COUNT_DOWN_TIME * 1000 / 250
				{
					_device_count_down_cnt = 0;
					_device_in_use_step = 0;
					_dev_sta = DEVICE_USE_STOP;
				}
			}
			break;
		case 2:
			break;	
		default:
			break;
	}
}

char device_pairing_step = 0;
char device_pairing_time = 0;
int device_pairing_cnt = 0;
void device_pairing_polling(void)
{
	switch (device_pairing_step)
	{
		case 0:
			_sys_sta_leds_color = BLUE;
			device_pairing_time++;
			if (device_pairing_time >= 5)
			{
				device_pairing_time = 0;
				device_pairing_step++;
			}
			break;
		case 1:
			_sys_sta_leds_color = BLACK;
			device_pairing_time++;
			if (device_pairing_time >= 5)
			{
				device_pairing_time = 0;
				device_pairing_step = 0;
				device_pairing_cnt++;
				if (device_pairing_cnt >= 360)// 3min = 180s = 360 * 500ms
				{
					device_pairing_cnt = 0;
					_dev_sta = DEVICE_POWER_ON;//back to power on state			
					stop_pairing();
				}
			}
			break;
		case 2:
			break;
		default:
			break;
	}
}



char device_charging_color_flg = 0;
int device_charging_time = 0;
void device_charge_polling(void)
{
	if (device_state.device_charge_state == 1)
	{
		device_charging_time++;
		if (device_charging_time >= 10)
		{
			device_charging_time = 0;
			if (device_charging_color_flg == 0)
			{
				device_charging_color_flg = 1;
				_sys_sta_leds_color = BLACK;
				app_easy_gap_disconnect(0);
				app_easy_gap_advertise_stop();
			}
			else
			{
				device_charging_color_flg = 0;
				_sys_sta_leds_color = WHITE;
			}
		}
	}
	else if (device_state.device_charge_state == 2)
	{
		_sys_sta_leds_color = BLUE;
	}
}

void set_device_state_charging(void)
{
	_sys_sta_leds_color = WHITE;
	device_charging_color_flg = 0;
	device_charging_time = 0;
//	ble_sta_old = APP_DISABLED;
	_dev_sta = DEVICE_CHARGING;
	app_easy_gap_disconnect(0);
	app_easy_gap_advertise_stop();
	
}

int low_battery_time = 0;
char low_battery_color_flg = 0;
void battery_low_power_polling(void)
{
	low_battery_time++;
	if (low_battery_time >= 10)
	{
		low_battery_time = 0;
		if (low_battery_color_flg == 0)
		{
			low_battery_color_flg = 1;
			_sys_sta_leds_color = RED;
		}
		else
		{
			low_battery_color_flg = 0;
			_sys_sta_leds_color = BLACK;
			app_easy_gap_disconnect(0);
			app_easy_gap_advertise_stop();
		}
	}
}

void set_device_state_low_battery(void)
{
	low_battery_time = 0;
	low_battery_color_flg = 0;
	_dev_sta = DEVICE_LOW_BATTERY;
	app_easy_gap_disconnect(0);
	app_easy_gap_advertise_stop();
}

void set_device_state_pairing(void)
{
	device_pairing_step = 0;
	device_pairing_time = 0;
	device_pairing_cnt = 0;
	_dev_sta = DEVICE_PAIRING;
	
//	device_state.device_pairing_state = 1;
}

void stop_pairing(void)
{
//	device_state.device_pairing_state = 0;
	//app_easy_gap_advertise_stop();
}


void set_device_in_use(void)
{
	if (_dev_sta != DEVICE_IN_USE && battery_level_value > 10 && _dev_sta != DEVICE_LOW_BATTERY)
	{
			if (GPIO_GetPinStatus( IN_DEC_PORT, IN_DEC_PIN ))
			{
				_dev_sta = DEVICE_IN_USE;
				_device_in_use_time = 0;
				_device_in_use_step = 0;
			}
	}
}

void set_device_use_stop(void)
{
	if (_dev_sta == DEVICE_IN_USE)
	{
		_dev_sta = DEVICE_USE_STOP;
	}
}

void device_sleep_mode_control(void)
{
	#if 0
	if (device_state.device_power_state == ON)
	{
		arch_set_sleep_mode(ARCH_SLEEP_OFF);
	}
	else
	{
		arch_set_sleep_mode(ARCH_EXT_SLEEP_ON);
	}
	#else
	if ((_sys_sta_leds_color == BLACK) && (_leds_in_arch_state == OFF)) //Low power controller
	
	{
		
		arch_set_sleep_mode(ARCH_EXT_SLEEP_ON);
		
	}
	else
	{
		arch_set_sleep_mode(ARCH_SLEEP_OFF);
		//RTC_GetTime();
	}
	#endif
}
extern char led_power_on_state;
u8 low_battery_timeout=0;
void device_state_polling(void)
{
	switch(_dev_sta)
	{
		case DEVICE_STATE_NONE:
			break;
		case DEVICE_POWER_OFF:
			_sys_sta_leds_color = BLACK;
			_leds_in_arch_state = OFF;
			break;
		case DEVICE_POWER_ON:
			_sys_sta_leds_color = WHITE;
			break;
		case DEVICE_IN_USE:
			if (GPIO_GetPinStatus( IN_DEC_PORT, IN_DEC_PIN ))
			{
				_leds_in_arch_state = ON;
				device_in_use_polling();
			}
			else
			{
				_sys_sta_leds_color = WHITE;
				_leds_in_arch_state = OFF;	
				_dev_sta = 	DEVICE_POWER_ON;
				led_power_on_state = 0;
				GPIO_SetInactive(LED_POWER_EN_PORT, LED_POWER_EN_PIN);
				save_usage_data(led_power_on_state);
			}
			break;
		case DEVICE_USE_STOP:
			if (GPIO_GetPinStatus( IN_DEC_PORT, IN_DEC_PIN ))
			{
				_sys_sta_leds_color = BLUE;
				_leds_in_arch_state = OFF;
			}
			else
			{
				_sys_sta_leds_color = WHITE;
				_leds_in_arch_state = OFF;	
				_dev_sta = 	DEVICE_POWER_ON;
				led_power_on_state = 0;
				GPIO_SetInactive(LED_POWER_EN_PORT, LED_POWER_EN_PIN);
				save_usage_data(led_power_on_state);				
			}
			break;
		case DEVICE_CHARGING:
			_leds_in_arch_state = OFF;
			device_charge_polling();
			break;
		case DEVICE_PAIRING:
			device_pairing_polling();
			break;
		case DEVICE_CONNECTED:
			_sys_sta_leds_color = BLUE;
			break;
		case DEVICE_LOW_BATTERY:
			low_battery_timeout++;
			if(low_battery_timeout<200) //50*100=5s*2=10s
			{
				_leds_in_arch_state = OFF;
				battery_low_power_polling();
			}
			else
			{
				low_battery_timeout = 0;
							_dev_sta = DEVICE_POWER_OFF;
							//user_app_adv_start();
							app_easy_gap_disconnect(0);
							app_easy_gap_advertise_stop();
							device_state.device_power_state = OFF;
				//low_battery_timeout_flag =1;
			}
			break;
		default:
			break;
	}
	
	device_sleep_mode_control();
	
	set_led_color(_sys_sta_leds_color);
	LED_Power_Control(_leds_in_arch_state);
	
}
extern u8 old_battery_level ;
u8 battery_low_flag = 0;
u16 batt_test = 0;
void app_user_timer_cb_handler(void)//excute every 50ms
{
		timer_user = app_easy_timer(APP_USER_TIMER_DELAY, app_user_timer_cb_handler);
		
		/* Button function */
		//if (device_state.device_charge_state == 0)
		if (_dev_sta != DEVICE_CHARGING)
		{
			//if (GPIO_GetPinStatus( GPIO_BUTTON_PORT, GPIO_BUTTON_PIN ))
			if (!GPIO_GetPinStatus( GPIO_BUTTON_PORT, GPIO_BUTTON_PIN ))
			{
				button_release_debounce_time_cnt = 0;
				button_press_debounce_time_cnt++;
				if (button_press_debounce_time_cnt >= 2)
				{
					button_press_debounce_time_cnt = 0;
					button_state = 1;
					button_press_cnt = TIME_BETWEEN_CLICKS;
				}
				button_long_press_cnt++;
				if (button_long_press_cnt >= 100)
				{
					button_long_press_cnt = 0;
					if (long_press_flg == 0)
					{
						long_press_flg = 1;
						ble_sta = ke_state_get(TASK_APP);
						/* Add long press function here */
						if ((_dev_sta == DEVICE_POWER_ON || _dev_sta == DEVICE_USE_STOP) && (ble_sta != APP_CONNECTED))
						{
							set_device_state_pairing();
							//user_app_adv_start();
						}
						else if(_dev_sta == DEVICE_PAIRING)
						{
							_dev_sta = DEVICE_POWER_ON;//back to power on state
							
							stop_pairing();
						}
					}
					
				}
			}
			else
			{
				button_long_press_cnt = 0;
				button_press_debounce_time_cnt = 0;
				button_release_debounce_time_cnt++;
				if (button_release_debounce_time_cnt >= 2)
				{
					button_release_debounce_time_cnt = 0;
					button_state = 0;
				}
			}
		}
		
		if (button_state != button_state_old)
		{
			if (button_state)
			{
				button_value++;
			}
			button_state_old = button_state;
		}
		
		if (!button_state)//released 
		{
			if (button_press_cnt != 0)//no press in 500ms
			{
				button_press_cnt--;
				if (button_press_cnt == 0)
				{
					if (long_press_flg)
					{
						long_press_flg = 0;
					}
					else 
					{
						button_press = button_value;//just for debugging
						if ((button_value == 1) &&  (_dev_sta != DEVICE_PAIRING) && (_dev_sta != DEVICE_LOW_BATTERY))
						{
							/* Add single press function here */
							if (_dev_sta == DEVICE_POWER_OFF)
							{
								_dev_sta = DEVICE_POWER_ON;
//								old_battery_level = 100;
								device_state.device_power_state = ON;
								arch_set_sleep_mode(ARCH_SLEEP_OFF);
								user_app_adv_start();
							}
							else
							{
								if (_dev_sta != DEVICE_IN_USE)
								{
									set_device_in_use();
								}
								else
								{
									user_time_cnt = 0;
									set_device_use_stop();
								}
							}
						}
						else if (button_value == 2)
						{
							/* Add double press function here */
							
							if(_dev_sta == DEVICE_PAIRING)
							{
								stop_pairing();
							}
							_dev_sta = DEVICE_POWER_OFF;
							//user_app_adv_start();
							app_easy_gap_disconnect(0);
							app_easy_gap_advertise_stop();
							device_state.device_power_state = OFF;
						}
					}
					button_value = 0;
				}
			}
		}
		
		device_state_polling();
		
		user_time_cnt++;
		if (user_time_cnt >= 20) 
		{
			user_time_cnt = 0;
			_1s_flg = 1;
		}
		
		if (user_time_cnt % 2 == 0) //50*2
		{
			_100ms_flg = 1;
		}
		if (user_time_cnt % 5 == 0)
		{
			_250ms_flg = 1;
		}
		if (user_time_cnt % 10 == 0)
		{
			_500ms_flg = 1;
		}
		
		if (_100ms_flg)
		{
			_100ms_flg = 0;
			
			/* Check charging */
			if (!GPIO_GetPinStatus( GPIO_PG_PORT, GPIO_PG_PIN ))
			{
				charger_out_delay = 0;
				charger_in_delay++;
				if (charger_in_delay >= 2)
				{
					charger_in_delay = 0;
					device_state.device_charger_state = 1;
				}
			}
			else
			{
				charger_in_delay = 0;
				charger_out_delay++;
				if (charger_out_delay >= 2)
				{
					charger_out_delay = 0;
					
					device_state.device_charge_state = 0;
					if (device_state.device_charger_state)
					{
						device_state.device_charger_state = 0;
						if (device_state.device_power_state == OFF)
						{
							_dev_sta = DEVICE_POWER_OFF;
							app_easy_gap_advertise_stop();
						}
						else
						{
							_dev_sta = DEVICE_POWER_ON;
							user_app_adv_start();
						}
						
						if (device_state.device_connected_state)
						{
							_dev_sta = DEVICE_CONNECTED;
						}
					}
				}
			}
			
			if (device_state.device_charger_state)
			{
				if (!GPIO_GetPinStatus( GPIO_CHARGE_PORT, GPIO_CHARGE_PIN ))
				{
					full_delay = 0;
					charge_delay++;
					if (charge_delay >= 2)
					{
						charge_delay = 0;
						if (device_state.device_charge_state != 1)
						{
							device_state.device_charge_state = 1;
							set_device_state_charging();
						}
					}
				}
				else
				{
					charge_delay = 0;
					full_delay++;
					if (full_delay >= 2)
					{
						full_delay = 0;
						if (device_state.device_charge_state != 2)
						{
							device_state.device_charge_state = 2;
							
						}
					}
				}
			}
			
		}
		if (_250ms_flg)//_250ms_flg//_1s_flg
		{
			_250ms_flg = 0;
			/* Battery level value */
			
			#if 1
				batt_test = get_battery_voltage();
				battery_level_value = Get_Battery_Level(batt_test);
			
			if ((_dev_sta != DEVICE_CHARGING)&& (device_state.device_power_state == ON))//((_dev_sta != DEVICE_CHARGING) && (device_state.device_power_state == ON))
			{
				if (battery_level_value <= 10)
				{
					if (_dev_sta != DEVICE_LOW_BATTERY)
					{
						if(battery_low_flag == 4)
						{
							set_device_state_low_battery();
							battery_low_flag = 0;
						}
						else
							battery_low_flag++;
					}
				}
				else
				{
					battery_low_flag = 0;
					if (_dev_sta == DEVICE_LOW_BATTERY)
					{
						if (device_state.device_power_state == OFF)
						{
							_dev_sta = DEVICE_POWER_OFF;
							app_easy_gap_advertise_stop();
						}
						else
						{
							_dev_sta = DEVICE_POWER_ON;
							user_app_adv_start();
						}
						
					}
				}
			}
			#endif
			
		}
		if (_500ms_flg)
		{
			_500ms_flg = 0;
			if (_dev_sta != DEVICE_CHARGING)
			{			
				ble_sta = ke_state_get(TASK_APP);
				
				if (ble_sta != ble_sta_old)
				{
					if ( ble_sta == APP_CONNECTED || ble_sta == APP_SECURITY || ble_sta == APP_PARAM_UPD || ble_sta == APP_STATE_MAX)// || ble_sta == APP_SECURITY )
					{
						if( _dev_sta == DEVICE_POWER_ON )
						_dev_sta = DEVICE_CONNECTED;
						device_state.device_connected_state = 1; 
					}
					else if( ble_sta_old == APP_CONNECTED || ble_sta_old == APP_SECURITY || ble_sta_old == APP_PARAM_UPD || ble_sta_old == APP_STATE_MAX)
					{
							close_batterytimer_using();
						if(_dev_sta == DEVICE_CONNECTED)
						{
							_dev_sta = DEVICE_POWER_ON;
						}
							user_app_adv_start();	
						device_state.device_connected_state = 0; 
					}
					ble_sta_old = ble_sta;
				}
			}
			
		}
		
}




/**
 ****************************************************************************************
 *
 * @file user_custs1_impl.c
 *
 * @brief Peripheral project Custom1 Server implementation source code.
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

#include "gpio.h"
#include "app_api.h"
#include "app.h"
#include "user_custs1_def.h"
#include "user_custs1_impl.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
#include "rgb_led.h"
#include "rtc.h"
#include "user_flash.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

ke_msg_id_t datatimer_used;
ke_msg_id_t batterytimer_used;
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


uint8_t val[16];
u8 year,month,day,weekday,hour,min,sec;
uint16_t battery_level_value = 100;
uint16_t batt_sample = 101;
void user_custs1_ctrl_wr_ind_handler(ke_msg_id_t const msgid,
                                      struct custs1_val_write_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
		
//    uint8_t val = 0;
	if(param->length <= 16)
	{
    memcpy(val, param->value, param->length);

    if (val[0] == 'R' && val[1] == 'D' && param->length == 2 )
    {
        datatimer_used = app_easy_timer(50, app_data_timer_cb_handler);
			
				get_usage_data_length();// Read the usage data length for uploading the usage data to APP
    }
		else if(val[0] == 'B' && val[1] == 'T' && param->length == 2)
		{
				batt_sample = 101;
				batterytimer_used = app_easy_timer(10, Battery_level_upload);
		}
    else if(val[0] == 'B' && val[1] == 'P' && param->length == 2)
		{
				if (batterytimer_used != EASY_TIMER_INVALID_TIMER)
				{
					app_easy_timer_cancel(batterytimer_used);
          batterytimer_used = EASY_TIMER_INVALID_TIMER;
				}			
		}
    else if(val[0] == 'L' && val[1] == 'T' && param->length == 2)
		{
			set_device_in_use();
		}
    else if(val[0] == 'L' && val[1] == 'P' && param->length == 2)
		{
			set_device_use_stop();
		}
		else if(val[0] == 'T' && val[1] == '=' && param->length == 16 )
		{
			year = 		(val[2]-'0')*10 + (val[3]-'0');
			month = 	(val[4]-'0')*10 + (val[5]-'0');
			day = 		(val[6]-'0')*10 + (val[7]-'0');
			weekday = (val[8]-'0')*10 + (val[9]-'0');
			hour = 		(val[10]-'0')*10 + (val[11]-'0');
			min = 		(val[12]-'0')*10 + (val[13]-'0');
			sec = 		(val[14]-'0')*10 + (val[15]-'0');
			RTC_SetTime(year,month,day,weekday,hour,min,sec);
		}
		
	}
}
void close_batterytimer_using(void)
{
				if (batterytimer_used != EASY_TIMER_INVALID_TIMER)
				{
					app_easy_timer_cancel(batterytimer_used);
          batterytimer_used = EASY_TIMER_INVALID_TIMER;
				}	
}
void user_custs1_led_wr_ind_handler(ke_msg_id_t const msgid,
                                     struct custs1_val_write_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    uint8_t val = 0;
    memcpy(&val, &param->value[0], param->length);
		
}

void user_custs1_long_val_cfg_ind_handler(ke_msg_id_t const msgid,
                                           struct custs1_val_write_ind const *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id)
{
}

void user_custs1_long_val_wr_ind_handler(ke_msg_id_t const msgid,
                                          struct custs1_val_write_ind const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
	#if 0
	struct custs1_val_ntf_req* long_val_ntf_req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
																										TASK_CUSTS1,
																										TASK_APP,
																										custs1_val_ntf_req,
																										DEF_CUST1_LONG_VALUE_CHAR_LEN);
	
	
	long_val_ntf_req->conhdl = app_env->conhdl;
	long_val_ntf_req->handle = CUST1_IDX_LONG_VALUE_VAL;
	long_val_ntf_req->length = param->length;
	memcpy(long_val_ntf_req->value, &param->value[0], param->length);

	ke_msg_send(long_val_ntf_req);
	#endif
}

void user_custs1_long_val_ntf_cfm_handler(ke_msg_id_t const msgid,
                                           struct custs1_val_write_ind const *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id)
{
}

void user_custs1_adc_val_1_cfg_ind_handler(ke_msg_id_t const msgid,
                                            struct custs1_val_write_ind const *param,
                                            ke_task_id_t const dest_id,
                                            ke_task_id_t const src_id)
{
}

void user_custs1_adc_val_1_ntf_cfm_handler(ke_msg_id_t const msgid,
                                            struct custs1_val_write_ind const *param,
                                            ke_task_id_t const dest_id,
                                            ke_task_id_t const src_id)
{
}

void user_custs1_button_cfg_ind_handler(ke_msg_id_t const msgid,
                                         struct custs1_val_write_ind const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id)
{
}

void user_custs1_button_ntf_cfm_handler(ke_msg_id_t const msgid,
                                         struct custs1_val_write_ind const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id)
{
		#if 0
    static uint8_t button_state = 0;
	
		struct custs1_val_ntf_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                                      TASK_CUSTS1,
                                                      TASK_APP,
                                                      custs1_val_ntf_req,
                                                      DEF_CUST1_ADC_VAL_1_CHAR_LEN);
		
		button_state = GPIO_GetPinStatus( GPIO_BUTTON_PORT, GPIO_BUTTON_PIN );

    req->conhdl = app_env->conhdl;
    req->handle = CUST1_IDX_BUTTON_STATE_VAL;
    req->length = DEF_CUST1_BUTTON_STATE_CHAR_LEN;
    memcpy(req->value, &button_state, DEF_CUST1_BUTTON_STATE_CHAR_LEN);

    ke_msg_send(req);
		#endif
		
}

void user_custs1_indicateable_cfg_ind_handler(ke_msg_id_t const msgid,
                                               struct custs1_val_write_ind const *param,
                                               ke_task_id_t const dest_id,
                                               ke_task_id_t const src_id)
{
}

void user_custs1_indicateable_ind_cfm_handler(ke_msg_id_t const msgid,
                                               struct custs1_val_write_ind const *param,
                                               ke_task_id_t const dest_id,
                                               ke_task_id_t const src_id)
{
}

u8 usage_data_read;
void app_data_timer_cb_handler(void)
{
	if (_usage_data_.usage_data_count_read != 0xffff)
	{
		if (_usage_data_.usage_data_count_read > 0)
		{
			// Initialize DA14583 internal flash
			da14583_spi_flash_init();
			
			_usage_data_.usage_data_read_address = USER_FLASH_ADDRESS_START2 + _usage_data_.usage_data_read_address_cnt * 7;
			
			usage_data_read = spi_flash_read_data (_usage_data_.usage_data_to_read, _usage_data_.usage_data_read_address, 7);
			
			_usage_data_.usage_data_read_address_cnt++;
			
			_usage_data_.usage_data_count_read--;
			if (_usage_data_.usage_data_count_read == 0)//finish uploading
			{
				_usage_data_.usage_data_read_address_cnt = 0;
				_usage_data_.usage_data_count_write = 0;
				spi_flash_block_erase(USER_FLASH_ADDRESS_START2, SECTOR_ERASE);
				spi_flash_block_erase(USER_FLASH_ADDRESS_START, SECTOR_ERASE);
			}
			// Release DA14583 internal flash
			da14583_spi_flash_release();
			
			struct custs1_val_ntf_req* long_val_ntf_req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
																									TASK_CUSTS1,
																									TASK_APP,
																									custs1_val_ntf_req,
																									DEF_CUST1_LONG_VALUE_CHAR_LEN);
			
			long_val_ntf_req->conhdl = app_env->conhdl;
			long_val_ntf_req->handle = CUST1_IDX_LONG_VALUE_VAL;
			long_val_ntf_req->length = 7;
			memcpy(long_val_ntf_req->value, _usage_data_.usage_data_to_read, 7);
			
			ke_msg_send(long_val_ntf_req);
			if (ke_state_get(TASK_APP) == APP_CONNECTED)
			{
					// Set it once again until Stop command is received in Control Characteristic
					datatimer_used = app_easy_timer(50, app_data_timer_cb_handler);

					uart2_print_string("Control_on\r\n");
			}
		}
	}
}

void Battery_level_upload()
{
		if(batt_sample != battery_level_value)
		{
			/* Battery voltage value notify */
			struct custs1_val_ntf_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
																												TASK_CUSTS1,
																												TASK_APP,
																												custs1_val_ntf_req,
																												DEF_CUST1_ADC_VAL_1_CHAR_LEN);


	//    static uint16_t sample;
			batt_sample = battery_level_value;

			req->conhdl = app_env->conhdl;
			req->handle = CUST1_IDX_ADC_VAL_1_VAL;
			req->length = DEF_CUST1_ADC_VAL_1_CHAR_LEN;
			memcpy(req->value, &batt_sample, DEF_CUST1_ADC_VAL_1_CHAR_LEN);

			ke_msg_send(req);
			
		}	
    if (ke_state_get(TASK_APP) == APP_CONNECTED)
    {
			batterytimer_used = app_easy_timer(100, Battery_level_upload);
		}
}

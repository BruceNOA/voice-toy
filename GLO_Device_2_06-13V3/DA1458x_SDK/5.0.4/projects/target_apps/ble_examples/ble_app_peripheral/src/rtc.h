#ifndef RTC_H
#define RTC_H

#include "gpio.h"

#define GPIO_RTC_SCL_PORT     GPIO_PORT_0
#define GPIO_RTC_SCL_PIN      GPIO_PIN_6

#define GPIO_RTC_SDA_PORT     GPIO_PORT_0
#define GPIO_RTC_SDA_PIN      GPIO_PIN_7


/* Registers */
#define REG_CTRL_1  		0x00
#define REG_CTRL_2  		0x01
#define REG_WATCH_SEC   0x02
#define REG_WATCH_MIN   0x03
#define REG_WATCH_HOUR  0x04
#define REG_WATCH_DATE  0x05
#define REG_WATCH_DAY   0x06
#define REG_WATCH_MON   0x07
#define REG_WATCH_YEAR  0x08
#define I2C_SLAVE_ADDRESS  (0xa2 >> 1)

#define RTC_SET_TIME_LEN 7
#define RTC_GET_TIME_LEN 7

#define RTC_CONFIG_FLASH_ADDRESS 0x12000
#define RTC_CONFIG_DATA 				 0X01

extern u8 rtc_set_time[RTC_SET_TIME_LEN];
extern u8 rtc_get_time[RTC_GET_TIME_LEN];

void RTC_GPIO_reservations(void);
void set_rtc_pad_functions(void) ;
void i2c_rtc_init(uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size);
void i2c_rtc_write_data(uint8_t *wr_data_ptr, uint32_t address, uint32_t size, uint32_t *bytes_written);
void i2c_rtc_read_data(uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_read);

#if 1
void RTC_SetTime(u8 year, u8 month, u8 day, u8 w_day, u8 hour, u8 min, u8 sec);
#else
void RTC_SetTime(void);
#endif



void RTC_GetTime(void);
void RTC_Config(void);
#endif








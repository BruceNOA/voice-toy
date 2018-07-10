#include "rtc.h"

u8 rtc_set_time[RTC_SET_TIME_LEN];
u8 rtc_get_time[RTC_GET_TIME_LEN];

void RTC_GPIO_reservations(void)
{
	RESERVE_GPIO(RTC_SCL, GPIO_RTC_SCL_PORT, GPIO_RTC_SCL_PIN, PID_I2C_SCL);
	RESERVE_GPIO(RTC_SDA, GPIO_RTC_SDA_PORT, GPIO_RTC_SDA_PIN, PID_I2C_SDA);
}

void set_rtc_pad_functions(void)       
{
	GPIO_ConfigurePin(GPIO_RTC_SCL_PORT, GPIO_RTC_SCL_PIN, INPUT, PID_I2C_SCL, false);
	GPIO_ConfigurePin(GPIO_RTC_SDA_PORT, GPIO_RTC_SDA_PIN, INPUT, PID_I2C_SDA, false);
}

void i2c_rtc_init(uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size)
{
	i2c_eeprom_init(dev_address, speed, address_mode, address_size);
}

i2c_error_code error_code_write;
void i2c_rtc_write_data(uint8_t *wr_data_ptr, uint32_t address, uint32_t size, uint32_t *bytes_written)
{
	i2c_rtc_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_1BYTE_ADDR);
	error_code_write = i2c_eeprom_write_data(wr_data_ptr, address, size, bytes_written);
	i2c_eeprom_release();
}

i2c_error_code error_code_read;
void i2c_rtc_read_data(uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_read)
{
	i2c_rtc_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_1BYTE_ADDR);
	error_code_read = i2c_eeprom_read_data(data, address, size, bytes_read);
	i2c_eeprom_release();
}

int BCDToDEC(u8 bcd) //BCD TO DEC
{
  return ((0xff & (bcd>>4))*10 +(0xf & bcd));
}

int DECToBCD(u8 dec) //DEC TO BCD
{
	return (((dec/10)<<4)+((dec%10)&0x0f));
}

u32 bytes_written_value;
#if 1
void RTC_SetTime(u8 year, u8 month, u8 day, u8 w_day, u8 hour, u8 min, u8 sec)
{
	u8 i; 
	#if 0
	rtc_set_time[0] = 30;//sec
	rtc_set_time[1] = 31;//min
	rtc_set_time[2] = 14;//hour
	rtc_set_time[3] = 25;//day
	rtc_set_time[4] = 3;//week day
	rtc_set_time[5] = 4;//month
	rtc_set_time[6] = 18;//year
	#else
	rtc_set_time[0] = sec;//sec
	rtc_set_time[1] = min;//min
	rtc_set_time[2] = hour;//hour
	rtc_set_time[3] = day;//day
	rtc_set_time[4] = w_day;//week day
	rtc_set_time[5] = month;//month
	rtc_set_time[6] = year;//year
	#endif
	
	
	for (i = 0; i < RTC_SET_TIME_LEN; i++)
	{
		rtc_set_time[i] = DECToBCD(rtc_set_time[i]);
	}
	
	i2c_rtc_write_data(rtc_set_time,REG_WATCH_SEC,RTC_SET_TIME_LEN,&bytes_written_value);
}
#else
void RTC_SetTime(void)
{
	u8 i; 
	rtc_set_time[0] = 30;//sec
	rtc_set_time[1] = 31;//min
	rtc_set_time[2] = 14;//hour
	rtc_set_time[3] = 25;//day
	rtc_set_time[4] = 3;//week day
	rtc_set_time[5] = 4;//month
	rtc_set_time[6] = 18;//year
	
	for (i = 0; i < RTC_SET_TIME_LEN; i++)
	{
		rtc_set_time[i] = DECToBCD(rtc_set_time[i]);
	}
	
	i2c_rtc_write_data(rtc_set_time,REG_WATCH_SEC,RTC_SET_TIME_LEN,&bytes_written_value);
}
#endif


u32 bytes_read_value;
void RTC_GetTime(void)
{
	u8 i;
	i2c_rtc_read_data(rtc_get_time, REG_WATCH_SEC, RTC_GET_TIME_LEN, &bytes_read_value);
	
	#if 0
	for (i = 0; i < RTC_GET_TIME_LEN; i++)
	{
		rtc_get_time[i] = BCDToDEC(rtc_get_time[i]);
	}
	#endif
}

uint32_t rtc_config_bytes_read = 0;
uint8_t rtc_config_data_read;
uint32_t rtc_config_bytes_written = 0;
uint8_t rtc_config_to_write = RTC_CONFIG_DATA;
void RTC_Config(void)
{
	// Initialize DA14583 internal flash
  da14583_spi_flash_init();
#if 1	
	rtc_config_bytes_read = spi_flash_read_data (&rtc_config_data_read, RTC_CONFIG_FLASH_ADDRESS, 1);
	if (rtc_config_data_read != RTC_CONFIG_DATA)
	{
		rtc_config_bytes_written = spi_flash_write_data (&rtc_config_to_write, RTC_CONFIG_FLASH_ADDRESS, 1);
		RTC_SetTime(18, 4, 28, 6, 9, 53, 30);
	}
#else	
	rtc_config_bytes_written = spi_flash_write_data (&rtc_config_to_write, RTC_CONFIG_FLASH_ADDRESS, 1);
	RTC_SetTime(18, 4, 28, 6, 9, 53, 30);
#endif
	
	// Release DA14583 internal flash
  da14583_spi_flash_release();
}



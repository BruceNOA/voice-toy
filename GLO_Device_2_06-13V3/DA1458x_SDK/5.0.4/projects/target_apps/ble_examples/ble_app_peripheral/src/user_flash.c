#include "user_flash.h"
#include "rtc.h"
#include "user_periph_setup.h"      


uint32_t flash_bytes_read = 0;
uint8_t data_read[7];
uint32_t flash_bytes_written = 0;
uint8_t data_to_write[7] = {1,2,3,4,5,6,7};

/* Just for testing using(writing and reading) the spi flash of DA14583 */
void spi_flash_test(void)
{
	// Initialize DA14583 internal flash
  da14583_spi_flash_init();
	
	spi_flash_block_erase(USER_FLASH_ADDRESS_START, SECTOR_ERASE);
	spi_flash_block_erase(USER_FLASH_ADDRESS_START2, SECTOR_ERASE);
	
	flash_bytes_read = spi_flash_read_data (data_read, USER_FLASH_ADDRESS_START, 7);
	//if (data_read != USER_FLASH_TEST_DATA)
	{
		flash_bytes_written = spi_flash_write_data (data_to_write, (USER_FLASH_ADDRESS_START2 - 3), 7);
	}
	
	flash_bytes_read = spi_flash_read_data (data_read, (USER_FLASH_ADDRESS_START2 - 3), 7);
	
	
	spi_flash_block_erase(USER_FLASH_ADDRESS_START, SECTOR_ERASE);
	spi_flash_block_erase(USER_FLASH_ADDRESS_START2, SECTOR_ERASE);
	
	flash_bytes_read = spi_flash_read_data (data_read, (USER_FLASH_ADDRESS_START2 - 3), 7);
	
	// Release DA14583 internal flash
  da14583_spi_flash_release();
}

u8 usage_data_count_write_bytes[2];
u8 usage_data_count_bytes_written;
u8 usage_data_written;

/* This function is used for saving usage data record in the spi flash of DA14583,
including the RTC data and LED state, also the number of usage data record*/
void save_usage_data(char led_state)
{
	RTC_GetTime();
	
	/* pack rtc data */
	_usage_data_.usage_data_write[0] = rtc_get_time[6];
	_usage_data_.usage_data_write[1] = rtc_get_time[5];
	_usage_data_.usage_data_write[2] = rtc_get_time[3];
	_usage_data_.usage_data_write[3] = rtc_get_time[2];
	_usage_data_.usage_data_write[4] = rtc_get_time[1];
	_usage_data_.usage_data_write[5] = rtc_get_time[0];
	
	/* pack led state */
	_usage_data_.usage_data_write[6] = led_state;
	
	// Initialize DA14583 internal flash
  da14583_spi_flash_init();
	
	_usage_data_.usage_data_write_address = USER_FLASH_ADDRESS_START2 + 7 * _usage_data_.usage_data_count_write;
	usage_data_written = spi_flash_write_data (_usage_data_.usage_data_write, _usage_data_.usage_data_write_address, 7);
	
	spi_flash_block_erase(USER_FLASH_ADDRESS_START, SECTOR_ERASE);
	
	_usage_data_.usage_data_count_write++;
	usage_data_count_write_bytes[0] = (u8)(_usage_data_.usage_data_count_write >> 8);
	usage_data_count_write_bytes[1] = (u8)_usage_data_.usage_data_count_write;
	
	usage_data_count_bytes_written = spi_flash_write_data (usage_data_count_write_bytes, USER_FLASH_ADDRESS_START, 2);
	
	// Release DA14583 internal flash
  da14583_spi_flash_release();
}


u8 usage_data_count_read_bytes[2];
u8 usage_data_count_bytes_read;
/* This function is used for reading usage data record number from the spi flash of DA14583,
also reset the related parameters for reading the usage data*/
void get_usage_data_length(void)
{
	// Initialize DA14583 internal flash
	da14583_spi_flash_init();

	usage_data_count_bytes_read = spi_flash_read_data (usage_data_count_read_bytes, USER_FLASH_ADDRESS_START, 2);
	_usage_data_.usage_data_count_read = (((u16) (usage_data_count_read_bytes[0])) << 8) + (u16) (usage_data_count_read_bytes[1]);

	_usage_data_.usage_data_read_address = USER_FLASH_ADDRESS_START2;
	_usage_data_.usage_data_read_address_cnt = 0;
	
	// Release DA14583 internal flash
	da14583_spi_flash_release();
}



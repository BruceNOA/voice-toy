#ifndef USER_FLASH_H
#define USER_FLASH_H



#define USER_FLASH_ADDRESS_START 	0x1D000 //length
#define USER_FLASH_ADDRESS_START2 0x1E000 //data

#define USER_FLASH_TEST_DATA 0X01

extern void da14583_spi_flash_init(void);
extern void da14583_spi_flash_release(void);


void spi_flash_test(void);

void save_usage_data(char led_state);

void get_usage_data_length(void);



#endif


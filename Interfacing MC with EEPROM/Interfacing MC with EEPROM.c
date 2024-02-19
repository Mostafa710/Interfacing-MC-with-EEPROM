#define F_CPU 8000000UL
#include <util/delay.h>
#include "LCD.h"
#include "I2C.h"

int main(void)
{
	LCD_init();
	I2C_Master_init(50000);
	char name[]="Mostafa Mamdouh";
	unsigned char i=0,j=0,x;
	/* Storing my name into the external EEPROM 
	and starting from address 0b000 0000 0000 */
	I2C_start();
	I2C_write_address_and_write(0b10100000);	// Sending Device Address + Write
	I2C_Master_write(0b00000000);				// Sending Word Address
	while(name[i] != '\0')
	{
		I2C_Master_write(name[i]);
		i++;
	}
	I2C_stop();
	_delay_ms(1000);
	/* Getting my name from the external EEPROM
	and display it on the LCD */
	while(j < i)
	{
		I2C_start();
		I2C_write_address_and_write(0b10100000);	// Sending Device Address + Write
		I2C_Master_write(0b00000000+j);				// Sending Word Address
		I2C_repeated_start();
		I2C_write_address_and_read(0b10100001);	// Sending Device Address + Read
		x = I2C_Master_read_with_NACK();
		I2C_stop();
		LCD_send_char(x);
		j++;
	}
}
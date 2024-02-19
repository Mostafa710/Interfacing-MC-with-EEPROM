#define F_CPU 8000000UL
#include <avr/io.h>
#include "std_macros.h"
#include "I2C.h"
#define prescaler 1

void I2C_Master_init(unsigned long SCL_Clock)
{
	TWBR = (((F_CPU/SCL_Clock)-16) / (2*prescaler));
	switch(prescaler)
	{
		case 1:
		TWSR = 0;
		break;
		case 4:
		TWSR = 1;
		break;
		case 16:
		TWSR = 2;
		break;
		case 64:
		TWSR = 3;
		break;
	}
}
void I2C_start(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
	while(READ_BIT(TWCR,TWINT)==0);    // Wait to finish
	if((TWSR & 0xF8) != 0x08)		   // Check if Start Condition has been received
	{
		I2C_stop();
	}
}
void I2C_repeated_start(void)
{
	TWCR|= (1<<TWINT)|(1<<TWSTA);
	while(READ_BIT(TWCR,TWINT)==0);    // Wait to finish
	if((TWSR & 0xF8) != 0x10)          // Check if Repeated Start Condition has been transmitted
	{
		I2C_stop();
	}
}
void I2C_stop(void)
{
	TWCR|= (1<<TWINT)|(1<<TWSTO);
	/* We may not use this while loop as the communication ended
	while(READ_BIT(TWCR,TWINT)==0); */
}
void I2C_set_address(unsigned char address)
{
	TWAR = address;
}
void I2C_write_address_and_write(unsigned char address)
{
	TWDR = address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(READ_BIT(TWCR,TWINT)==0);    // Wait to finish
	if((TWSR & 0xF8) != 0x18)		   // Check if SLA+write has been transmitted and ACK has been received
	{
		I2C_stop();
	}
}
void I2C_write_address_and_read(unsigned char address)
{
	TWDR = address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(READ_BIT(TWCR,TWINT)==0);    // Wait to finish
	if((TWSR & 0xF8) != 0x40)		   // Check if SLA+read has been transmitted and ACK has been received
	{
		I2C_stop();
	}
}
void I2C_Master_write(unsigned char data)
{
	TWDR = data;
	SET_BIT(TWCR,TWINT);
	while(READ_BIT(TWCR,TWINT)==0);    // Wait to finish
	if((TWSR & 0xF8) != 0x28)		   // Check if data has been transmitted and ACK has been received
	{
		I2C_stop();
	}
}
unsigned char I2C_Slave_read(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(READ_BIT(TWCR,TWINT)==0);
	while((TWSR & 0xF8) != 0x60);    // Double check if own SLA+Write has been received and ACK has been returned
	SET_BIT(TWCR,TWINT);
	while(READ_BIT(TWCR,TWINT)==0); 
	while((TWSR & 0xF8) != 0x80);    // Double check if data has been received and ACK has been returned
	return TWDR;
}
unsigned char I2C_Master_read_with_NACK(void)
{
	SET_BIT(TWCR,TWINT);
	while(READ_BIT(TWCR,TWINT)==0);    // Wait to finish
	if((TWSR & 0xF8) != 0x58)		   // Check if data has been received and none-ACK has been returned
	{
		I2C_stop();
	}
	return TWDR;
}
#ifndef I2C_H_
#define I2C_H_

void I2C_Master_init(unsigned long SCL_Clock);
void I2C_start(void);
void I2C_repeated_start(void);
void I2C_stop(void);
void I2C_set_address(unsigned char address);
void I2C_write_address_and_write(unsigned char address);
void I2C_write_address_and_read(unsigned char address);
void I2C_Master_write(unsigned char data);
unsigned char I2C_Slave_read(void);
unsigned char I2C_Master_read_with_NACK(void);

#endif
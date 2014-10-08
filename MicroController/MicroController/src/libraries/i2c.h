//! @file
//! This library implements bit-banged low-level I2C routines
//! using the standard SCL and SDA pins

#include "LPC11xx.h"

//! initialize the I2C bus
void i2c_init( void );

//! output an I2C start 
void i2c_start( void );

//! output an I2C stop
void i2c_stop();

//! get and return the ack condition
unsigned char i2c_ack_get( void );

//! output an I2C ack
void i2c_put_ack( void );

//! output an I2C nack
void i2c_put_nack( void );

//! output a byte on the i2c bus
void i2c_put_byte( unsigned char x );

//! get a byte from the i2c bus and return it
unsigned char i2c_get_byte( void );

//! set a byte on the specified register of the specified peripheral
void i2c_set_byte( unsigned char device, unsigned char reg, unsigned char byte);

//! prepare the peripheral for reading from the specified register
void i2c_read_select_register(unsigned char device, unsigned char reg);

//! return the value stored in the specified register on the peripheral
unsigned char i2c_read(unsigned char device);

//! sets a bit, but leaves the other bits unchanged
void i2c_set_bit( unsigned char device, unsigned char reg, unsigned char position, unsigned char bit);
/* ========================================================================== */
/*                                                                            */
/*   DS1990.h                                                                 */
/*   (c) 2014 Author Daniël van den Berg                                      */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

//!@file This library is used to change and read the state of the i/o pins

#ifndef DS1990_H_
#define DS1990_H_

void ds_pin_set( unsigned char x );
unsigned char ds_pin_get();
void ds_pin_pulse( int t );
unsigned char ds_present();
void ds_send_bit( unsigned char b );
void ds_send_byte( unsigned char b );
unsigned char ds_receive_bit( void );
unsigned char ds_receive_byte();
void ds1990_id_print();

#endif
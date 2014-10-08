/* ========================================================================== */
/*                                                                            */
/*   pins.h                                                                   */
/*   (c) 2014 Author Daniël van den Berg                                      */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

//!@file
//! This library is used to change and read the state of the i/o pins

#ifndef PINS_H_
#define PINS_H_

//!configure pin as input pin.
//
//!Used to configure the specified pin on the specified port to be used as a readable input pin.
//!@param port the port the specified pin is placed on. Either 1 or 0.
//!@param pin the number of the pin.
void pin_configure_as_input( int port, int pin );

//!configure pin as output pin.
//
//!Used to configure the specified pin on the specified port to be used as an output pin.
//!@param port the port the specified pin is placed on. Either 1 or 0.
//!@param pin the number of the pin.
void pin_configure_as_output( int port, int pin );

//!set the state of a pin.
//
//!Used to set the state of a pin to either be on (1) or off(0).
//!@param port the port the specified pin is placed on. Either 1 or 0.
//!@param pin the number of the pin.
//!@param x the new state of the pin. Either 0 (false) or not 0 (true)
void pin_set( int port, int pin, unsigned char x );

//!get the state of a pin.
//
//!Used to read if the specified pin is on or off.  
//!@param port the port the specified pin is placed on. Either 1 or 0.
//!@param pin the number of the pin.
unsigned char pin_get( int port, int pin );

//!send a pulse over a pin.
//
//!Used to turn a pin on for the specified amount of time, and have it turn off again.  
//!@param port the port the specified pin is placed on. Either 1 or 0.
//!@param pin the number of the pin.
//!@param time the time the port is supposed to stay on, specified in nanoseconds.
void pin_pulse(int port, int pin, int time);

#endif
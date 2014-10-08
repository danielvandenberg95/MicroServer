/* ========================================================================== */
/*                                                                            */
/*   Filename.c                                                               */
/*   (c) %YEAR% Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "LPC11xx.h"
#include "timer.h"
#include "pins.h"

int servo_port;
int servo_pin;

void servo_low( void ){
   pin_set(servo_port,servo_pin,0);
}

void servo_high( void ){
   pin_set(servo_port,servo_pin,1);
}

void servo_init( int port2, int pin2 ){
  servo_port = port2;
  servo_pin = pin2;
  pin_configure_as_output(servo_port,servo_pin); 
  servo_low();
}

void servo_pulse( unsigned int length ){
   servo_high();
   delay(length);   
   servo_low();
}

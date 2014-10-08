/* ========================================================================== */
/*                                                                            */
/*   pins.c                                                                   */
/*   (c) 2014 Author Daniël van den Berg                                      */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "pins.h"
#include "timer.h"
#include "LPC11xx.h"  

void pin_configure_as_input( int port, int pin ){ 
  if (port){
    LPC_GPIO1->DIR &= ~ 1<<pin;
  }else{
    LPC_GPIO0->DIR &= ~ 1<<pin;
  }  
}

void pin_configure_as_output( int port, int pin ){
  if (port){
    LPC_GPIO1->DIR |= 1<<pin;
  }else{
    LPC_GPIO0->DIR |= 1<<pin;
  }  
}

void pin_set( int port, int pin, unsigned char x ){
  if (port){
    if( x ){
       LPC_GPIO1->DATA = LPC_GPIO1->DATA | ( 1 << pin );
    } else {
       LPC_GPIO1->DATA = LPC_GPIO1->DATA & ~ ( 1 << pin );
    }
   }else{
    if( x ){
       LPC_GPIO0->DATA = LPC_GPIO0->DATA | ( 1 << pin );
    } else {
       LPC_GPIO0->DATA = LPC_GPIO0->DATA & ~ ( 1 << pin );
    }
   }
}
unsigned char pin_get( int port, int pin ){
  if (port){
    return ( LPC_GPIO1->DATA & ( 1 << pin ) ) != 0;
  }else{
    return ( LPC_GPIO0->DATA & ( 1 << pin ) ) != 0;  
  }
}

void pin_pulse(int port, int pin, int time){
      pin_set(port,pin,1);
      delay(time);
      pin_set(port,pin,0);
} 
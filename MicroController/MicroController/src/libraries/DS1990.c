/* ========================================================================== */
/*                                                                            */
/*   DS1990.c                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

// read a DS1990, use pin GP0_4 ('SCL')

#include "LPC11xx.h"
#include "timer.h" 
#include "pins.h"
#include "uart.h"

void ds_pin_set( unsigned char x ){
   if( x ){
      pin_configure_as_input(0,4);        // omdat het input is trekt hij de stroom automatisch omhoog
   } else {
      pin_configure_as_output(0,4);
      pin_set(0,4,0);
   }      
}

unsigned char ds_pin_get(){
    return pin_get(0,4)==0;
}

void ds_pin_pulse(int t ){
   ds_pin_set( 0 );
   delay( t );
   ds_pin_set( 1 );
}  

unsigned char ds_present(){

   // give the device time to get back to the 'rest' state
   ds_pin_set( 1 );
   delay( 2000 );
   
   // test for stuck-at-0
   if( ! ds_pin_get() ){ return 0; }
   
   // request a presence pulse
   ds_pin_pulse( 1000 );   
   
   // test for the presence pulse 
   delay( 30 );
   if( ds_pin_get() ){ return 0; }
   
   
   // test whether the presence pulse ends
   delay( 1000 );
   if( ! ds_pin_get() ){ return 0; }
   
   // all tests succeeded
   return 1;   
}



void ds_send_bit( unsigned char b ){
    ds_pin_set(b);
   if(b==1){
   ds_pin_pulse(8);
   }
   else {
   ds_pin_pulse(90);
   }
   delay(10);
}

void ds_send_byte( unsigned char b ){
  int i;
  
  for(i =0; i<8; i++){
  ds_send_bit(b & 0x01); // met 0x01 vraag je via hexadecimaal de eerste bit op van de byte
  b = b >> 1; // zo schuift hij door naar de volgende bit door b op te tellen
  }
}

unsigned char ds_receive_bit( void ){
   unsigned char x;
   ds_pin_pulse(5);
   delay(5);
   x = ds_pin_get();
   delay(100);
   return x;
}

unsigned char ds_receive_byte(){
   int i;
   unsigned char d = 0;
   for( i = 0; i < 8; i++ ){
      d = d >> 1;
      if( ds_receive_bit() ){
         d = d | 0x80;   
      } else {
         d = d & ~ 0x80;
      }
   }
   return d;
}

void ds1990_id_print(){
   int i,d;
   ds_send_byte(0x33);
   for( i = 0; i < 8; i++ ){
      d = ds_receive_byte();
      uart_put_int_hexadecimal( d, 2 );
      uart_put_string( " " );
   }
   uart_put_string( "\n" );
} 
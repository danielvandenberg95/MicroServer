/* ========================================================================== */
/*                                                                            */
/*   uart.h                                                                   */
/*   (c) 2014 Daniël van den Berg                                             */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */


#include "LPC11xx.h"
#include "uart.h"
#include "timer.h"
#include "pins.h"

#define BIT_TIME ((1000*1000)/BMPTK_BAUDRATE);

void txd_low( void ){
  pin_set(1,7,0);  
}

void txd_high( void ){
  pin_set(1,7,1);  
}

void uart_init( void ){
  pin_configure_as_output(1,7);
  timer_init();
  txd_high();
}

void uart_put_string(char *s){
  while (*s != '\0'){
    uart_put_char(*s);
    s++;
  }
}

void uart_put_char(char c){
  unsigned int t = now();
  txd_low();
  t += BIT_TIME;
  await(t);
  int i;
  for (i=0;i<8;i++){
    if ((c&0x01) == 0){
      txd_low();
    }else{
      txd_high();
    }
    c >>= 1;
    
    t += BIT_TIME;
    await(t);
  }
  
  txd_high();
  t+=2*BIT_TIME;
  await(t);
}          

void uart_put_int_negative( int x ){
   int xx = x / 10;
   if( xx != 0 ){
      uart_put_int_negative( xx );
   }
   uart_put_char( '0' - ( x % 10 ));
}

void uart_put_int_decimal( int x ){
   if( x < 0 ){
      uart_put_char( '-' );
   } else {   
      x = - x;      
   } 
   uart_put_int_negative( x );
}   

void uart_put_int_hexadecimal_digit( int x ){
   x = x & 0x0F;
   if( x < 10 ){
      uart_put_char( x + '0' );
   } else {
      uart_put_char( ( x + 'A' ) - 10 );
   }      
}

void uart_put_int_hexadecimal( unsigned int x, int n ){
   while( n > 0 ){
      uart_put_int_hexadecimal_digit( x >> (( n - 1 ) * 4 ) );
      n--;
   }
}

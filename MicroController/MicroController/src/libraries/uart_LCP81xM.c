/* ========================================================================== */
/*                                                                            */
/*   uart.h                                                                   */
/*   (c) 2014 Daniël van den Berg                                             */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */


#include "lpc810lib.h"
#include "uart_LCP81xM.h"

#define BIT_TIME ((1000*1000)/38400);

void txd_low( void ){
  pin_set(7,0);  
}

void txd_high( void ){
  pin_set(7,1);  
}

void uart_init( void ){
  pin_configure_as_output(7);
  txd_high();
}

void uart_put_string(char *s){
  while (*s != '\0'){
    uart_put_char(*s);
    s++;
  }
}

void uart_put_char(char c){
  txd_low();
  wait_us(((1000*1000)/38400));
  int i;
  for (i=0;i<8;i++){
    if ((c&0x01) == 0){
      txd_low();
    }else{
      txd_high();
    }
    c >>= 1;
    
    wait_us(((1000*1000)/38400));
  }
  
  txd_high();
  wait_us(((1000*1000)/38400)*2);
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

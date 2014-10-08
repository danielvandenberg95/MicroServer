/* ========================================================================== */
/*                                                                            */
/*   uart.h                                                                  */
/*   (c) 2012 Author Wouter van Ooijen                                        */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */


//!@file
//! This library implements a bit-mbanged output-only UART at
//! the baudrate specified by BMPTK_BAUDRATE.

//! initialize the UART
//
//! Call this function to initialize the UART pins.
void uart_init( void );

//! write a single character
//
//! This function writes a single character using the standard UART
//! output pin of the chip (GP1_7) and the baudrate BMPTK_BAUDRATE, 
//! which is passed as a command line argument duuring compilation.
void uart_put_char( char c );

//! write a character string
//
//! This function writes the characters of a standard 0-terminated string
//! using uart_put_char().
void uart_put_string( char *s );

//! set the txd pin to low.
//
//! This function sets the txd pin low.
void txd_low( void ); 

//! set the txd pin to high.
//
//! This function sets the txd pin high.
void txd_high( void );

//! write a negative int to uart.
//
//! Write a negative integer over the UART pin.
void uart_put_int_negative( int x );  

//! write a decimal int to uart.
//
//! Write a decimal integer over the UART pin.
void uart_put_int_decimal( int x );

//! write a hexadecimal digit to uart.
//
//! Write a hexadecimal digit over the UART pin.
void uart_put_int_hexadecimal_digit( int x );

//! write a hexadecimal int to uart.
//
//! Write a hexadecimal integer over the UART pin.
void uart_put_int_hexadecimal( unsigned int x, int n );
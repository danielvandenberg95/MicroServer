//! @file
//! UART library


#ifndef _UART_H
#define _UART_H


//! Initialiseer hardware UART
//
//! Initialiseer de UART hardware.
void uart_init( int baud_rate );


//! Schrijf char naar UART
//
//! Schrijf een byte in het UART Transmit register. UART hardware verzend de byte.
void uart_put_char( const char c );


//! Schrijf integer naar UART
//
//! Verdeelt getal in losse decimalen en verzend deze met uart_put_char().
void uart_put_int( int n );


//! Schrijf string naar UART
//
//! Schrijft alle karakters uit een gegeven string naar uart_put_char().
void uart_put_string( char *s );


//! Check inkomende data
//
//! geeft 1 terug als er een char ontvangen is en klaar om te lezen. Geeft 0 terug als het ontvang register leeg is.
unsigned char uart_receiver_data_ready( void );


//! Lees ontvangen char
//
//! geeft eerst volgende char uit het ontvang buffer terug. Meerdere chars kunnen in het buffer staan. Controleer eerst of er een char beschikbaar is met uart_receiver_data_ready() 
unsigned char uart_get_char( void );


#endif
/*

  Hardwarematige UART library voor verzenden en ontvangen van data. Gebaseerd op sw-uart.
  
  Library geschreven door Bart Muelders (1621330)                 | v1.0
  Bugfix door Kjeld Perquin (1635973) en Yorrick Lans (1639569)   | v1.1
  
  Library mag vrij gebruikt worden mits bovenstaande vermelding aanwezig is.
*/


#include "LPC11xx.h"
#include "hwUart.h"


#define UART_MCU_CLOCK 12000000                                                 // klok frequentie van chip


void uart_init( int baud_rate ){  
  LPC_SYSCON->SYSAHBCLKCTRL |= ( 1 << 16 );                                     // zet IO config aan
   
  LPC_SYSCON->SYSAHBCLKCTRL |= ( 1 << 12 );                                     // zet clock voor UART aan
  LPC_SYSCON->UARTCLKDIV = 0x1;                                                 // deel de UART clock door 1

  LPC_IOCON->PIO1_6 &= ~0x07;                                                   // zet de eerste 3 bits (pinfunctie) van pin 1_6 op 0 (reset) 
  LPC_IOCON->PIO1_6 |= 0x01;                                                    // zet bit 1 op 1 zodat pin 1_6 RXD wordt
  LPC_IOCON->PIO1_7 &= ~0x07;                                                   // zet de eerste 3 bits (pinfunctie) van pin 1_7 op 0 (reset)
  LPC_IOCON->PIO1_7 |= 0x01;                                                    // zet bit 1 op 1 zodat pin 1_7 TXD wordt

  LPC_UART->LCR = 0x83;                                                         // stel UART format in: 8N1 (8 bits, geen pariteit, 1 stop bit). Zet toegang tot Divisor Latches aan (bit 7)

  /* bereken fractional divider */
  int fractional_division = UART_MCU_CLOCK                                      // klok snelheid
              / LPC_SYSCON->SYSAHBCLKDIV                                        // AHB klok divider
              / LPC_SYSCON->UARTCLKDIV                                          // UART klok divider
              / 16                                                              // divisor latch is 16x baud
              / baud_rate;                                                      // gegeven baudrate
                  
  LPC_UART->DLM = fractional_division / 256;                                    // laagste 8 bits van fractional divider
  LPC_UART->DLL = fractional_division % 256;                                    // hoogste 8 bits van fractional divider

  LPC_UART->FDR = 0x00 | ( 1 << 4 ) | 0;                                        // fractional divider register pre-scaler

  LPC_UART->FCR = 0x07;                                                         // zet FIFO (fist-in, first-out) aan en reset

  LPC_UART->LCR = 0x03;                                                         // laat UART format 8N1, maar zet toegang tot Divisor Latches uit
  
  LPC_SYSCON->SYSAHBCLKCTRL &= ~( 1 << 16 );                                    // zet IO config weer uit
}


void uart_put_char( const char ch ){
  while( !( LPC_UART->LSR & 0x20 ) );                                           // wacht tot eventuele vorige byte verzonden is
  LPC_UART->THR = ch;                                                           // zet char in verzend register
}

unsigned char uart_receiver_data_ready( void ){
  return ( LPC_UART->LSR & (0x01) ) == 1;                                       //geef aan of er 1 of meerdere chars in ontvang buffer staan
}


unsigned char uart_get_char( void ){
  return LPC_UART->RBR;                                                         //geef eerst volgende char dat in buffer staat
}

static void uart_put_int_decimals( int number ){
   int number2 = number / 10;                                                   // getal delen door 10, sla op in 'getal2'
   
   if( number2 != 0 ){                                                          // als 'getal2' niet nul is
      uart_put_int_decimals( number2 );                                         // roep functie recursief aan
   }
   
   uart_put_char( '0' - ( number % 10 ));                                       // print decimaal
}

void uart_put_int( int number ){
   if( number < 0 ){                                                            // als getal kleiner is dan 0
      uart_put_char( '-' );                                                     // print '-' voor getal
   }
   else{                                                                        // als getal positief is
      number = -number;                                                         // maak negatief
   }
    
   uart_put_int_decimals( number );                                             // print alle decimalen
}


void uart_put_string( char *string ){
  while( *string != '\0' ){                                                     //zolang einde van string niet bereikt is
    uart_put_char( *string );                                                   //verzend karaketer op plaats s
    string++;                                                                   //verschuif 1 plaats verder
  }      
}
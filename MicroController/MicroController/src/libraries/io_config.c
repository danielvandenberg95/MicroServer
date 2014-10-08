#include "LPC11xx.h"
#include "io_config.h"


void io_config( unsigned char state ){
  if( state ){                                                                  // als IO config aan moet...
    LPC_SYSCON->SYSAHBCLKCTRL |= ( 1 << 16 );                                   // zet aan
  }
  else{                                                                         // als IO config uit moet...
    LPC_SYSCON->SYSAHBCLKCTRL &= ~( 1 << 16 );                                  // zet uit
  }
}
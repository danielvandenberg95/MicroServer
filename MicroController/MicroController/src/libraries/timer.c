/* ========================================================================== */
/*                                                                            */
/*   timer.c                                                                  */
/*   (c) 2012 Author Wouter van Ooijen                                        */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "timer.h"
#include "LPC11xx.h"

void timer_init(void){
  #ifndef _TIMER_C_INIT
  #define _TIMER_C_INIT
  LPC_SYSCON->SYSAHBCLKCTRL |= 1<<9;
  LPC_TMR32B0->CTCR = 0x00;
  LPC_TMR32B0->PR = 11;
  LPC_TMR32B0->TC = 0;
  LPC_TMR32B0->TCR = 0x01;
  #endif
}

unsigned int now(){
  return LPC_TMR32B0->TC;
}

void await(unsigned int t){
  while (t > now());
}

void delay(unsigned int t){
  t += now();
  await(t);
}


void delay_millis(unsigned int t){
  delay(t*1000);
}

void delay_seconds(unsigned int t){
  delay(t*1000000);
}
/* ========================================================================== */
/*                                                                            */
/*   PWM.c                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */
 
#define period 100
#define prescale 11;

#include "LPC11xx.h"

void pwm_init(){
    LPC_SYSCON->SYSAHBCLKCTRL |= 1<<16;
   LPC_SYSCON->SYSAHBCLKCTRL |= 1<<10;
   LPC_IOCON->R_PIO1_1 &= ~0x07;
   LPC_IOCON->R_PIO1_1 |= 0x03;
   
   LPC_TMR32B1->CTCR |= 0x00;
   LPC_TMR32B1->PR = prescale;
   LPC_TMR32B1->TC = 0;
   
   LPC_TMR32B1->PWMC |= 1;      //Configure CT32B1_MAT0 as PWM (hence it wll be celared when the counter is reset). 
   LPC_TMR32B1->MCR |= 1<<10;
   LPC_TMR32B1->MR3 = period;
   LPC_TMR32B1->EMR &= ~0x30;
   LPC_TMR32B1->EMR |= 0x10; 
   
   LPC_TMR32B1->MR0 = period;
   
   LPC_TMR32B1->TCR |= 0x01;
}

void pwm_set_value(int value){
   LPC_TMR32B1->MR0 = period - value;

}
/* ========================================================================== */
/*                                                                            */
/*   ADC.c                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "LPC11xx.h"
void pin_configure_as_adc(int port, int pin){
   LPC_IOCON->R_PIO1_0 &= ~0x07;
   LPC_IOCON->R_PIO1_0 |= 0x02;
   LPC_IOCON->R_PIO1_0 &= ~(1<<7);
    
   LPC_ADC->CR &=~0x07;
   LPC_ADC->CR |=(1<<1);
}

void adc_start(){
   LPC_ADC->GDR &= ~(1<<31);
   LPC_ADC->CR |= (1<<24);
} 

unsigned char adc_is_ready(){
  return ((LPC_ADC->GDR & 1<<31) != 0);
}

void adc_wait_for_ready(){
  while (!adc_is_ready()){}
}

int adc_read_value(){
  int d = 0;
  d = ((LPC_ADC->GDR)>>6)&((1<<10) - 1);
  return d;
}

int adc_read(){
  adc_start();
  adc_wait_for_ready();
  return adc_read_value();
}

void adc_init(){
   LPC_SYSCON->SYSAHBCLKCTRL |= 1<<13;
                                      
   //unsigned int save_clockcontrol = LPC_SYSCON->SYSAHBCLKCTRL; 
   LPC_SYSCON->SYSAHBCLKCTRL |= 1<<16;
   //LPC_SYSCON->SYSAHBCLKCTRL = save_clockcontrol;
   LPC_SYSCON->PDRUNCFG &= ~(1<<4);
   LPC_ADC->CR |= 0x0300;
} 
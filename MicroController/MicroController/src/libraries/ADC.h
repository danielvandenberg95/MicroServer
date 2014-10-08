/* ========================================================================== */
/*                                                                            */
/*   ADC.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */
      
void adc_init();                                
void pin_configure_as_adc(int port, int pin);
void adc_start();    
unsigned char adc_is_ready();
void adc_wait_for_ready();
int adc_read_value();
int adc_read();                     

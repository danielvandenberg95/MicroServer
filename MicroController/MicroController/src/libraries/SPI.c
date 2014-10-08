/* ========================================================================== */
/*                                                                            */
/*   SPI.c                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "timer.h"
#include "pins.h"
#include "SPI.h" 

void SPI_init(){
  timer_init();
                            
  pin_configure_as_output(CHIPSELECT);
  pin_configure_as_output(MOSI);
  pin_configure_as_output(SPICLOCK);
  
  pin_configure_as_input(MISO);
  
  pin_set(CHIPSELECT, 0); 
  pin_set(SPICLOCK,0);
  pin_set(MISO,0);
  //delay(DELAY);
}

void chipSelect(unsigned char x){
  pin_set(CHIPSELECT, x == 0);
}       

void sendByte(unsigned char byteOut){
  unsigned char bit;                                                          
  //delay(DELAY/2);
  for (bit = 0; bit < 8; bit ++){
    if (byteOut & 0x80){
      pin_set(MOSI,1);
    }else{
      pin_set(MOSI,0);
    }
    byteOut <<= 1;
    //delay(DELAY/2);
    pin_set(SPICLOCK, 1);
    //delay(DELAY);
    pin_set(SPICLOCK, 0);
  }
}  

void sendBytesFromTo(unsigned char bytesOut[],unsigned long start, unsigned long end){
  for (;start<end;start++){
    sendByte(bytesOut[start]);  
  }
}

void sendBytes(unsigned char bytesOut[],unsigned long n){
	sendBytesFromTo(bytesOut,0,n);
}

unsigned char readByte(){
  unsigned char byteIn = 0;
  unsigned char bit;
  for (bit = 0; bit < 8; bit ++){
    //delay(DELAY);           
    pin_set(SPICLOCK, 1);
    byteIn <<= 1;
    if (pin_get(MISO) == 1){
      byteIn |= 1;
    }
    //delay(DELAY);
    pin_set(SPICLOCK, 0); 
  }            
  return byteIn;  
}

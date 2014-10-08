/* ========================================================================== */
/*                                                                            */
/*   SPI.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _H_SPI
#define _H_SPI

#define CHIPSELECT 1,5
#define MOSI 1,9 //Master Out Slave In
#define MISO 1,4 //Master In Slave Out
#define SPICLOCK 1,8
#define DELAY 0

void SPI_init();
void chipSelect(unsigned char x);
void sendByte(unsigned char byteOut);
void sendBytes(unsigned char bytesOut[],unsigned long n);
void sendBytesFromTo(unsigned char bytesOut[],unsigned long start, unsigned long end);
unsigned char readByte();

#endif
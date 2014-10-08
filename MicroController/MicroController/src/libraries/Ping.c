/* ========================================================================== */
/*                                                                            */
/*   Ping.c                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "IP.h"
#include "SPI.h"
#include "Ethernet.h"

//unsigned int seqNR = 0;

void Ping_init(){
  IP_init();
  SPI_init();
  ethernet_init();
}

void sendPingReply(unsigned char destinationIP[4] , unsigned char destinationMac[6],unsigned int seqNR, unsigned int id){
  unsigned char data[] = {
    0x00,//Set type to ping response
    0x00,//Set the code to 0
    
    0x00,//Set the checksum (to be calculated later)
    0x00,
    
    id>>8,//Set the identifier to 1
    id&0xFF,
    
    seqNR>>8,//Set the sequence nr
    seqNR&0xFF,
    
    0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69
    
    
  };
  
  unsigned long checksum = 0;
  int i = 0;
  for (;i<sizeof(data)/sizeof(data[0]);i+=2){
    checksum += (data[i]<<8|data[i+1]);
    if (checksum > 0xFFFF){
      checksum += (checksum>>16);
      checksum = checksum&0xFFFF;
    }    
  }
  
  checksum = ~checksum;
  //checksum = 0x555b;
  data[2] = checksum>>8;
  data[3] = checksum & 0xFF; 
  
  #if DEBUGGING == 1                                            
    uart_put_string("Data size: ");
    uart_put_int_decimal(sizeof(data)/sizeof(data[0]));
    uart_put_char('\n');
   #endif          
  prepareHeaderIP(destinationIP, destinationMac, sizeof(data)/sizeof(data[0]), 0x01);
  sendBytes(data,sizeof(data)/sizeof(data[0]));
  sendPacket();
}

void processPingPacket(unsigned char data[], unsigned int dataSize){
  unsigned char destinationIP[4] = {data[26],data[27],data[28],data[29]};
  unsigned char destinationMac[6] = {data[6],data[7],data[8],data[9],data[10],data[11]};
  unsigned int seqNR = (data[40]<<8)|data[41];
  unsigned int id = (data[38]<<8)|data[39];
  sendPingReply(destinationIP,destinationMac,seqNR,id);
}

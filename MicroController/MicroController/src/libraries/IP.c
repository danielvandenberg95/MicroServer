/* ========================================================================== */
/*                                                                            */
/*   Filename.c                                                               */
/*   (c) %YEAR% Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */
#include "timer.h"
#include "Ethernet.h"
#include "SPI.h"
//#include "Ping.h"
#include "TCP.h"

unsigned char identification;

void IP_init(){
  timer_init();
  ethernet_init();
  SPI_init();
  TCP_init();
}

void prepareHeaderIP(unsigned char destinationIP[4], unsigned char destinationMac[6], unsigned int dataLength, unsigned char dataProtocol){
   unsigned char data[] = {
  0x08,//Set the package type to IP
  0x00,
  
  0x45,//Send the version and header length
  
  0x00,//Send DSCP and ECN (obsolete)
  
  (20+dataLength)>>8, //Send total length
  (20+dataLength)&0xFF,
  
  identification>>8,
  identification&0xFF,
  
  0x00,0x00,//Set fragment offset and flags
  
  128,//Set ttl
  
  dataProtocol,//Set data protocol
  
  0x00,0x00,//Keep the header checksum at 0 for now, to be calculated later.
  
  ip[0],ip[1],ip[2],ip[3],//Send source IP address
  destinationIP[0],destinationIP[1],destinationIP[2],destinationIP[3],//Send destination IP address
  
  };
  identification ++;
	unsigned int length = sizeof(data)/sizeof(data[0]);
	unsigned long checksum = 0;
	int i = 0;
	for (i=0;i<length;i+=2){
		checksum += (data[i]<<8|data[i+1]);
	}
	
	while (checksum > 0xFFFF){
		checksum += (checksum>>16);
		checksum = checksum&0xFFFF;
	}		
	
	checksum = ~checksum;
	checksum += 0x0800;
	data[12] = checksum>>8;
	data[13] = checksum & 0xFF;
	data[13] = checksum & 0xFF;
  
  initPacket(destinationMac);
  sendBytes(data,length);        
}

void processIPv4Packet(unsigned char data[], unsigned int dataSize){
  if (data[14] != 0x45 ||data[15] != 0x00){
    return;
  }
  //unsigned int length = data[16]<<8|data[17];
  //unsigned int ID = (data[18]<<8)|data[19];
  switch (data[23]){
    /*case 0x01:
      processPingPacket(data, dataSize);
    break;*/
    case 0x06:
      processTCPPacket(data, dataSize);
    break;
  }
  
}
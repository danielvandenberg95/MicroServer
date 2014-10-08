/* ========================================================================== */
/*                                                                            */
/*   ARP.c                                                               */
/*   (c)  2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "Ethernet.h"
#include "SPI.h"
#include "ENC28J60.h"


void ARP_init(){
  ethernet_init();
  ENC28J60_init();
}

void sendPacketARP(){
  unsigned char data[] = {
  0x08,//Set the package type to ARP
  0x06,
  
  0x00,//Send the hardware type
  0x01,
  
  0x08,//Send protocol type
  0x00,
  
  0x06,//Send hardware size
  
  0x04,//Send protocol size
  
  0x00,//Send operation code (opcode)
  0x01,//Request
  
  mac[0],//Send sender mac address;
  mac[1],
  mac[2],
  mac[3],
  mac[4],
  mac[5],
  
  ip[0],//Send sender IP
  ip[1],
  ip[2],
  ip[3],
  
  0x00,0x00,0x00,0x00,0x00,0x00,//Send target mac address (Empty, ignored in requests)
  0xa9,0xfe,100,100//Send target IP address
  
  };
  unsigned char targetMac[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  initPacket(targetMac);
  sendBytes(data,sizeof(data)/sizeof(data[0]));
  sendPacket();
}

void sendPacketARPReply(unsigned char dest[6]){
	unsigned char data[] = {
		0x08,//Set the package type to ARP
		0x06,

		0x00,//Send the hardware type
		0x01,

		0x08,//Send protocol type
		0x00,

		0x06,//Send hardware size

		0x04,//Send protocol size

		0x00,//Send operation code (opcode)
		0x02,//Reply

		mac[0],//Send sender mac address;
		mac[1],
		mac[2],
		mac[3],
		mac[4],
		mac[5],

		ip[0],//Send sender IP
		ip[1],
		ip[2],
		ip[3],

		dest[0],//Send target mac address (to be set later)
		dest[1],
		dest[2],
		dest[3],
		dest[4],
		dest[5],

		0,0,0,0//Send target IP address to empty, ignored in reply
	};

	initPacket(dest);
	sendBytes(data,sizeof(data)/sizeof(data[0]));
	sendPacket();
}

void processARPRequest(unsigned char data[], unsigned int datasize){
	unsigned char IP[4] = {data[38],data[39],data[40],data[41]};
	if (isOwnIP(IP)){
		sendPacketARPReply(IP);//What the heck? Yeah, let's pass the IP for mac...
	}
}

void processARPacket(unsigned char data[], unsigned int dataSize){
  switch (data[20]<<8|data[21]){
    case 0x0001:
      processARPRequest(data, dataSize);      
    break;
    case 0x0002://ARP reply
    break;
  }
}


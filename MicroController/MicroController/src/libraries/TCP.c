/*==========================================================================*/
/*																			*/
/*	 TCP.c																	*/
/*	 (c) 2014 Author Daniël van den Berg									*/
/*																			*/
/*	 Description															*/
/*																			*/
/*==========================================================================*/

#include "Ethernet.h"
#include "SPI.h"
#include "IP.h"
#include "timer.h"
#include "HTTP.h"

typedef struct{
	unsigned long seqNRServer;
	unsigned long seqNRClient;
	unsigned int clientPort;
	unsigned char clientIP[4];
	unsigned char clientMac[6];
}TCPSession;

TCPSession sessions[32];
unsigned char sessionNR = 0;

TCPSession getSession(unsigned long seqNRClient){
	int i = 0;
	for (;i<32;i++){
		if (sessions[i].seqNRClient == seqNRClient){
			return sessions[i];
		}
	}
	TCPSession session;
	session.seqNRClient = seqNRClient;
	session.clientPort = 0;
	sessions[sessionNR] = session;
	sessionNR ++;
	if (sessionNR >= 32){
		sessionNR -= 32;
	}
	
	return session;
}

unsigned long sequenceNR = 1;

void TCP_init(){
	ethernet_init();
	timer_init();
	HTTP_init();
	sequenceNR = 548461;
}

void prepareTCPHeader(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort,unsigned char flags,unsigned long seqNRClient, unsigned long seqNRServer, unsigned char payloadData[], unsigned int dataSize){
	seqNRClient += (flags == 0b00010010);
	unsigned char data[] = {
		0x00, //Set source port
		0x50,
		
		destPort>>8,//Set destination port
		destPort&0xFF,
		
		seqNRServer>>24,//Session number
		seqNRServer>>16&0xFF,
		seqNRServer>>8&0xFF,
		seqNRServer&0xFF,
		
		seqNRClient>>24,//Ack nr
		seqNRClient>>16&0xFF,
		seqNRClient>>8&0xFF,
		seqNRClient&0xFF,
		
		0x00,//Header length
		flags,//Set flags
		
		0x20,//Set window size
		0x00,
		
		0x00,//Set checksum
		0x00,/*0x00,0x00,*/
		
		0x00,//Set urgent pointer
		0x00,
		
		0x02,//Set options
		0x04,
	
		0x05,0x96,0x01,0x03,0x03,0x06/*,0x01,0x01,0x04,0x02*/
	};
	
	unsigned int headerLenght = sizeof(data)/sizeof(data[0]);
	data[12] = headerLenght<<2;
	
	unsigned char pseudoHeader[] = {
		ip[0],
		ip[1],
		ip[2],
		ip[3],
		destinationIP[0],
		destinationIP[1],
		destinationIP[2],
		destinationIP[3],
		0x00,
		0x06,
		(headerLenght+dataSize)>>8,
		(headerLenght+dataSize) & 0xFF
	};
	
	unsigned long checksum = 0;
	int i = 0;
	for (;i<sizeof(pseudoHeader)/sizeof(pseudoHeader[0]);i+=2){
		checksum += (pseudoHeader[i]<<8|pseudoHeader[i+1]);
		if (checksum > 0xFFFF){
			checksum += (checksum>>16);
			checksum = checksum&0xFFFF;
		}		
	}
	for (i=0;i<headerLenght;i+=2){
		checksum += (data[i]<<8|data[i+1]);
		if (checksum > 0xFFFF){
			checksum += (checksum>>16);
			checksum = checksum&0xFFFF;
		}		
	}
	for (i=0;i<dataSize;i+=2){
		if (i <= dataSize - 2){
			checksum += (payloadData[i]<<8|payloadData[i+1]);
		}else{
			checksum += (payloadData[i]<<8);
		}
		if (checksum > 0xFFFF){
			checksum += (checksum>>16);
			checksum = checksum&0xFFFF;
		}		
	}
	
	checksum = ~checksum;
	//checksum = 0x555b;
	data[16] = checksum>>8;
	data[17] = checksum & 0xFF;
	
	prepareHeaderIP(destinationIP, destinationMac, headerLenght + dataSize, 0x06);
	sendBytes(data,headerLenght);
}

void prepareTCPHeaderFromTo(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort,unsigned char flags,unsigned long seqNRClient, unsigned long seqNRServer, unsigned char payloadData[], unsigned int start, unsigned int end){
	unsigned int dataSize = end-start;

	seqNRClient += (flags == 0b00010010);
	unsigned char data[] = {
		0x00, //Set source port
		0x50,
		
		destPort>>8,//Set destination port
		destPort&0xFF,
		
		seqNRServer>>24,//Session number
		seqNRServer>>16&0xFF,
		seqNRServer>>8&0xFF,
		seqNRServer&0xFF,
		
		seqNRClient>>24,//Ack nr
		seqNRClient>>16&0xFF,
		seqNRClient>>8&0xFF,
		seqNRClient&0xFF,
		
		0x00,//Header length
		flags,//Set flags
		
		0x20,//Set window size
		0x00,
		
		0x00,//Set checksum
		0x00,/*0x00,0x00,*/
		
		0x00,//Set urgent pointer
		0x00,
		
		0x02,//Set options
		0x04,
	
		0x05,0x96,0x01,0x03,0x03,0x06/*,0x01,0x01,0x04,0x02*/
	};
	
	unsigned int length = sizeof(data)/sizeof(data[0]);
	data[12] = length<<2;
	
	unsigned char pseudoHeader[] = {
		ip[0],
		ip[1],
		ip[2],
		ip[3],
		destinationIP[0],
		destinationIP[1],
		destinationIP[2],
		destinationIP[3],
		0x00,
		0x06,
		(length+dataSize)>>8,
		(length+dataSize) & 0xFF
	};
	
	unsigned long checksum = 0;
	int i = 0;
	for (;i<sizeof(pseudoHeader)/sizeof(pseudoHeader[0]);i+=2){
		checksum += (pseudoHeader[i]<<8|pseudoHeader[i+1]);
		if (checksum > 0xFFFF){
			checksum += (checksum>>16);
			checksum = checksum&0xFFFF;
		}		
	}
	for (i=0;i<length;i+=2){
		checksum += (data[i]<<8|data[i+1]);
		if (checksum > 0xFFFF){
			checksum += (checksum>>16);
			checksum = checksum&0xFFFF;
		}		
	}
	for (i=start;i<end;i+=2){
		checksum += (payloadData[i]<<8|payloadData[i+1]);
		if (checksum > 0xFFFF){
			checksum += (checksum>>16);
			checksum = checksum&0xFFFF;
		}		
	}
	
	checksum = ~checksum;
	//checksum = 0x555b;
	data[16] = checksum>>8;
	data[17] = checksum & 0xFF;
	
	prepareHeaderIP(destinationIP, destinationMac, sizeof(data)/sizeof(data[0]) + dataSize, 0x06);
	sendBytes(data,sizeof(data)/sizeof(data[0]));
}

void processTCPPacket(unsigned char data[], unsigned int dataSize){
	if (((data[36]<<8)|data[37]) != 0x0050){//Unknown TCP destination port
		return;
	}
	unsigned char destinationIP[4] = {data[26],data[27],data[28],data[29]};
	unsigned char destinationMac[6] = {data[6],data[7],data[8],data[9],data[10],data[11]};
	unsigned int destinationPort = data[34]<<8|data[35];
	unsigned long seqNRClient = data[38]<<24|data[39]<<16|data[40]<<8|data[41];
	unsigned long seqNRServer = data[42]<<24|data[43]<<16|data[44]<<8|data[45];
	unsigned char flags = data[47];
	unsigned int startOfHTML = 0x22+(data[0x2E]>>4)*4;
	switch (flags){
		case 0x02://Connect request (SYN)
			prepareTCPHeader(destinationIP,destinationMac,destinationPort,0b00010010,seqNRClient,sequenceNR+now()+sessionNR,0,0);//SYN-ACK
			sendPacket();
		break;
		case 0x10://Ack
			
		break;
		case 0x11://Fin ack
			prepareTCPHeader(destinationIP,destinationMac,destinationPort,0b00010000,seqNRClient+1,seqNRServer,0,0);
			sendPacket();
		break;
		case 0x18://Push ack message
			processHTTPPacket(destinationIP,destinationMac,destinationPort,seqNRClient,seqNRServer,data,dataSize,startOfHTML);
		break;
	}
}
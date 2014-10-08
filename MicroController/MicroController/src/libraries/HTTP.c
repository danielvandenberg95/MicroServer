#include "HTTP.h"
#include "TCP.h"
#include "SPI.h"
#include "pins.h"
#include "Ethernet.h"
#include "index.h"
#include "stats.h"
#include "toggle.h"
#include "ADC.h"
#include "uart.h"
#include "timer.h"
#include "ARP.h"
#include "modules.h"

unsigned char site[1024];

void HTTP_init(){
	timer_init();
	SPI_init();
	ethernet_init();
	adc_init();
	uart_init();
}

void toggleOutput(unsigned char toggle[3]){
	if (toggle[1] == 'D'){
		toggle[0]='0';
		toggle[1] = '5';
	}
	if (toggle[1] == 'C'){
		toggle[0]='0';
		toggle[1] = '4';
	}
	pin_set(toggle[0]-'0',toggle[1]-'0',1-(toggle[2]-'0'));
}
	
void sendWebpage(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort, unsigned long seqNRClient, unsigned long seqNRServer){

	unsigned int adc = adc_read();
	(void)adc;//Supress unused, does not use any rom nor ram.
	unsigned int i = 0, offset = 0;
	while (SITE[i+offset] != '\0'){
		site[i%1024] = SITE[i+offset];
		if (SITE[i+offset] == '^'){
			if (SITE[i+offset+1] == 'A' && SITE[i+offset+2] == 'D' && SITE[i+offset+3] == 'C' && SITE[i+offset+4] == '^'){
				site[i%1024] = (adc / 1000) + '0';
				i++;
				site[i%1024] = ((adc % 1000) / 100) + '0';
				i++;
				site[i%1024] = ((adc % 100) / 10) + '0';
				i++;
				site[i%1024] = (adc % 10) + '0';
				offset+=1;
			}else if (SITE[i+offset+1] == 'M' && SITE[i+offset+2] == 'O' && SITE[i+offset+3] == 'D' && SITE[i+offset+4] == '^'){
				offset += 5;
				unsigned char array[64];
				
				int offset2;
				do {
					offset2 = i;
					getSite(array,64,0);
					uart_put_char(array[i-offset2]);
					uart_put_char('\n');
					while ((i-offset2) < 64 && array[i-offset2] != '\0'){
						site[i%1024] = array[i-offset2];
						if (i % 1024 == 1024-1){
							prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011000, seqNRClient, seqNRServer,site,1024);
							sendBytes(site,1024);
							sendPacket();
							seqNRServer += 1024;
						}
						i++;
						offset --;
					}
				} while (array[i-offset2] != '\0');
				i--;
			}else{
				if (SITE[i+offset+3] == '^'){
					site[i%1024]=pin_get(SITE[i+offset+1]-'0',SITE[i+offset+2]-'0')+'0';
					offset+=3;
				}
			}
		}
		if (i % 1024 == 1024-1){
			prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011000, seqNRClient, seqNRServer,site,1024);
			sendBytes(site,1024);
			sendPacket();
			seqNRServer += 1024;
		}
		i++;
	}
	prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011001, seqNRClient, seqNRServer,site,i%1024);
	sendBytes(site,i%1024);
	sendPacket();
}
	
void sendStats(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort, unsigned long seqNRClient, unsigned long seqNRServer){

	unsigned int adc = adc_read();
	(void)adc;//Supress unused, does not use any rom nor ram.
	unsigned int i = 0, offset = 0;
	while (STATS[i+offset] != '\0'){
		site[i%1024] = STATS[i+offset];
		if (STATS[i+offset] == '^'){
			if (STATS[i+offset+1] == 'A' && STATS[i+offset+2] == 'D' && STATS[i+offset+3] == 'C' && STATS[i+offset+4] == '^'){
				site[i%1024] = (adc / 1000) + '0';
				i++;
				site[i%1024] = ((adc % 1000) / 100) + '0';
				i++;
				site[i%1024] = ((adc % 100) / 10) + '0';
				i++;
				site[i%1024] = (adc % 10) + '0';
				offset+=1;
			}else{
				if (STATS[i+offset+3] == '^'){
					site[i%1024]=pin_get(STATS[i+offset+1]-'0',STATS[i+offset+2]-'0')+'0';
					offset+=3;
				}
			}
		}
		if (i % 1024 == 1024-1){
			prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011000, seqNRClient, seqNRServer,site,i%1024+1);
			sendBytes(site,i%1024+1);
			sendPacket();
			seqNRServer += i%1024+1;
		}
		i++;
	}
	prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011001, seqNRClient, seqNRServer,site,i%1024);
	sendBytes(site,i%1024);
	sendPacket();
}

void sendToggled(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort, unsigned long seqNRClient, unsigned long seqNRServer){
	unsigned int i = 0, offset = 0;
	while (TOGGLE[i+offset] != '\0'){
		site[i%1024] = TOGGLE[i+offset];
		if (i % 1024 == 1024-1){
			prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011000, seqNRClient, seqNRServer,site,i%1024+1);
			sendBytes(site,i%1024+1);
			sendPacket();
			seqNRServer += i%1024+1;
		}
		i++;
	}
	prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011001, seqNRClient, seqNRServer,site,i%1024);
	sendBytes(site,i%1024);
	sendPacket();
}
/*
void sendIcon(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort, unsigned long seqNRClient, unsigned long seqNRServer){
	unsigned int i = 0, offset = 0;
	while (ICON[i+offset] != '\0'){
		site[i%1024] = ICON[i+offset];
		if (i % 1024 == 1024-1){
			prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011000, seqNRClient, seqNRServer,site,i%1024+1);
			sendBytes(site,i%1024+1);
			sendPacket();
			seqNRServer += i%1024+1;
		}
		i++;
	}
	prepareTCPHeader(destinationIP , destinationMac, destPort, 0b00011001, seqNRClient, seqNRServer,site,i%1024);
	sendBytes(site,i%1024);
	sendPacket();
}*/

void processHTTPPacket(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort, unsigned long seqNRClient, unsigned long seqNRServer,unsigned char data[],unsigned int dataSize, unsigned int startOfHTML){
	seqNRClient += dataSize-startOfHTML;
	
	int i = startOfHTML;
	{//Make sure the request is an actual request
		unsigned char check[] = {"GET "};
		for (;i<4;i++){
			if (data[i] != check[i-startOfHTML]){
				return;
			}
		}
	}
	while (data[i] != '/'){ // find start of request URI
		i++;
	}
	
	unsigned char page[64];
	{//Copy requested URI into page[]
		int start = i+1;
		while (data[i] != ' ' && data[i] != '?'){
			i++;
			page[i-start] = data[i];
		}
	}
	
	if (page[0] == ' ' || page[0] == '?' || (page[0] == 'i' && page[1] == 'n' && page[2] == 'd' && page[3] == 'e' && page[4] == 'x')){
		//Handle toggles, and reply with the index page.
		while (data[i] != '?' && data[i] != ' '){
			i++;
		}
		if (data[i] == '?'){
		i++;
			unsigned int getStart = i;
			while (data[i] != ' '){
				i++;
			}
			unsigned int getEnd = i-1;
			unsigned int getSize = getEnd-getStart+1;
			unsigned char get[getSize];
			uart_put_string("Get: [");
			for (i = getStart;i<=getEnd;i++){
				get[i-getStart] = data[i];
				uart_put_char(get[i-getStart]);
			}
			i = 0;
			uart_put_string("]\n");
			do { //Keep handling toggles until no more are found.
				if (get[i] == '&'){
					i++;
				}
				if (get[i] == 't' && get[i+1] == '='){
					unsigned char TOGGLE[3];
					unsigned int j = 0;
					for (;j < 3;j++){
						TOGGLE[j] = get[i+2+j];
					}
					i = i+2+j;
					toggleOutput(TOGGLE);
				}/*
				if (get[i] == 'i' && get[i+1] == 'p' && get[i+2] == '='){
					unsigned char newIp[4] = {0,0,0,0};
					unsigned int j = 0;
					for (;j < 16;j++){
						if (get[i+2+j] == '.'){
							continue;
						}
						newIp[(int)j/4] *= 10;
						newIp[(int)j/4] += get[i+2+j]-'0';
					}
					i = i+2+j;
					uart_put_string("New IP:");
					for (j=0;j < 4;j++){
						ip[j] = newIp[j];
						uart_put_int_decimal(ip[j]);
						uart_put_char('\n');
					}
					uart_put_char('\n');
					sendPacketARP();				
				}*/
			}while (get[i] == '&');
			
		}
		sendWebpage(destinationIP, destinationMac, destPort, seqNRClient, seqNRServer);
	}else if(page[0] == 't' && page[1] == 'o' && page[2] == 'g' && page[3] == 'g' && page[4] == 'l' && page[5] == 'e'){
		while (data[i] != '?' && data[i] != ' '){
			i++;
		}
		if (data[i] == '?'){
		i++;
			unsigned int getStart = i;
			while (data[i] != ' '){
				i++;
			}
			unsigned int getEnd = i-1;
			unsigned int getSize = getEnd-getStart+1;
			unsigned char get[getSize];
			uart_put_string("Get: [");
			for (i = getStart;i<=getEnd;i++){
				get[i-getStart] = data[i];
				uart_put_char(get[i-getStart]);
			}
			i = 0;
			uart_put_string("]\n");
			do { //Keep handling toggles until no more are found.
				if (get[i] == '&'){
					i++;
				}
				if (get[i] == 't' && get[i+1] == '='){
					unsigned char TOGGLE[3];
					unsigned int j = 0;
					for (;j < 3;j++){
						TOGGLE[j] = get[i+2+j];
					}
					i = i+2+j;
					toggleOutput(TOGGLE);
				}/*
				if (get[i] == 'i' && get[i+1] == 'p' && get[i+2] == '='){
					unsigned char newIp[4] = {0,0,0,0};
					unsigned int j = 0;
					for (;j < 16;j++){
						if (get[i+2+j] == '.'){
							continue;
						}
						newIp[(int)j/4] *= 10;
						newIp[(int)j/4] += get[i+2+j]-'0';
					}
					i = i+2+j;
					uart_put_string("New IP:");
					for (j=0;j < 4;j++){
						ip[j] = newIp[j];
						uart_put_int_decimal(ip[j]);
						uart_put_char('\n');
					}
					uart_put_char('\n');
					sendPacketARP();				
				}*/
			}while (get[i] == '&');
			
		}
		sendToggled(destinationIP, destinationMac, destPort, seqNRClient, seqNRServer);/*
	}else if(page[0] == 'f' && page[1] == 'a' && page[2] == 'v' && page[3] == 'i' && page[4] == 'c' && page[5] == 'o' && page[6] == 'n' && page[7] == '.' && page[8] == 'i' && page[9] == 'c' && page[10] == 'o'){
		sendIcon(destinationIP, destinationMac, destPort, seqNRClient, seqNRServer);*/
	}else if(page[0] == 's' && page[1] == 't' && page[2] == 'a' && page[3] == 't' && page[4] == 's'){
		sendStats(destinationIP, destinationMac, destPort, seqNRClient, seqNRServer);
	}
}
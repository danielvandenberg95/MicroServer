/*==========================================================================*/
/*																			*/
/*		main.c																*/
/*		(c) 2014 Author Daniël van den Berg									*/
/*																			*/
/*		Description															*/
/*																			*/
/*==========================================================================*/
//#define DEBUGGING
//Ethernet pinger
										
#include "SPI.h"
#include "LPC11xx.h"
#include "timer.h"
#include "pins.h"
#include "uart.h"
#include "ARP.h"
#include "Ethernet.h"
#include "ENC28J60.h"
#include "IP.h"
#include "Ping.h"
#include "ADC.h"

static __INLINE void NVIC_SystemReset(void);
static void processPacket();
static void receivePacket();
static void init();

void processPacket(unsigned char receiveVectors[4], unsigned char data[], unsigned int dataSize){
	switch (data[12]<<8|data[13]){
		case 0x800:
			processIPv4Packet(data,dataSize);
		break;
		case 0x0806:
			processARPacket(data,dataSize);
		break;
		default:
			uart_put_string("Unknown package type ");
			uart_put_int_hexadecimal(data[12]<<8|data[13],4);
			uart_put_char('\n');
	}
}


void receivePacket(){
	if ((controlRegisterRead(0x1C) & 0b00000001) != 0){// RXERIF: Receive Error Interrupt Flag bit
		/*
			Normally, upon the receive error condition, the host controller would process any packets pending from the receive buffer and then make additional room for future packets by advancing the ERXRDPT registers (low byte first) and decrementing the EPKTCNT register. See Section 7.2.4 Freeing Receive Buffer Space for more information on processing packets. Once processed, the host controller should use the BFC command to clear the EIR.RXERIF bit. 
		*/
		uart_put_string("\nReceive error!!!\n");
		uart_put_string("Amount of buffered packages: ");
		uart_put_int_decimal(controlRegisterRead(0x19));
		uart_put_char('\n');
		controlRegisterWriteInt(0x0D,0x0C,readBufferStart);
		controlRegisterWriteBit(ECON2,6,1);
		controlRegisterWriteBit(0x1C,0,0);
	}
	//Check EPKTCNT to see if a packet is accepted. (0x19 on bank 1)
	switchBank(1);
	if (controlRegisterRead(0x19) == 0x00){
		switchBank(0);
		return;
	}
	switchBank(0);
	
	//Read packet
	
	
	int cPos = controlRegisterReadInt(0x0D,0x0C);
	controlRegisterWriteInt(0x01,0x00,cPos);
	chipSelect(1);
	sendByte(RBM);
	int nextPacketPointer = readByte() | (readByte()<<8);
	cPos += 6;
	unsigned char receiveVectors[4];
	unsigned int dPos = 0;
	for (;dPos < 4; dPos++){
		receiveVectors[dPos] = readByte();
		cPos ++;
	}
	unsigned int receivedDataSize = ((receiveVectors[1]<<8)|receiveVectors[0])-4;//-4 for CRC, which is auto-handled.
	
	if (receivedDataSize > 1500){
		uart_put_string("Packet discarded due to overwriting data limit of buffer.\n");
		chipSelect(0);
		
		controlRegisterWriteBit(ECON1,2,0);
		//Set the ERXRDPT (0x0D:0x0C) pointer to allow the hardware to override the package
		controlRegisterWriteInt(0x0D,0x0C,readBufferStart);
		//Set ERXST again, to reset.
		controlRegisterWriteInt(0x09,0x08,readBufferStart);
		controlRegisterWriteBit(ECON1,2,1);
		controlRegisterWriteBit(ECON2,6,1);
		
		//Restart microcontroller
		NVIC_SystemReset();
		return;
	}
	unsigned char data[receivedDataSize];
	
	dPos = 0;
	while (dPos < receivedDataSize){
		data[dPos] = readByte();
		cPos ++;
		dPos ++;
	}
	
	chipSelect(0);
	
	//Set the ERXRDPT (0x0D:0x0C) pointer to allow the hardware to override the package
	controlRegisterWriteInt(0x0D,0x0C,nextPacketPointer);
	controlRegisterWriteBit(ECON2,6,1);
	
	processPacket(receiveVectors,data,dPos);
}

void init(){
	ENC28J60_init();
	uart_init();
	timer_init();
	uart_put_string("\nEthernet controller\n");
	
	SPI_init();
	ethernet_init();
	ARP_init();
	IP_init();
	adc_init();
	
	//softReset();
	
	/* If corrupted:
	Wait for ESTAT.CLKRDY == 1
	Write ERXST to point to the start of the receive register
	Write ERXND to point to the end of the receive register
	*/
	
	while ((controlRegisterRead(0x1D)&1) != 1){ 
	};
	
	unsigned char targetMac[] = {0x00,0x00,0x00,0x00,0xFA,0x05};
	setMacAddress(targetMac);
	
	initSending();
	initReceiving();
	controlRegisterWriteBit(ECON1,2,1);
	
	pin_configure_as_adc(1,3);
	
	pin_configure_as_input(1,2);
	pin_configure_as_input(1,1);
	pin_configure_as_input(1,0);
	
	pin_configure_as_output(0,7);
	pin_configure_as_output(0,6);
	pin_configure_as_output(0,5);
	pin_configure_as_output(0,4);
	pin_set(0,7,0);
	pin_set(0,6,0);
	pin_set(0,5,0);
	pin_set(0,4,0);
}
int main(){
	
	init(); 
	controlRegisterWriteBit(ECON1,2,1);
	
	//Set econ2	
	switchBank(0);
	controlRegisterWriteBit(ECON2,7,1);
	
	sendPacketARP();
	
	while (1){
		receivePacket();
	}
}
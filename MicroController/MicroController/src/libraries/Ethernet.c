/* ========================================================================== */
/*                                                                            */
/*   Ethernet.c                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */
                     
#include "Ethernet.h"
#include "SPI.h"
#include "ENC28J60.h"
#include "timer.h"

unsigned char ip[4] = {169,254,100,100};

void ethernet_init(){
  SPI_init();
  ENC28J60_init();
  timer_init();
  
  
  unsigned char targetMac[6] = {0x00,0x00,0x00,0x00,0xFA,0x05};
  setMacAddress(targetMac); 
}

void sendMacBytes(){
  sendBytes(mac,6);  
}

unsigned char isOwnIP(unsigned char testIP[4]){
  int i = 4;
    while (i){
      if (testIP[i] != ip[i]){
        return 0;
      }
      i--;
    }  
  return 1;  
}

void sendIPBytes(){
  sendBytes(ip,4);
}

void sendMacBytesTarget(unsigned char targetMac[6]){
  sendBytes(targetMac,6);
}

void sendIPBytesTarget(unsigned char IP[4]){
  sendBytes(IP,4);
}

void setMacAddress(unsigned char targetMac[6]){
  switchBank(3);
  unsigned char index = 0;
  for (;index <= 5;index ++){
    mac[index] = targetMac[index];
  }
  controlRegisterWriteByte(0x00, mac[4]);
  controlRegisterWriteByte(0x01, mac[5]);
  controlRegisterWriteByte(0x02, mac[2]);
  controlRegisterWriteByte(0x03, mac[3]);
  controlRegisterWriteByte(0x04, mac[0]);
  controlRegisterWriteByte(0x05, mac[1]);
}

void sendPacket(){
  chipSelect(0);
  int writePointer = controlRegisterReadInt(0x03,0x02);//Read the position of the buffer write pointer (EWRPT(H/L))
  writePointer --;
  controlRegisterWriteInt(0x07,0x06,writePointer); //Program ETXND pointer to be the last byte in the data payload, which is equal to the write pointer - 1
  controlRegisterWriteBit(ECON1,3,1);                          
  while ((controlRegisterRead(ECON1) & (1<<3)) == (1<<3)){
    delay(1);
  }
}

void sendPacketStartEnd(int start, int end){
	controlRegisterWriteInt(0x05,0x04,start);
	controlRegisterWriteInt(0x07,0x06,end);
	controlRegisterWriteBit(ECON1,3,1);
	while ((controlRegisterRead(ECON1) & (1<<3)) == (1<<3)){
		delay(1);
	}
}

void initPacket(unsigned char targetMac[6]){
  //Write 01 to ETXSTL, and 20 to ETXSTH (0120h combined)
  //Use WBM SPI (01111010 dddddddd dddddddd ...) command to write the per packet control byte, the destination address, the source MAC, type/length and data payload
  //Write the EXTND pointer to point to the end of the packet
  //Clear EIR.TXIF, set EIE.TXIE. (Set EIE.INTIE for interrupt)
  //Start transmission by setting the ECON1.TXRTS
  //Wait for ECON1.TXRTS to be cleared and the sending to be completed.
  //Optionally:
  //Check ESTAT.TXABRT to see if the transmission was successfull.
  switchBank(0);
  int start = 0x0028;
  controlRegisterWriteInt(0x05,0x04,start);
  controlRegisterWriteInt(0x03,0x02,start);
  
  controlRegisterWriteBit(0x1C,3,0);
  controlRegisterWriteBit(0x1B,3,1);
  
  //Write per packet control byte  
  unsigned char ppcb = 0b00000000;
  chipSelect(1);   
  sendByte(WBM);
  sendByte(ppcb);
  sendMacBytesTarget(targetMac);
  sendMacBytes();
}

unsigned char checkPacket(){
  switchBank(0b00);
  return controlRegisterRead(0x1D);
}


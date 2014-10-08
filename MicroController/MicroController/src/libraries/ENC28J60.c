/* ========================================================================== */
/*                                                                            */
/*   ENC28J60.c                                                               */
/*   (c) %YEAR% Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "SPI.h"
#include "ENC28J60.h"

//Ethernet buffer organization
/*
  Transmit buffer start: The pointer to the position at which the transmit buffer starts.
    ETXST (H:L/0x05:0x04)
  Buffer write pointer: The position to which there will be written.
    EWRPT (H:L/0x03:0x02)
  Transmit buffer end: The pointer to the position at which the transmit buffer ends.
    ETXND (H:L/0x07:0x06)
  Receive buffer start: The pointer to the position at which the receive buffer starts.
    ERXST (H:L/0x09:0x08)
  Buffer read pointer: The pointer at which position from the buffer data will be read.
    ERDPT (H:L/0x01:0x00)
  Receive buffer read pointer: The pointer that indicates where to start reading for a packet. 
  The hardware will not write past this position.
    ERXRDPT (H:l/0x0D:0x0C)
  Receive buffer end: The pointer to the position at which the receive buffer ends.
    ERXND (H:L/0x0B:0x0A)
  Hardware write pointer: The position where the hardware will write to.
    ERXWRPT (H:L 0x0F:0x0E) (Read-only)
  
*/


void ENC28J60_init(){
  SPI_init();
}

unsigned char controlRegisterRead(unsigned char reg){
  chipSelect(1);                                                   
  sendByte(reg & 0b00011111);
  unsigned char res = readByte();
  chipSelect(0);
  return res;
}

int controlRegisterReadInt(unsigned char regH, unsigned char regL){
  int result = (controlRegisterRead(regH)<<8)|controlRegisterRead(regL);
  return result;
} 

void controlRegisterWriteByte(unsigned char reg, unsigned char value){         
  chipSelect(1);                                              
  sendByte(0b01000000 | (0b00011111 & reg));                                                        
  sendByte(value);
  chipSelect(0);
}
                                                                                          
void controlRegisterWriteInt(unsigned char regH, unsigned char regL, unsigned int value){ 
  controlRegisterWriteByte(regL,value&0xFF);
  controlRegisterWriteByte(regH,value>>8);
}

void controlRegisterWriteBit(unsigned char reg, unsigned char pos, unsigned char bit){
  unsigned char value = controlRegisterRead(reg);
  if (!bit){
    value &= ~(1<<pos);
  }else{
    value |= 1<<pos;
  }
  controlRegisterWriteByte(reg,value);
}

void switchBank(unsigned char bank){
  unsigned char value = controlRegisterRead(ECON1);
  value &= ~(0b00000011);
  value |= (0b00000011&bank);
  controlRegisterWriteByte(ECON1,value);
}

void initSending(){
  /*//Make sure the receive logic is disabled by clearing ECON1.RXEN
  controlRegisterWriteBit(ECON1,2,0);*/

  /*
   Set the MARXEN bit in MACON1 to enable the MAC to receive frames. If using full duplex, most applications should also set TXPAUS and RXPAUS to allow IEEE defined flow control to function.
   Configure the PADCFG, TXCRCEN and FULDPX bits of MACON3. Most applications should enable automatic padding to at least 60 bytes and always append a valid CRC. For convenience, many applications may wish to set the FRMLNEN bit as well to enable frame length status reporting. The FULDPX bit should be set if the application will be connected to a full-duplex configured remote node; otherwise, it should be left clear. 
   Configure the bits in MACON4. For conform- ance to the IEEE 802.3 standard, set the DEFER bit.
   Program the MAMXFL registers with the maxi- mum frame length to be permitted to be received or transmitted. Normal network nodes are designed to handle packets that are 1518 bytes or less. 
   Configure the Back-to-Back Inter-Packet Gap register, MABBIPG. Most applications will pro- gram this register with 15h when Full-Duplex mode is used and 12h when Half-Duplex mode is used. 
   Configure the Non-Back-to-Back Inter-Packet Gap register low byte, MAIPGL. Most applications will program this register with 12h. 
   If half duplex is used, the Non-Back-to-Back Inter-Packet Gap register high byte, MAIPGH, should be programmed. Most applications will program this register to 0Ch
   If Half-Duplex mode is used, program the Retransmission and Collision Window registers, MACLCON1 and MACLCON2. Most applications will not need to change the default Reset values. If the network is spread over exceptionally long cables, the default value of MACLCON2 may need to be increased. 
   Program the local MAC address into the MAADR1:MAADR6 registers. 
  */

  switchBank(2);
  //Set the MARXEN bit in MACON1 to enable the MAC to receive frames.
  controlRegisterWriteBit(0x00,0,1);
  //Configure the PADCFG, TXCRCEN and FULDPX bits of MACON3. Most applications should enable automatic padding to at least 60 bytes and always append a valid CRC. For convenience, many applications may wish to set the FRMLNEN bit as well to enable frame length status reporting. The FULDPX bit should be set if the application will be connected to a full-duplex configured remote node; otherwise, it should be left clear.
  unsigned char macon3 = controlRegisterRead(0x02);
  macon3 |= 0b00110010;
  macon3 &= ~0b11000001;
  controlRegisterWriteByte(0x02,macon3);
  //Configure the bits in MACON4. For conform- ance to the IEEE 802.3 standard, set the DEFER bit.
  controlRegisterWriteBit(0x03,6,1);
  //Program the MAMXFL registers with the maximum frame length to be permitted to be received or transmitted. Normal network nodes are designed to handle packets that are 1518 bytes or less.
  controlRegisterWriteInt(0x0B,0x0A,MAMXFL);
  //Configure the Back-to-Back Inter-Packet Gap register, MABBIPG. Most applications will pro- gram this register with 15h when Full-Duplex mode is used and 12h when Half-Duplex mode is used.
  controlRegisterWriteByte(0x04,12);
  //Configure the Non-Back-to-Back Inter-Packet Gap register low byte, MAIPGL. Most applications will program this register with 12h. 
  controlRegisterWriteByte(0x06,12);
  //If half duplex is used, the Non-Back-to-Back Inter-Packet Gap register high byte, MAIPGH, should be programmed. Most applications will program this register to 0Ch
  controlRegisterWriteByte(0x07,0x0C);
  //If Half-Duplex mode is used, program the Retransmission and Collision Window registers, MACLCON1 and MACLCON2. Most applications will not need to change the default Reset values. If the network is spread over exceptionally long cables, the default value of MACLCON2 may need to be increased. 
  
  //Program the local MAC address into the MAADR1:MAADR6 registers.        
  //Output mac address to terminal
  
  switchBank(0);
  
  /*//Enable reception by setting ECON1.RXEN. 
  //DO NOT modify duplex, receive buffer start and end pointers AFTER THIS POINT!
  controlRegisterWriteBit(ECON1,2,1);*/
}

void initReceiving(){
  //Make sure the receive logic is disabled by clearing ECON1.RXEN
  controlRegisterWriteBit(ECON1,2,0);
  switchBank(0);
  
  //No interrupts desired, so clear EIE.PKTIE, EIE.INTIE, EIE.RXERIE and EIR.RXERIF
  /*controlRegisterWriteByte(0x1B,controlRegisterRead(0x1B)&0b00111110);
  controlRegisterWriteBit(0x1C,0,0);*/
  
  //ERXFCON: ETHERNET RECEIVE FILTER CONTROL REGISTER (Bank 1, address 0x18. Default:  1010 0001)
  //Default: Packets with a matching MAC address or a broadcast address (FF-F.....F-FF) will be accepted 
  //controlRegisterWriteByte(0x18,0b00000000); 
  
  //Set receive buffer start and end pointers (ERXSTH:ERXSTL / 0x09:0x08 and ERXNDH:ERXNDL / 0x0B:0x0A)
  controlRegisterWriteInt(0x09,0x08,0x0900);//0x08FF + 1, even address.
  controlRegisterWriteInt(0x0B,0x0A,0x1FFF);//0x1FFF
  
  //Set the ERXRDPT (0x0D:0x0C) pointer to make sure the hardware can write to the receive buffer
  controlRegisterWriteInt(0x0D,0x0C,0x0900);//0x08FF + 1, even address.
  
  //Enable reception by setting ECON1.RXEN. 
  //DO NOT modify duplex, receive buffer start and end pointers AFTER THIS POINT!
  controlRegisterWriteBit(ECON1,2,1);
  
  //When a packet is received the EIR.PKTIF bit will be set.       
}



void softReset(){
  chipSelect(1);
  sendByte(0xFF);
  chipSelect(0);
}

int receiveBufferFreeSpace(){
/*
  if ERXWRPT > ERXRDPT, then Free Space = (ERXND  ERXST)  (ERXWRPT  ERXRDPT) else if ERXWRPT = ERXRDPT, then Free Space = (ERXND  ERXST) else Free Space = ERXRDPT  ERXWRPT  1
*/
  switchBank(0);                                                         
  int erxrdpt = controlRegisterReadInt(0x0D,0x0C);
  int erxwrpt = controlRegisterReadInt(0x0F,0x0E);
  int erxst = controlRegisterReadInt(0x09,0x08);
  int erxnd = controlRegisterReadInt(0x0B,0x0A);
  if (erxwrpt > erxrdpt){ 
    return (erxnd - erxst) - (erxwrpt - erxrdpt);
  }else if (erxwrpt == erxrdpt){
    return erxnd - erxst;
  }
  return erxrdpt - erxwrpt - 1;
}
/* ========================================================================== */
/*                                                                            */
/*   Ethernet.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _H_ETHERNET
#define _H_ETHERNET
        
unsigned char mac[6];
//unsigned char ip[4] = {0xa9,0xfe,0x8d,0xb0};
//unsigned char ip[4] = {0x00,0x00,0x00,0x00};
unsigned char ip[4];

void ethernet_init();

void printMacAddress();
void sendMacBytes();
//unsigned char isOwnMac(unsigned char testMac[6]);
unsigned char isOwnIP(unsigned char testIP[4]);
void sendIPBytes();
void sendMacBytesTarget(unsigned char targetMac[6]);
void setMacAddress(unsigned char targetMac[6]);
void sendPacket();
void sendPacketStartEnd(int start, int end);
void initPacket(unsigned char targetMac[6]);
unsigned char checkPacket();

#endif
/* ========================================================================== */
/*                                                                            */
/*   ARP.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _H_ARP
#define _H_ARP

void ARP_init();
void sendPacketARP();
void writeToBuffer();
void sendPacketARPReply(unsigned char dest[6]);
void processARPRequest(unsigned char data[], unsigned int datasize);
void processARPacket(unsigned char data[], unsigned int dataSize);

#endif
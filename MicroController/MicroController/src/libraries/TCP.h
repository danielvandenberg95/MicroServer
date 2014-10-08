/* ========================================================================== */
/*                                                                            */
/*   TCP.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

void TCP_init();
void processTCPPacket(unsigned char data[], unsigned int dataSize);
void prepareTCPHeader(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort,unsigned char flags,unsigned long seqNRClient, unsigned long seqNRServer, unsigned char payloadData[], unsigned int dataSize);
void prepareTCPHeaderFromTo(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort,unsigned char flags,unsigned long seqNRClient, unsigned long seqNRServer, unsigned char payloadData[], unsigned int start, unsigned int end);
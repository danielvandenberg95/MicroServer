/* ========================================================================== */
/*                                                                            */
/*   IP.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

void IP_init();
//!Prepares an IP header
//
//!Prepares and sends an IP header to the network chip. Use sendBytes() to add data, and then use sendPacket() to send the packet.
void prepareHeaderIP(unsigned char destinationIP[4], unsigned char destinationMac[6], unsigned int dataLength, unsigned char dataProtocol);

void processIPv4Packet(unsigned char data[], unsigned int dataSize);
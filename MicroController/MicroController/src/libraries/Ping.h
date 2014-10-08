/* ========================================================================== */
/*                                                                            */
/*   Ping.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

void Ping_init();
void processPingPacket(unsigned char data[], unsigned int dataSize);

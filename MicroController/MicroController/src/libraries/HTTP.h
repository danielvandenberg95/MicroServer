void HTTP_init();

void processHTTPPacket(unsigned char destinationIP[4] , unsigned char destinationMac[6], unsigned int destPort, unsigned long seqNRClient, unsigned long seqNRServer,unsigned char data[],unsigned int dataSize, unsigned int startOfHTML);
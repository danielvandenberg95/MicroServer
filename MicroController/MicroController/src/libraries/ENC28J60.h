/* ========================================================================== */
/*                                                                            */
/*   ENC28J60.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _H_ENC28J60
#define _H_ENC28J60 

#define ECON1 0x1F
#define ECON2 0x1E

#define WBM 0b01111010
#define RBM 0b00111010
#define MAMXFL 1518

#define readBufferStart 0x08FF

void ENC28J60_init();
unsigned char controlRegisterRead(unsigned char reg);
int controlRegisterReadInt(unsigned char regH, unsigned char regL);
void controlRegisterWriteByte(unsigned char reg, unsigned char value);
void controlRegisterWriteInt(unsigned char regH, unsigned char regL, unsigned int value);
void controlRegisterWriteBit(unsigned char reg, unsigned char pos, unsigned char bit);
void switchBank(unsigned char bank);

void initSending();
void initReceiving();
void softReset();
int receiveBufferFreeSpace();

#endif
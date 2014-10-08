#ifndef _modules_h
#define _modules_h
void init();
//!Array has got to be a pointer to the charArray to fill.
void getSite( unsigned char* array, int size, unsigned char id);
void sendValue( char value, unsigned char id );
void sendValues( char* valueArray, int size, unsigned char id);
void selectChip(unsigned char id);
void deselectChip();
void assignIDs();
#endif
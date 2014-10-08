#include "modules.h"
#include "i2c.h"

void init(){
	i2c_init();
}

char status = 0;
/*
0	-	Not busy
1	-	Selected chip
2	-	Reading site
*/

void selectChip(unsigned char id){
	i2c_start();
	i2c_put_byte(id);
}

void deselectChip(){
	i2c_stop();
	status = 0;
}

void getSite( unsigned char* array, int size, unsigned char id){
	array[0] = 'M';
	array[1] = 'o';
	array[2] = 'd';
	array[3] = 'u';
	array[4] = 'l';
	array[5] = 'e';
	array[6] = '\0';
	/*if (status != 1 && status != 2){
		selectChip(id);
	}
	int i = 0;
	for (;i<size;i++){
		array[i] = i2c_get_byte();
		if (array[i] == '\0'){
			deselectChip();
			return;
		}
	}*/
}
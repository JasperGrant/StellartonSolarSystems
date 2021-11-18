#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"

int initsales(void){
	
	FILE * output = fopen("salesrelativefile.txt","w");
	
	HEADER header;
	header.first_id = 1;
	
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, output);
	
	fclose(output);
}

int makesale(void){
	printf("Enter Customer ID: ");
	
	printf("Enter Product ID: ");
}

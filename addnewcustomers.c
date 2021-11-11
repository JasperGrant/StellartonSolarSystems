#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int addnewcustomers(void){
	CUSTOMER newcustomer;
	char filename[NAMELEN];
	char customerarray[256];
	printf("Enter input file name:\n");
	scanf("%s", &filename);
	FILE * input = fopen(filename, "r+");
	while(fgets(customerarray, 256, input) != NULL){
		strtok(customerarray, ",")
	}
	printf("%s", filename);
	
	
	
}

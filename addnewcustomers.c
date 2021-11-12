#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int addnewcustomers(void){
	CUSTOMER newcustomer;
	char filename[MAXLEN];
	char *element;
	char temprecord[MAXREC];
	//printf("Enter input file name:\n");
	//scanf("%s", &filename);
	FILE * input = fopen("Customers.txt", "r");
	FILE * output = fopen("customersrelativefile.txt", "a+");
	fwrite(element, MAXLEN, 1, output);
	long examplecid = 1001;
	while (fgets(temprecord, MAXREC, input)){
		fwrite(&examplecid, MAXLEN, 1, output);
		element = strtok(temprecord, ",");
		while (element){
			
			printf("%s\n", element);
			fwrite(element, MAXLEN, 1, output);
			element = strtok(NULL, ",");
		}
		examplecid++;
	}
	fclose(input);
	fclose(output);
	
}

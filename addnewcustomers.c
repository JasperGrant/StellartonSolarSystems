#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"

int initcustomers(void){

	CUSTOMER newcustomer;
	char filename[MAXLEN];
	char *element;
	char temprecord[MAXREC];
	//printf("Enter input file name:\n");
	//scanf("%s", &filename);
	FILE * input = fopen("Customers.txt", "r");
	FILE * output = fopen("customersrelativefile.txt", _access("customersrelativefile.txt", 0) < 0 ? "w" : "r+");
	//fwrite(element, MAXLEN, 1, output);
	//long examplecid = 1001;
	while (fgets(temprecord, MAXREC, input)){
		//fwrite(&examplecid, MAXLEN, 1, output);
		long customerid = 1000;
		newcustomer.CID = customerid;
	
		element = strtok(temprecord, ",");
		TRUNCATE(element);
		strcpy(newcustomer.name, element);
		
		element = strtok(NULL, ",");	
		TRUNCATE(element);
		strcpy(newcustomer.businessname, element);
	
		element = strtok(NULL, ",");
		TRUNCATE(element);
		strcpy(newcustomer.streetaddress, element);
		
		element = strtok(NULL, ",");	
		TRUNCATE(element);
		strcpy(newcustomer.town, element);
		
		element = strtok(NULL, ",");	
		TRUNCATE(element);
		strcpy(newcustomer.province, element);
		
		element = strtok(NULL, ",");	
		TRUNCATE(element);
		strcpy(newcustomer.postalcode, element);
		
		element = strtok(NULL, ",");	
		TRUNCATE(element);
		strcpy(newcustomer.postalcode, element);
		
		element = strtok(NULL, ",");	
		TRUNCATE(element);
		strcpy(newcustomer.telephone, element);
		
		customerid++;
		
		fseek(output, (newcustomer.CID - 1000) * sizeof(CUSTOMER), SEEK_SET);
		fwrite(&newcustomer, sizeof(CUSTOMER), 1, output);
	}
	fclose(input);
	fclose(output);
	
}

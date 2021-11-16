#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"

int initcustomers(void){

	HEADER_CUSTOMER header;

	CUSTOMER newcustomer;
	char *element;
	char temprecord[MAXREC];
	
	FILE * input = fopen("Customers.txt", "r");
	FILE * output = fopen("customersrelativefile.txt", _access("customersrelativefile.txt", 0) < 0 ? "w" : "r+");

	long customerid = header.first_cid = 1001; //1001 is the first CID
	
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER_CUSTOMER), 1, output);
	
	fgets(temprecord, MAXREC, input); //Disregard first line
	
	while (fgets(temprecord, MAXREC, input)){
		TRUNCATE(temprecord);
		newcustomer.CID = customerid;
		
		element = strtok(temprecord, ",");
		strcpy(newcustomer.name, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.businessname, element);
	
		element = strtok(NULL, ",");
		strcpy(newcustomer.streetaddress, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.town, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.province, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.postalcode, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.telephone, element);
		
		customerid++;
		fseek(output, ((newcustomer.CID - 1001) * sizeof(CUSTOMER)) + sizeof(HEADER_CUSTOMER), SEEK_SET);
		fwrite(&newcustomer, sizeof(CUSTOMER), 1, output);
	}
	fclose(input);
	fclose(output);
	
	return 0;
	
}

int readcustomers(void){
	CUSTOMER customer;
	FILE * output = fopen("customersrelativefile.txt", "r+");
	
	fseek(output, sizeof(HEADER_CUSTOMER), SEEK_SET);
	fread(&customer, sizeof(CUSTOMER), 1, output);
	printf("Customer:%s, %d, %s, %s, %s, %s, %s, %s\n", customer.name, customer.CID, customer.businessname, customer.streetaddress, customer.town, customer.province, customer.postalcode, customer.telephone);
}

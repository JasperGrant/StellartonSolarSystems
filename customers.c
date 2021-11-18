#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"

int initcustomers(void){

	//Init structs
	HEADER header;
	CUSTOMER customer;

	char *element;
	char temprecord[MAXREC];
	
	FILE * input = fopen("Customers.txt", "r");
	FILE * cfd = fopen("customersrelativefile.txt", _access("customersrelativefile.txt", 0) < 0 ? "w" : "r+");

	long customerid = header.first_id = 1000; //1000 is the first CID

	fgets(temprecord, MAXREC, input); //Disregard first line in input
	
	while (fgets(temprecord, MAXREC, input)){
		TRUNCATE(temprecord);
		customer.CID = customerid;
		
		element = strtok(temprecord, ",");
		strcpy(customer.name, element);
		
		element = strtok(NULL, ",");	
		strcpy(customer.businessname, element);
	
		element = strtok(NULL, ",");
		strcpy(customer.streetaddress, element);
		
		element = strtok(NULL, ",");	
		strcpy(customer.town, element);
		
		element = strtok(NULL, ",");	
		strcpy(customer.province, element);
		
		element = strtok(NULL, ",");	
		strcpy(customer.postalcode, element);
		
		element = strtok(NULL, ",");	
		strcpy(customer.telephone, element);
		
		customerid++;
		fseek(cfd, ((customer.CID - 1000) * sizeof(CUSTOMER)) + sizeof(HEADER), SEEK_SET);
		fwrite(&customer, sizeof(CUSTOMER), 1, cfd);
	}
	header.first_id = customerid;
	fseek(cfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, cfd);
	
	fclose(input);
	fclose(cfd);
	
	return 0;
	
}

int readcustomers(void){
	
	//Init structs
	CUSTOMER customer;
	HEADER header;
	
	FILE * cfd = fopen("customersrelativefile.txt", "r+");
	
	fseek(cfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, cfd);
	printf("Next CID: %ld\n", header.first_id);
	for(int i = 0;i<header.first_id-1000; i++){
		fseek(cfd, i*sizeof(CUSTOMER) + sizeof(HEADER), SEEK_SET);
		fread(&customer, sizeof(CUSTOMER), 1, cfd);
		printf("%ld, %s, %s, %s, %s, %s, %s, %s\n", 
		customer.CID, customer.name, customer.businessname, customer.streetaddress, 
		customer.town, customer.province, customer.postalcode, customer.telephone);
	}
	fclose(cfd);
}





int addnewcustomers(void)
{
	//Init structs
	CUSTOMER customer;
	HEADER header;
	
	
	FILE * cfd = fopen("customersrelativefile.txt", "r+");
	
	
	/* Access header record to get first available customer id */
	fseek(cfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, cfd);
	
	
	fflush(stdin);
	
	printf("Enter name\n");
	fgets(customer.name, MAXLEN, stdin);
	TRUNCATE(customer.name);
	
	printf("Enter businessname\n");
	fgets(customer.businessname, MAXLEN, stdin);
	TRUNCATE(customer.businessname);
	
	printf("Enter street address\n");
	fgets(customer.streetaddress, MAXLEN, stdin);
	TRUNCATE(customer.streetaddress);
	
	printf("Enter town\n");
	fgets(customer.town, MAXLEN, stdin);
	TRUNCATE(customer.town);
	
	printf("Enter province\n");
	fgets(customer.province, MAXLEN, stdin);
	TRUNCATE(customer.province);
	
	printf("Enter postalcode\n");
	fgets(customer.postalcode, MAXLEN, stdin);
	TRUNCATE(customer.postalcode);
	
	printf("Enter Telephone\n");
	fgets(customer.telephone, MAXLEN, stdin);
	TRUNCATE(customer.telephone);
	
	//printf("\nID: %d\n", header.first_id);
	fseek(cfd, sizeof(HEADER) + (header.first_id-1000) * sizeof(CUSTOMER), SEEK_SET);
	customer.CID = header.first_id;
	fwrite(&customer, sizeof(CUSTOMER), 1, cfd);
	
	header.first_id++;
	
	fseek(cfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, cfd);
	
	fclose(cfd);
	
}










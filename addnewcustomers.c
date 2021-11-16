#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"

int initcustomers(void){

	HEADER header;

	CUSTOMER customer;
	char *element;
	char temprecord[MAXREC];
	
	FILE * input = fopen("Customers.txt", "r");
	FILE * output = fopen("customersrelativefile.txt", _access("customersrelativefile.txt", 0) < 0 ? "w" : "r+");

	long customerid = header.first_id = 1001; //1001 is the first CID
	
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, output);
	
	fgets(temprecord, MAXREC, input); //Disregard first line
	
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
		fseek(output, ((customer.CID - 1001) * sizeof(CUSTOMER)) + sizeof(HEADER), SEEK_SET);
		fwrite(&customer, sizeof(CUSTOMER), 1, output);
	}
	header.first_id = customerid;
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, output);
	
	fclose(input);
	fclose(output);
	
	return 0;
	
}

int readcustomers(void){
	CUSTOMER customer;
	HEADER header;
	FILE * output = fopen("customersrelativefile.txt", "r+");
	
	fseek(output, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, output);
	printf("Header: %ld\n", header.first_id);
	for(int i = 0;i<header.first_id-1001; i++){
		fseek(output, i*sizeof(CUSTOMER) + sizeof(HEADER), SEEK_SET);
		fread(&customer, sizeof(CUSTOMER), 1, output);
		printf("Customer: %s, %ld, %s, %s, %s, %s, %s, %s\n", 
		customer.name, customer.CID, customer.businessname, customer.streetaddress, 
		customer.town, customer.province, customer.postalcode, customer.telephone);
	}
}





int addnewcustomers(void)
{
	/* Add a customer to the customerfile */
	CUSTOMER customer;
	HEADER header;
	
	
	FILE * cfd = fopen("customersrelativefile.txt", "r+");
	
	
	/* Access header record to get first available customer id */
	fseek(cfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, cfd);
	
	
	/* Geting all the information */
	
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
	fgets(customer.postalcode, MAXLEN, stdin);
	TRUNCATE(customer.postalcode);
	
	
	
	/* Complete remaining fields */
	/* Display complete record */
	//printf("ID: >%d< Name: >%s< businessname: >%s< Province: >%s<  Town: >%s<  postal code: >%s<  telephone: >%s<\n", 
	//	customer.CID, customer.name, customer.businessname, customer.province, customer.town, customer.postalcode, customer.telephone);
	
	
	printf("\nID: %d\n", header.first_id);
	fseek(cfd, sizeof(HEADER) + (header.first_id-1001) * sizeof(CUSTOMER), SEEK_SET);
	fwrite(&customer, sizeof(CUSTOMER), 1, cfd);
	//printf("HELLO\n");
	fseek(cfd, 0, SEEK_SET);
	header.first_id++;
	fwrite(&header, sizeof(HEADER), 1, cfd);
	
	fclose(cfd);
	
}










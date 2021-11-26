/* 
customers.c
Definition file for functions related to customers.
Date modified: 2021-11-19
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int initcustomers(void){

	//Initialize structs
	HEADER header;
	CUSTOMER customer;

	//Initialize intermediate files for data processing
	char *element;
	char temprecord[MAXREC];
	
	//Open files
	FILE * input = fopen("Customers.txt", "r");
	FILE * cfd = fopen("customersrelativefile.txt", _access("customersrelativefile.txt", 0) < 0 ? "w" : "r+");

	//Assign value to CID and header next CID
	long customerid = header.first_id = 1000; //1000 is the first CID

	fgets(temprecord, MAXREC, input); //Disregard first line in input
	
	/*read the infromation from the text file until end of file
	use string token to extract each element
	use strcpy to copy the extrcted element to the matching feild in the product structure*/
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
		
		//Increment CID and then write customer to relative file
		customerid++;
		fseek(cfd, ((customer.CID - 1000) * sizeof(CUSTOMER)) + sizeof(HEADER), SEEK_SET);
		fwrite(&customer, sizeof(CUSTOMER), 1, cfd);
	}
	
	//Update next CID in header
	header.first_id = customerid;
	fseek(cfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, cfd);
	
	//Close files
	fclose(input);
	fclose(cfd);
	
	return 0;
	
}

int readcustomers(void){
	
	//Init structs
	CUSTOMER customer;
	HEADER header;
	
	//Open relative file
	FILE * cfd = fopen("customersrelativefile.txt", "r+");
	
	//Find next CID from header
	fseek(cfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, cfd);
	
	//Print out next CID
	printf("Next CID: %ld\n", header.first_id);
	
	//Go through loop moving seeking and reading each element and then printing to stdout
	for(int i = 0;i<header.first_id-1000; i++){
		fseek(cfd, i*sizeof(CUSTOMER) + sizeof(HEADER), SEEK_SET);
		fread(&customer, sizeof(CUSTOMER), 1, cfd);
		printf("%ld, %s, %s, %s, %s, %s, %s, %s\n", 
		customer.CID, customer.name, customer.businessname, customer.streetaddress, 
		customer.town, customer.province, customer.postalcode, customer.telephone);
	}
	
	//Close files
	fclose(cfd);
	
	return 0;
}


int addnewcustomers(void)
{
	//Init structs
	CUSTOMER customer;
	HEADER header;
	
	//Open relative file
	FILE * cfd = fopen("customersrelativefile.txt", "r+");
	
	
	/* Access header record to get first available customer id */
	fseek(cfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, cfd);
	
	//Flush input to allow use of fgets after scanf
	fflush(stdin);
	
	//Prompt for elements of customer. TRUNCATE removes the \n characters
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
	
	//Assign CID value from header.first_id
	customer.CID = header.first_id;
	
	//Write customer to relative file
	fseek(cfd, sizeof(HEADER) + (header.first_id-1000) * sizeof(CUSTOMER), SEEK_SET);
	fwrite(&customer, sizeof(CUSTOMER), 1, cfd);
	
	//Increment next CID in header by one
	header.first_id++;
	
	//Updated header
	fseek(cfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, cfd);
	
	//Close relative file
	fclose(cfd);
	
	return 0;
	
}

int lookupcustomer(void){
	
	char tempstring[MAXREC];
	long customerid;
	fflush(stdin); 
	printf("Enter the customer ID you want to look up\n");
	fgets(tempstring, MAXLEN, stdin);
	customerid = atoi(tempstring);

	//Init structs
	CUSTOMER customer;
	HEADER header;
	// open supplier relatve file
	FILE * cfd = fopen("customerrelativefile.txt", "r+");
		fseek(cfd, (customerid-1001)*sizeof(CUSTOMER) + sizeof(HEADER), SEEK_SET);
		fread(&customer, sizeof(CUSTOMER), 1, cfd);
		printf("%ld, %s, %s, %s, %s, %s, %s, %s\n", 
		customer.CID, customer.name, customer.businessname, customer.streetaddress, 
		customer.town, customer.province, customer.postalcode, customer.telephone);	

	//close relative file
	fclose(cfd);
	
	return 0;
}


/* 
suppliers.c
Definition file for functions related to suppliers.
Date modified: 2021-11-19
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"


int initsuppliers(void){

	//Initializing structs
	HEADER header;
	SUPPLIER supplier;
	
	//Init intermediate files for data processing
	
	char *element; //temperory variable to store each element
	char temprecord[MAXREC]; //array to read in 
	//Open files
	FILE * input = fopen("Suppliers v2.txt", "r");
	FILE * sfd = fopen("suppliersrelativefile.txt", _access("suppliersrelativefile.txt", 0) < 0 ? "w" : "r+");
	
	
	long supplierid = header.first_id = 1000; //1001 is the first supplier id
	
	fgets(temprecord, MAXREC, input); //Disregard first line
	//read the information from the text file until end of file
	//use string token to extract each element
	//use strcpy to copy th extrcted element to the matching feild in the supplier structure
	while (fgets(temprecord, MAXREC, input)){ 
	
		TRUNCATE(temprecord);
		supplier.SID = supplierid;
		
		element = strtok(temprecord, "\t\"");
		strcpy(supplier.manufacturer, element);

		element = strtok(NULL, "\t\"");	
		strcpy(supplier.contact, element);

		element = strtok(NULL, "\t\"");
		strcpy(supplier.company, element);
		
		element = strtok(NULL, "\t\"");	
		strcpy(supplier.address, element);
		
		element = strtok(NULL, "\t\"");	
		strcpy(supplier.telephone, element);
		
		element = strtok(NULL, "\t\"");
		strcpy(supplier.email, element);

		
		supplierid++; //increment supplier id
		fseek(sfd, ((supplier.SID - 1000) * sizeof(SUPPLIER)) + sizeof(HEADER), SEEK_SET);//move to the next record
		fwrite(&supplier, sizeof(SUPPLIER), 1, sfd);//write the information to the reletive file
	}
	header.first_id = supplierid; //update first available id in the header 
	fseek(sfd, 0, SEEK_SET);//move to the header 
	fwrite(&header, sizeof(HEADER), 1, sfd);//write the updated id to the header
	
	//close both files
	fclose(input);
	fclose(sfd);
	
	return 0;
	
}


int readsuppliers(void){
	//Init structs
	SUPPLIER supplier;
	HEADER header;
	// open product relatve file
	FILE * sfd = fopen("suppliersrelativefile.txt", "r+");
	//access first available id
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	printf("Next SID: %ld\n", header.first_id);
	//Go through loop moving seeking and reading each element and then printing to stdout
	for(int i = 0;i<header.first_id-1000; i++){
		fseek(sfd, i*sizeof(SUPPLIER) + sizeof(HEADER), SEEK_SET);
		fread(&supplier, sizeof(SUPPLIER), 1, sfd);
		printf("%ld, %s, %s, %s, %s, %s, %s\n", supplier.SID, supplier.manufacturer, 
		supplier.contact, supplier.company, supplier.address, supplier.telephone, supplier.email);
		
	}
	//close relative file
	fclose(sfd);
	
	return 0;
}




int addnewsuppliers(void)
{
	//Initializing structs
	SUPPLIER supplier;
	HEADER header;
	
	//open supplier reletive file for reading
	FILE * sfd = fopen("suppliersrelativefile.txt", "r+");
	
	
	/* Access header record to get first available customer id */
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	
	
	fflush(stdin);//Flush input to allow use of fgets after scanf
	
	//prompt user to enter customer detail
	//truncate each element to add null at the end of each element
	
	printf("Enter Manufacturer\n");
	fgets(supplier.manufacturer, MAXLEN, stdin);
	TRUNCATE(supplier.manufacturer);
	
	printf("Enter Supplier Contact\n");
	fgets(supplier.contact, MAXLEN, stdin);
	TRUNCATE(supplier.contact);
	
	printf("Enter Company Name\n");
	fgets(supplier.company, MAXLEN, stdin);
	TRUNCATE(supplier.company);
	
	printf("Enter Supplier Address\n");
	fgets(supplier.address, MAXLEN, stdin);
	TRUNCATE(supplier.address);
	
	printf("Enter Telephone Number\n");
	fgets(supplier.telephone, MAXLEN, stdin);
	TRUNCATE(supplier.telephone);
	
	printf("Enter Supplier Email\n");
	fgets(supplier.email, MAXLEN, stdin);
	TRUNCATE(supplier.email);

	fseek(sfd, sizeof(HEADER) + (header.first_id-1000) * sizeof(SUPPLIER), SEEK_SET);
	supplier.SID = header.first_id;
	fwrite(&supplier, sizeof(SUPPLIER), 1, sfd);
	
	header.first_id++;//increament first available id
	
	fseek(sfd, 0, SEEK_SET);//move to the header
	fwrite(&header, sizeof(HEADER), 1, sfd);//write the updated first available id to the header
	
	//close file
	fclose(sfd);
	
	return 0;
}
int lookupsupplier(void){
	
	char tempstring[MAXREC];
	long supplierid = 0;
	fflush(stdin); 
	printf("Enter the Supplier ID you want to look up\n");
	fgets(tempstring, MAXLEN, stdin);
	supplierid = atoi(tempstring);
	
	//Init structs
	SUPPLIER supplier;
	HEADER header;
	// open supplier relatve file
	FILE * sfd = fopen("supplierrelativefile.txt", "r+");
		fseek(sfd, (supplierid-1000)*sizeof(SUPPLIER) + sizeof(HEADER), SEEK_SET);
		fread(&supplier, sizeof(SUPPLIER), 1, sfd);
		printf("%ld, %s, %s, %s, %s, %s, %s\n", supplier.SID, supplier.manufacturer, 
		supplier.contact, supplier.company, supplier.address, supplier.telephone, supplier.email);	

	//close relative file
	fclose(sfd);
	
	return 0;
}






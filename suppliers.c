#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"


int initsuppliers(void){

	//Init structs
	HEADER header;
	SUPPLIER supplier;
	
	char *element;
	char temprecord[MAXREC];
	FILE * input = fopen("Suppliers v2.txt", "r");
	FILE * sfd = fopen("suppliersrelativefile.txt", _access("suppliersrelativefile.txt", 0) < 0 ? "w" : "r+");
	
	
	long supplierid = header.first_id = 1000; //1001 is the first PID
	
	fgets(temprecord, MAXREC, input); //Disregard first line
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

		
		supplierid++;
		fseek(sfd, ((supplier.SID - 1000) * sizeof(SUPPLIER)) + sizeof(HEADER), SEEK_SET);
		fwrite(&supplier, sizeof(SUPPLIER), 1, sfd);
	}
	header.first_id = supplierid;
	fseek(sfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, sfd);
	
	fclose(input);
	fclose(sfd);
	
	return 0;
	
}


int readsuppliers(void){
	//Init structs
	SUPPLIER supplier;
	HEADER header;
	FILE * sfd = fopen("suppliersrelativefile.txt", "r+");
	
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	printf("Next SID: %ld\n", header.first_id);
	for(int i = 0;i<header.first_id-1000; i++){
		fseek(sfd, i*sizeof(SUPPLIER) + sizeof(HEADER), SEEK_SET);
		fread(&supplier, sizeof(SUPPLIER), 1, sfd);
		printf("%ld, %s, %s, %s, %s, %s, %s\n", supplier.SID, supplier.manufacturer, 
		supplier.contact, supplier.company, supplier.address, supplier.telephone, supplier.email);
		
	}
	fclose(sfd);
}




int addnewsuppliers(void)
{
	//Init structs
	SUPPLIER supplier;
	HEADER header;
	
	
	FILE * sfd = fopen("suppliersrelativefile.txt", "r+");
	
	
	/* Access header record to get first available customer id */
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	
	
	fflush(stdin);
	
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
	
	header.first_id++;
	
	fseek(sfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, sfd);
	
	fclose(sfd);
	
}


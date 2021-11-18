#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"


int initsuppliers(void){

	HEADER header;
	
	SUPPLIER supplier;
	
	char *element;
	char temprecord[MAXREC];
	FILE * input = fopen("Suppliers v2.txt", "r");
	FILE * output = fopen("suppliersrelativefile.txt", _access("suppliersrelativefile.txt", 0) < 0 ? "w" : "r+");
	
	
	long suppid = header.first_id = 1001; //1001 is the first PID
	
	fgets(temprecord, MAXREC, input); //Disregard first line
	while (fgets(temprecord, MAXREC, input)){
		TRUNCATE(temprecord);
		supplier.SID = supplierid;
		
		element = strtok(temprecord, "\t\"$");
		printf("%s\n", element);
		strcpy(supplier.manufacturer, element);

		element = strtok(NULL, "\t\"$");	
		strcpy(supplier.contact, element);

		element = strtok(NULL, "\t\"$");
		strcpy(supplier.company, element);
		
		element = strtok(NULL, "\t\"$");	
		strcpy(supplier.address, element);
		
		element = strtok(NULL, "\t\"$");	
		strcpy(supplier.telephone, element);

		
		supplierid++;
		fseek(output, ((supplier.PID - 1001) * sizeof(SUPPLIER)) + sizeof(HEADER), SEEK_SET);
		fwrite(&supplier, sizeof(SUPPLIER), 1, output);
	}
	header.first_id = supplierid;
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, output);
	fclose(input);
	fclose(output);
	
	return 0;
	
}


int readsuppliers(void){
	SUPPLIER supplier;
	HEADER header;
	FILE * output = fopen("suppliersrelativefile.txt", "r+");
	
	fseek(output, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, output);
	printf("Header: %ld\n", header.first_id);
	for(int i = 0;i<header.first_id-1001; i++){
		fseek(output, i*sizeof(SUPPLIER) + sizeof(HEADER), SEEK_SET);
		fread(&supplier, sizeof(SUPPLIER), 1, output);
		//printf("%s\n", supplier.name);
		
		printf("Supplier: %s, %ld, %s, %s, %s, %s\n", supplier.manufacturer, product.PID, 
		supplier.contact, supplier.company, supplier.address, supplier.telephone);
		
	}
}




int addnewsuppliers(void)
{
	/* Add a customer to the customerfile */
	SUPPLIER supplier;
	HEADER header;
	
	
	FILE * sfd = fopen("supplierrelativefile.txt", "r+");
	
	
	/* Access header record to get first available customer id */
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	
	
	fflush(stdin);
	
	printf("Enter Name\n");
	fgets(supplier.name, MAXLEN, stdin);
	TRUNCATE(supplier.name);
	
	printf("Enter Product Classification\n");
	fgets(supplier.classification, MAXLEN, stdin);
	TRUNCATE(supplier.classification);
	
	printf("Enter Contact Information\n");
	fgets(supplier.manufacturer, MAXLEN, stdin);
	TRUNCATE(supplier.manufacturer);
	
	printf("Enter the name of the Company\n");
	fgets(supplier.unitcost, MAXLEN, stdin);
	TRUNCATE(supplier.unitcost);
	
	printf("Enter the Address\n");
	fgets(supplier.address, MAXLEN, stdin);
	TRUNCATE(supplier.address);
	
	printf("Enter Telephone Number\n");
	fgets(supplier.telephone, MAXLEN, stdin);
	TRUNCATE(supplier.telephone);


	fseek(pfd, sizeof(HEADER) + (header.first_id-1001) * sizeof(SUPPLIER), SEEK_SET);
	supplier.SID = header.first_id;
	fwrite(&supplier, sizeof(SUPPLIER), 1, sfd);
	
	header.first_id++;
	
	fseek(sfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, sfd);
	
	fclose(sfd);
	
}


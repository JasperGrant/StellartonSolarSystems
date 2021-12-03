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

/*The initsuppliers function intializes a reletive file 
if it doee not alredy exists and if the 
file already exists then it opens to write to it and 
converts the text to a relative file*/

int initsuppliers(void){

	//Initializing structs
	HEADER header;
	SUPPLIER supplier;
	
	//Init intermediate files for data processing
	
	char *element; //temperory variable to store each element
	char temprecord[MAXREC]; //array to read in 
	//Open files
	FILE * input = fopen("Suppliers v2.txt", "r");
	FILE * sfd = fopen("suppliersrelativefile.dat", _access("suppliersrelativefile.dat", 0) < 0 ? "w" : "r+");
	
	
	long supplierid = header.first_id = SUPPLIERFIRSTID; //1001 is the first supplier id
	
	fgets(temprecord, MAXREC, input); //Disregard first line
	//read the information from the text file until end of file
	//use string token to extract each element
	//use strcpy to copy th extrcted element to the matching feild in the supplier structure
	while (fgets(temprecord, MAXREC, input)){ 
	
		TRUNCATE(temprecord);
		supplier.SID = supplierid;
		supplier.status = ACTIVE;
		
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
		fseek(sfd, ((supplier.SID - SUPPLIERFIRSTID) * sizeof(SUPPLIER)) + sizeof(HEADER), SEEK_SET);//move to the next record
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

/*The function readsuppliers opens the suppliers 
relative file and displays the supplier store*/
int readsuppliers(void){
	//Init structs
	SUPPLIER supplier;
	HEADER header;
	// open product relatve file
	FILE * sfd = fopen("suppliersrelativefile.dat", "r+");
	//access first available id
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	printf("Next SID: %ld\n", header.first_id);
	//Go through loop moving seeking and reading each element and then printing to stdout
	for(int i = 0;i<header.first_id-SUPPLIERFIRSTID; i++){
		fread(&supplier, sizeof(SUPPLIER), 1, sfd);
		//Check if record is deleted
		if(supplier.status == ACTIVE){
			printf("Supplier ID: %ld\nManufacturer: %s\nContact: %s\nCompany: %s\nAddress: %s\nTelephone: %s\nEmail: %s\n\n\n", 
			supplier.SID, supplier.manufacturer, supplier.contact, supplier.company, supplier.address, supplier.telephone, supplier.email);
		}
	}
	//close relative file
	fclose(sfd);
	
	return 0;
}

/*The fucntion addnewsuppliers reads the supplier
feilds from the user into the supplier structure
and writes it to the supplier relative file*/

int addnewsuppliers(void)
{
	//Initializing structs
	SUPPLIER supplier;
	HEADER header;
	
	//open supplier reletive file for reading
	FILE * sfd = fopen("suppliersrelativefile.dat", "r+");
	
	
	/* Access header record to get first available product id */
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	
	
	fflush(stdin);//Flush input to allow use of fgets after scanf
	
	//prompt user to enter product detail
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
	
	//Set status to active
	supplier.status = ACTIVE;

	//Assign SID value from header.first_id
	supplier.SID = header.first_id;

	fseek(sfd, sizeof(HEADER) + (header.first_id-SUPPLIERFIRSTID) * sizeof(SUPPLIER), SEEK_SET);
	fwrite(&supplier, sizeof(SUPPLIER), 1, sfd);
	
	header.first_id++;//increament first available id
	
	fseek(sfd, 0, SEEK_SET);//move to the header
	fwrite(&header, sizeof(HEADER), 1, sfd);//write the updated first available id to the header
	
	//close file
	fclose(sfd);
	
	return 0;
}

/*Reads the supplier id to be deleted 
from the user and passes it to
the delete function*/

int userdeletesuppliers(void){
	//Prompt user for supplier ID
	printf("Enter a SID: ");
	fflush(stdin);
	int input;
	scanf("%d", &input);
	
	//run delete function
	deletesuppliers(input);
}

/*The function delete suppliers uses the supplier id to
acces the supplier record to be deleted and changes the 
status to deleted hence logically deleting the record*/
int deletesuppliers(int input){
	//Input variable
	char tempstring[MAXREC];
	
	//Init structs
	SUPPLIER supplier;
	HEADER header;
	
	//Open relative file
	FILE * sfd = fopen("suppliersrelativefile.dat", "r+");
	
	//Read from header to find first availible ID
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	if((input<SUPPLIERFIRSTID) || (input>=header.first_id))
	{
		printf("Invalid ID\n");
		return -1;
	}
	
	//Read supplier values from file
	fseek(sfd, (input-SUPPLIERFIRSTID)*sizeof(SUPPLIER) + sizeof(HEADER), SEEK_SET);
	fread(&supplier, sizeof(SUPPLIER), 1, sfd);
	
	//Change supplier status to DELETED and then put record into file.
	fseek(sfd, (input-SUPPLIERFIRSTID)*sizeof(SUPPLIER) + sizeof(HEADER), SEEK_SET);
	supplier.status = DELETED;
	fwrite(&supplier, sizeof(SUPPLIER), 1, sfd);
	
	//Close relative file
	fclose(sfd);
}

/*The function changesuppliers prompts the user for a supplier id 
and and then asks which feilds are to be changed and overwrites those 
feilds*/

int changesuppliers(void){
	
	//open the supplier reletive file to read
	FILE * sfd = fopen("suppliersrelativefile.dat", "r+");
	
	char *tempstring;
	long supplierid;
	fflush(stdin); 
	
	printf("Enter the Supplier ID you want to change:\n");
	fgets(tempstring, MAXLEN, stdin);
	supplierid = atoi(tempstring);
	fflush(stdin);
	
	//intialzing the structs
	SUPPLIER supplier;
	HEADER header;
	
	//Read from header to find first availible ID
	fseek(sfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, sfd);
	if((supplierid<SUPPLIERFIRSTID) || (supplierid>=header.first_id))
	{
		printf("Invalid ID\n");
		return -1;
	}
	
	
	int option;
	int op = 1;
	//while loop used to chnage multiple feilds without jumping back to main
	while(op){
	printf("Select the feild you would like to change\n1.Manufacturer\n2.Contact\n3.Company Name\n4.Address\n5.Telephone\n6.Email\n");
	scanf("%d", &option);

	
	
	fseek(sfd, (supplierid-SUPPLIERFIRSTID)*sizeof(SUPPLIER) + sizeof(HEADER), SEEK_SET);
	fread(&supplier, sizeof(SUPPLIER), 1, sfd);
	fflush(stdin);
	//use the option entered by the user to change a particular feild
	switch(option){
		case 1:
			printf("Enter Manufacturer\n");
            fgets(supplier.manufacturer, MAXLEN, stdin);
            TRUNCATE(supplier.manufacturer);
	        break;
				
		case 2:
			printf("Enter Supplier Contact\n");
			fgets(supplier.contact, MAXLEN, stdin);
			TRUNCATE(supplier.contact);
			break;
			
		case 3:
			printf("Enter Company Name\n");
			fgets(supplier.company, MAXLEN, stdin);
			TRUNCATE(supplier.company);
			break;
			
		case 4:
			printf("Enter Supplier Address\n");
			fgets(supplier.address, MAXLEN, stdin);
			TRUNCATE(supplier.address);
			break;
				
		case 5:
			printf("Enter Telephone Number\n");
			fgets(supplier.telephone, MAXLEN, stdin);
			TRUNCATE(supplier.telephone);
			break;
				

			case 6:
				printf("Enter Supplier Email\n");
				fgets(supplier.email, MAXLEN, stdin);
				TRUNCATE(supplier.email);
				break;		
	}

		
	
	//access the supplier record using the supplier id again since the fread was done
	fseek(sfd, (supplierid-SUPPLIERFIRSTID)*sizeof(SUPPLIER) + sizeof(HEADER), SEEK_SET);
	//write the product record to the product relative file
	fwrite(&supplier, sizeof(SUPPLIER), 1, sfd);
	
	//print the customer record added to the supplier relative file
	printf("%ld, %s, %s, %s, %s, %s, %s\n", supplier.SID, supplier.manufacturer, 
	supplier.contact, supplier.company, supplier.address, supplier.telephone, supplier.email);	
	
	//the user can change multiple feilds of the same record
	printf("Do you want to change another feild\n");
	printf("Enter 1 for YES   ||   Enter 0 for NO\n");
	
	fflush(stdin);
	
	scanf("%d", &op);//read the option from the user if they want to change another feild
	
	fflush(stdin);//Flush input to not confused program with scanf followed by fgets.
}
	//close relative file
	fclose(sfd);
	
	return 0;
}





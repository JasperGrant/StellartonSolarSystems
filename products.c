/* 
products.c
Definition file for functions related to products.
Date modified: 2021-11-19
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int initproducts(void){

	//Init structs
	HEADER header;
	PRODUCT product;
	
	//Init intermediate files for data processing
	char *element;
	char temprecord[MAXREC];
	//Open files
	FILE * input = fopen("Products v4.txt", "r");
	FILE * pfd = fopen("productsrelativefile.txt", _access("productsrelativefile.txt", 0) < 0 ? "w" : "r+");
	
	
	long productid = header.first_id = 1; //1 is the first PID
	
	fgets(temprecord, MAXREC, input); //Disregard first line in input
	
	/*read the infromation from the text file until end of file
	use string token to extract each element
	use strcpy to copy the extrcted element to the matching feild in the product structure*/
	while (fgets(temprecord, MAXREC, input)){
		TRUNCATE(temprecord);
		product.PID = productid;
		product.status = ACTIVE;
		
		element = strtok(temprecord, "\t\"$");
		strcpy(product.name, element);

		element = strtok(NULL, "\t\"$");	
		strcpy(product.classification, element);

		element = strtok(NULL, "\t\"$");
		strcpy(product.manufacturer, element);
		
		element = strtok(NULL, "\t\"$");	
		product.unitcost = (int)atof(element)*100;

		element = strtok(NULL, "\t\"$");	
		strcpy(product.manufacturercode, element);
		
		element = strtok(NULL, "\t\"$");	
		product.stock = atoi(element);
		
		element = strtok(NULL, "\t\"$");	
		product.reorder = atoi(element);
		
		productid++;//increment product id
		fseek(pfd, ((product.PID - 1) * sizeof(PRODUCT)) + sizeof(HEADER), SEEK_SET);//move to the next record
		fwrite(&product, sizeof(PRODUCT), 1, pfd);//write the elments to the relative file
	}
	header.first_id = productid;
	fseek(pfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, pfd);
	
	fclose(input);
	fclose(pfd);
	
	return 0;
	
}

int readproducts(void){
	
	//Init structs
	PRODUCT product;
	HEADER header;
	// open product relatve file
	FILE * pfd = fopen("productsrelativefile.txt", "r+");
	//access first available id
	fseek(pfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, pfd);
	
	printf("Next PID: %ld\n", header.first_id);
	//Go through loop moving seeking and reading each element and then printing to stdout
	for(int i = 0;i<header.first_id-1; i++){
		fread(&product, sizeof(PRODUCT), 1, pfd);
		//Check if record is deleted
		if(product.status == ACTIVE){
			printf("Product ID: %ld\nProduct Name:%s\nClassification: %s\nManufacturer: %s\nCost: $ %.2f\nManufacturer Code: %s\nStock: %d\nReorder Level: %d\n\n\n", 
			product.PID, product.name, product.classification, product.manufacturer, 
			(float)product.unitcost/100, product.manufacturercode, product.stock, product.reorder);	
		}
	}
	//close relative file
	fclose(pfd);
	
	return 0;
}


int addnewproducts(void)
{
	char tempstring[LONGMAXLEN];
	
	//Init structs
	PRODUCT product;
	HEADER header;
	
	//open product relative file
	FILE * pfd = fopen("productsrelativefile.txt", "r+");
	
	
	//Access header record to get first available product id 
	fseek(pfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, pfd);
	
	
	fflush(stdin); //Flush input to not confused program with scanf followed by fgets.
	
	/*prompt user to enter product detail
	truncate each element to add null at the end of each element*/
	
	printf("Enter Name\n");
	fgets(product.name, MAXLEN, stdin);
	TRUNCATE(product.name);
	
	printf("Enter Classification\n");
	fgets(product.classification, MAXLEN, stdin);
	TRUNCATE(product.classification);
	
	printf("Enter Manufacturer\n");
	fgets(product.manufacturer, MAXLEN, stdin);
	TRUNCATE(product.manufacturer);
	
	
	printf("Enter Unit Cost\n");
	fgets(tempstring, MAXLEN, stdin);
    product.unitcost = atof(tempstring)*100;//converts the string argument string to a floating-point number and is stored in cents
	
	printf("Enter Manufacturer Code\n");
	fgets(product.manufacturercode, MAXLEN, stdin);
	TRUNCATE(product.manufacturercode);
	
	printf("Enter Stock\n");
	fgets(tempstring, MAXLEN, stdin);
	product.stock = atoi(tempstring);//converts the string argument str to an integer
	
	printf("Enter the Reorder Level\n");
	fgets(tempstring, MAXLEN, stdin);
	product.reorder = atoi(tempstring);//converts the string argument str to an integer
	
	//Set status to active
	product.status = ACTIVE;

	//Assign PID value from header.first_id
	product.PID = header.first_id;

	fseek(pfd, sizeof(HEADER) + (header.first_id-1) * sizeof(PRODUCT), SEEK_SET);
	fwrite(&product, sizeof(PRODUCT), 1, pfd);
	
	header.first_id++;//increment first available id
	
	fseek(pfd, 0, SEEK_SET);//move to the header
	fwrite(&header, sizeof(HEADER), 1, pfd);//write the updated first available id to the header
	
	fclose(pfd);//close file
	
	return 0;
	
}

int userdeleteproducts(void){
	//Prompt user for product ID
	printf("Enter a PID: ");
	fflush(stdin);
	int input;
	scanf("%d", &input);
	
	//run delete function
	deleteproducts(input);
}

int deleteproducts(int input){
	//Input variable
	char tempstring[MAXREC];
	
	//Init structs
	PRODUCT product;
	HEADER header;
	
	//Open relative file
	FILE * pfd = fopen("productsrelativefile.txt", "r+");
	
	//Read supplier values from file
	fseek(pfd, (input-1)*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
	fread(&product, sizeof(PRODUCT), 1, pfd);
	
	//Change product status to DELETED and then put record into file.
	fseek(pfd, (input-1)*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
	product.status = DELETED;
	fwrite(&product, sizeof(PRODUCT), 1, pfd);
	
	//Close relative file
	fclose(pfd);
}

int changeproducts(void){
	//open product reletive file to read and write
	FILE * pfd = fopen("productsrelativefile.txt", "r+");
	
	char *tempstring;
	long productid;
	fflush(stdin); 
	
	printf("Enter the Product ID you want to change:\n");
	fgets(tempstring, MAXLEN, stdin);
	productid = atoi(tempstring);
	fflush(stdin);
	
	//Init structs
	PRODUCT product;
	HEADER header;
	// open product relatve file
	
	int option;
	printf("Select the feild you would like to change\n1.Name\n2.Classification\n3.Manufacturer\n4.Unitcost\n5.Manufacturer Code\n6.Stock\n7.Reorder Level\n");
	scanf("%d", &option);
	
	fseek(pfd, (productid-1)*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
	fread(&product, sizeof(PRODUCT), 1, pfd);
	fflush(stdin);
	
		switch(option){
		case 1:
			printf("Enter new Name\n");
	        fgets(product.name, MAXLEN, stdin);
	        TRUNCATE(product.name);
	        break;
			
		case 2:
			printf("Enter new Classification\n");
			fgets(product.classification, MAXLEN, stdin);
			TRUNCATE(product.classification);
			break;
		
		case 3:
			printf("Enter new Manufacturer\n");
			fgets(product.manufacturer, MAXLEN, stdin);
			TRUNCATE(product.manufacturer);
			break;
		
		case 4:
			printf("Enter new Unit Cost\n");
	        fgets(tempstring, MAXLEN, stdin);
    		product.unitcost = atof(tempstring)*100;
			break;
			
		case 5:
			printf("Enter new Manufacturer Code\n");
			fgets(product.manufacturercode, MAXLEN, stdin);
			TRUNCATE(product.manufacturercode);
			break;
			
		case 6:
			printf("Enter new Stock\n");
			fgets(tempstring, MAXLEN, stdin);
			product.stock = atoi(tempstring);
			break;
		
		case 7:
			printf("Enter new Reorder Level\n");
			fgets(tempstring, MAXLEN, stdin);
			product.reorder = atoi(tempstring);
			break;
			
					
	}
	fseek(pfd, (productid-1)*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
	fwrite(&product, sizeof(PRODUCT), 1, pfd);
	
	printf("%ld, %s, %s, %s, %.2f, %s, %d, %d\n", 
	product.PID, product.name, product.classification, product.manufacturer, 
	(float)product.unitcost/100, product.manufacturercode, product.stock, product.reorder);	

	//close relative file
	fclose(pfd);
	
	return 0;
}





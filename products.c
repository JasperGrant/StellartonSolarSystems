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

int initproducts(void){ //TODO

	//Init structs
	HEADER header;
	PRODUCT product;
	
	//Init intermediate files for data processing
	char *element;
	char temprecord[MAXREC];
	//Open files
	FILE * input = fopen("Product v2.txt", "r");
	FILE * pfd = fopen("productsrelativefile.txt", _access("productsrelativefile.txt", 0) < 0 ? "w" : "r+");
	
	
	long productid = header.first_id = 1; //1 is the first PID
	
	fgets(temprecord, MAXREC, input); //Disregard first line in input
	
	/*read the infromation from the text file until end of file
	use string token to extract each element
	use strcpy to copy the extrcted element to the matching feild in the product structure*/
	while (fgets(temprecord, MAXREC, input)){
		TRUNCATE(temprecord);
		product.PID = productid;
		
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
		fseek(pfd, i*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
		fread(&product, sizeof(PRODUCT), 1, pfd);
		printf("%ld, %s, %s, %s, %.2f, %s, %d\n", 
		product.PID, product.name, product.classification, product.manufacturer, 
		(float)product.unitcost/100, product.manufacturercode, product.stock);	
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
	
	/*prompt user to enter customer detail
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

	fseek(pfd, sizeof(HEADER) + (header.first_id-1) * sizeof(PRODUCT), SEEK_SET);
	product.PID = header.first_id;
	fwrite(&product, sizeof(PRODUCT), 1, pfd);
	
	header.first_id++;//increament first available id
	
	fseek(pfd, 0, SEEK_SET);//move to the header
	fwrite(&header, sizeof(HEADER), 1, pfd);//write the updated first available id to the header
	
	fclose(pfd);//close file
	
	return 0;
	
}

int lookupproduct(void){
	
	char tempstring[MAXREC];
	int productid1 = 0;
	fflush(stdin); 
	printf("Enter the ID of the product you want to look up\n");
	fgets(tempstring, MAXLEN, stdin);
	productid1 = atoi(tempstring);
	
	//Init structs
	PRODUCT product;
	HEADER header;
	// open product relatve file
	FILE * pfd = fopen("productsrelativefile.txt", "r+");
		fseek(pfd, (productid1-1)*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
		fread(&product, sizeof(PRODUCT), 1, pfd);
		printf("%ld, %s, %s, %s, %.2f, %s, %d\n", 
		product.PID, product.name, product.classification, product.manufacturer, 
		(float)product.unitcost/100, product.manufacturercode, product.stock);	

	//close relative file
	fclose(pfd);
	
	return 0;
}




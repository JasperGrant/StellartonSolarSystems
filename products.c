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
	
	char *element;
	char temprecord[MAXREC];
	FILE * input = fopen("Product v2.txt", "r");
	FILE * pfd = fopen("productsrelativefile.txt", _access("productsrelativefile.txt", 0) < 0 ? "w" : "r+");
	
	
	long productid = header.first_id = 1; //1 is the first PID
	
	fgets(temprecord, MAXREC, input); //Disregard first line in input
	
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
		
		productid++;
		fseek(pfd, ((product.PID - 1) * sizeof(PRODUCT)) + sizeof(HEADER), SEEK_SET);
		fwrite(&product, sizeof(PRODUCT), 1, pfd);
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
	FILE * pfd = fopen("productsrelativefile.txt", "r+");
	
	fseek(pfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, pfd);
	printf("Next PID: %ld\n", header.first_id);
	for(int i = 0;i<header.first_id-1; i++){
		fseek(pfd, i*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
		fread(&product, sizeof(PRODUCT), 1, pfd);
		//printf("%s\n", product.name);
		
		printf("%ld, %s, %s, %s, %.2f, %s, %d\n", 
		product.PID, product.name, product.classification, product.manufacturer, 
		(float)product.unitcost/100, product.manufacturercode, product.stock);
		
	}
	fclose(pfd);
}


int addnewproducts(void)
{
	char tempstring[LONGMAXLEN];
	
	//Init structs
	PRODUCT product;
	HEADER header;
	
	
	FILE * pfd = fopen("productsrelativefile.txt", "r+");
	
	
	//Access header record to get first available product id 
	fseek(pfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, pfd);
	
	
	fflush(stdin); //Flush input to not confused program with scanf followed by fgets.
	
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
    product.unitcost = atof(tempstring)*100;
	
	printf("Enter Manufacturer Code\n");
	fgets(product.manufacturercode, MAXLEN, stdin);
	TRUNCATE(product.manufacturercode);
	
	printf("Enter Stock\n");
	fgets(tempstring, MAXLEN, stdin);
	product.stock = atoi(tempstring);
	//fgets(product.stock, MAXLEN, stdin);

	


	fseek(pfd, sizeof(HEADER) + (header.first_id-1) * sizeof(PRODUCT), SEEK_SET);
	product.PID = header.first_id;
	fwrite(&product, sizeof(PRODUCT), 1, pfd);
	
	header.first_id++;
	
	fseek(pfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, pfd);
	
	fclose(pfd);
	
}





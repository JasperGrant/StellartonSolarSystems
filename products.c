#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"

int initproducts(void){ //TODO

	HEADER header;
	
	PRODUCT product;
	
	char *element;
	char temprecord[MAXREC];
	FILE * input = fopen("Product v2.txt", "r");
	FILE * output = fopen("productsrelativefile.txt", _access("productsrelativefile.txt", 0) < 0 ? "w" : "r+");
	
	
	long productid = header.first_id = 1001; //1001 is the first PID
	
	fgets(temprecord, MAXREC, input); //Disregard first line
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
		fseek(output, ((product.PID - 1001) * sizeof(PRODUCT)) + sizeof(HEADER), SEEK_SET);
		fwrite(&product, sizeof(PRODUCT), 1, output);
	}
	header.first_id = productid;
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, output);
	fclose(input);
	fclose(output);
	
	return 0;
	
}
/*
int readproducts(void){ //TODO
	PRODUCT product;
	HEADER header;
	FILE * output = fopen("productsrelativefile.txt", "r+");
	
	fseek(output, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, output);
	printf("Header: %ld\n", header.first_id);
	for(int i = 0;i<header.first_id-1001; i++){
		fseek(output, i*sizeof(PRODUCT) + sizeof(PRODUCT), SEEK_SET);
		fread(&product, sizeof(PRODUCT), 1, output);
		printf("product: %s, %ld, %s, %s, %s, %s, %s, %s\n", 
		product.name, product.CID, product.businessname, product.streetaddress, 
		product.town, product.province, product.postalcode, product.telephone);
	}
}
*/

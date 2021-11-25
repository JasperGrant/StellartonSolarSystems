/* 
sales.c
Definition file for functions related to sales.
Date modified: 2021-11-19
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int initsales(void){
	
	FILE * tfd = fopen("salesrelativefile.txt","w");
	
	//Init structs
	HEADER header;
	header.first_id = 1;
	
	//Write header
	fseek(tfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, tfd);
	
	//Close relative file
	fclose(tfd);
	
	return 0;
}

int readsales(void){
	
	//Init structs
	SALE sale;
	HEADER header;
	
	//Open relative file
	FILE * tfd = fopen("salesrelativefile.txt", "r+");
	
	//Find first_id from header
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	
	//Print next TID
	printf("Next TID: %ld\n", header.first_id);
	
	//Go through loop moving seeking and reading each element and then printing to stdout
	for(int i = 0;i<header.first_id-1; i++){
		fseek(tfd, i*sizeof(SALE) + sizeof(HEADER), SEEK_SET);
		fread(&sale, sizeof(SALE), 1, tfd);
		printf("Sale: %ld, %ld: %s, %ld: %s, %d, $ %.2f\n", sale.TID, sale.CID, sale.name,
		sale.PID, sale.productname, sale.quantity, (float)sale.totalcost/100.0);
	}
	
	//Close relative file
	fclose(tfd);
	
	return 0;
}

int addnewsales(void){
	
	//Init structs
	HEADER header;
	CUSTOMER customer;
	PRODUCT product;
	SALE sale;
	
	//Open relative files
	FILE * cfd = fopen("customersrelativefile.txt", "r");//Open customer relative file
	FILE * pfd = fopen("productsrelativefile.txt", "r+");//Open product relative file
	FILE * tfd = fopen("salesrelativefile.txt", "r+");//Open sale relative file
	
	//Prompt for CID, PID and quantity of order
	printf("Enter Customer ID: ");
	scanf("%d",&sale.CID);
	printf("Enter Product ID: ");
	scanf("%d",&sale.PID);
	printf("Enter quantity of product: ");
	scanf("%d",&sale.quantity);
	
	//Obtain first availible TID
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	sale.TID = header.first_id; 
	
	//Access customer that goes with CID
	fseek(cfd, sizeof(HEADER) + ((sale.CID-1000)*sizeof(CUSTOMER)), SEEK_SET);
	fread(&customer, sizeof(CUSTOMER), 1, cfd);

	//Access product that goes with PID
	fseek(pfd, sizeof(HEADER) + ((sale.PID-1)*sizeof(PRODUCT)), SEEK_SET);
	fread(&product, sizeof(PRODUCT), 1, pfd);
	
	//Check if quantity is more then is in stock. If so inform customer and end function
	if(sale.quantity > product.stock){
		printf("Sale impossible as quantity of item requested is greater then stock.\n");
		return -1;
	}
	
	//Decrement stock with quantity
	product.stock-=sale.quantity;
	
	//Write new quantity to products relative file
	fseek(pfd, sizeof(HEADER) + (sale.PID-1)*sizeof(PRODUCT), SEEK_SET);
	fwrite(&product, sizeof(PRODUCT), 1, pfd);
	
	//Properly assign values needed from product and customer to sale
	strcpy(sale.name, customer.name);
	strcpy(sale.productname, product.name);
	sale.totalcost = product.unitcost * sale.quantity;
	
	//Write sale to relative file
	fseek(tfd, sizeof(HEADER) + (header.first_id-1) * sizeof(SALE), SEEK_SET);
	fwrite(&sale, sizeof(SALE), 1, tfd);
	
	header.first_id++;//Increment availible ID by 1
	
	//Write new first_id to header
	fseek(tfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, tfd);
	
	//Close files
	fclose(cfd);
	fclose(pfd);
	fclose(tfd);
	
	return 0;
}


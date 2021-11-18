#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"

int initsales(void){
	
	FILE * output = fopen("salesrelativefile.txt","w");
	
	HEADER header;
	header.first_id = 1;
	
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, output);
	
	fclose(output);
}

int readsales(void){
	SALE sale;
	HEADER header;
	FILE * output = fopen("salesrelativefile.txt", "r+");
	
	fseek(output, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, output);
	printf("Header: %ld\n", header.first_id);
	for(int i = 0;i<header.first_id-1; i++){
		fseek(output, i*sizeof(SALE) + sizeof(HEADER), SEEK_SET);
		fread(&sale, sizeof(SALE), 1, output);
		printf("Sale: %ld, %ld: %s, %ld: %s, %d, %f\n", sale.TID, sale.CID, sale.companyname,
		sale.PID, sale.productname, sale.quantity, (float)sale.totalcost/100);
	}
}

int addnewsales(void){
	
	HEADER header;
	CUSTOMER customer;
	PRODUCT product;
	SALE sale;
	
	FILE * cfd = fopen("customersrelativefile.txt", "r+");//Open customer relative file
	FILE * pfd = fopen("productsrelativefile.txt", "r+");//Open product relative file
	FILE * tfd = fopen("salesrelativefile.txt", "r+");//Open sale relative file
	
	printf("Enter Customer ID: ");
	scanf("%d",&sale.CID);
	printf("Enter Product ID: ");
	scanf("%d",&sale.PID);
	printf("Enter quantity of product: ");
	scanf("%d",&sale.quantity);
	
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	
	sale.TID = header.first_id; 
	
	fseek(cfd, sizeof(HEADER) + (sale.CID-1)*sizeof(CUSTOMER), SEEK_SET);
	fread(&customer, sizeof(CUSTOMER), 1, cfd);

	fseek(pfd, sizeof(HEADER) + (sale.PID-1000)*sizeof(PRODUCT), SEEK_SET);
	fread(&product, sizeof(PRODUCT), 1, pfd);
	
	printf("quantity: %d", sale.quantity);
	printf("stock: %d", product.stock);
	
	if(sale.quantity > product.stock){
		printf("Sale impossible as quantity of item requested is greater then stock.\n");
		return -1;
	}
	product.stock-=sale.quantity;
	fseek(pfd, sizeof(HEADER) + (sale.PID-999)*sizeof(CUSTOMER), SEEK_SET);
	fwrite(&product, sizeof(PRODUCT), 1, pfd);
	
	strcpy(sale.companyname, customer.businessname);
	strcpy(sale.productname, product.name);
	sale.totalcost = product.unitcost * sale.quantity;
	
	fseek(tfd, sizeof(HEADER) + (header.first_id-1) * sizeof(SALE), SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, tfd);
	
	header.first_id++;
	fseek(tfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, tfd);
	
	fclose(cfd);
	fclose(pfd);
	fclose(tfd);
	
}


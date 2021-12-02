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

extern int globaldate;

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

int initbackorders(void){
	
	FILE * bfd = fopen("backordersrelativefile.txt","w");
	
	//Init structs
	HEADER header;
	header.first_id = 1;
	
	//Write header
	fseek(bfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, bfd);
	
	//Close relative file
	fclose(bfd);
	
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
		fread(&sale, sizeof(SALE), 1, tfd);
		if(sale.status == ACTIVE){
			printf("Sale: %ld, %ld: %s, %ld: %s, %d, $ %.2f\n", sale.TID, sale.CID, sale.name,
			sale.PID, sale.productname, sale.quantity, (float)sale.totalcost/100.0);
		}
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
	FILE * tfd;
	
	//Prompt for CID, PID and quantity of order
	printf("Enter Customer ID: ");
	scanf("%d",&sale.CID);
	printf("Enter Product ID: ");
	scanf("%d",&sale.PID);
	printf("Enter quantity of product: ");
	scanf("%d",&sale.quantity);
		
	//Access customer that goes with CID
	fseek(cfd, sizeof(HEADER) + ((sale.CID-1000)*sizeof(CUSTOMER)), SEEK_SET);
	fread(&customer, sizeof(CUSTOMER), 1, cfd);

	//Access product that goes with PID
	fseek(pfd, sizeof(HEADER) + ((sale.PID-1)*sizeof(PRODUCT)), SEEK_SET);
	fread(&product, sizeof(PRODUCT), 1, pfd);
	
	//Sale quantity and product stock are the same the last of the products has been bought so the product can be deleted.
	if(sale.quantity == product.stock){
		deleteproducts(sale.PID);
		tfd = fopen("salesrelativefile.txt", "r+");//Open sale relative file
		//dailyorder(sale.PID);
	}
	//Check if quantity is more then is in stock. If so inform customer and add the order to the backorder relative file
	else if(sale.quantity > product.stock){
		
		printf("Sale impossible as quantity of item requested is greater then stock. Order has been added to the backorders to be fullfilled when the stock is updated\n");
	    tfd = fopen("backordersrelativefile.txt", "r+");//Open product relative file
	}
	//if sale is possible just open the sales reletive file
	else{
		tfd = fopen("salesrelativefile.txt", "r+");//Open sale relative file
		
		//Decrement stock with quantity
		product.stock-=sale.quantity;
		
		//Write new quantity to products relative file
		fseek(pfd, sizeof(HEADER) + (sale.PID-1)*sizeof(PRODUCT), SEEK_SET);
		fwrite(&product, sizeof(PRODUCT), 1, pfd);
	}

		
	//Obtain first availible TID
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	sale.TID = header.first_id; 
	
	//Set sale status to active
	sale.status = ACTIVE;
		
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
	
	fclose(tfd);
	//Close files
	fclose(cfd);
	fclose(pfd);
	
	return 0;
}

int userdeletesales(void){
	//Prompt user for sales ID
	printf("Enter a TID: ");
	fflush(stdin);
	int input;
	scanf("%d", &input);
	
	//run delete function
	deletesales(input);
}

int deletesales(int input){
	//Input variable
	char tempstring[MAXREC];
	
	//Init structs
	SALE sale;
	HEADER header;
	
	//Open relative file
	FILE * tfd = fopen("salesrelativefile.txt", "r+");
	
	//Read sales values from file
	fseek(tfd, (input-1)*sizeof(SALE) + sizeof(HEADER), SEEK_SET);
	fread(&sale, sizeof(SALE), 1, tfd);
	
	//Change sale status to DELETED and then put record into file.
	fseek(tfd, (input-1)*sizeof(SALE) + sizeof(HEADER), SEEK_SET);
	sale.status = DELETED;
	fwrite(&sale, sizeof(SALE), 1, tfd);
	
	//Close relative file
	fclose(tfd);
}

int readbackorders(void){
	
	//Init structs
	SALE sale;
	HEADER header;
	
	//Open relative file
	FILE * tfd = fopen("backordersrelativefile.txt", "r+");
	
	//Find first_id from header
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	
	//Print next TID
	printf("Next TID: %ld\n", header.first_id);
	
	//Go through loop moving seeking and reading each element and then printing to stdout
	for(int i = 0;i<header.first_id-1; i++){
		fread(&sale, sizeof(SALE), 1, tfd);
		if(sale.status == ACTIVE){
			printf("Backorder: %ld, %ld: %s, %ld: %s, %d, $ %.2f\n", sale.TID, sale.CID, sale.name,
			sale.PID, sale.productname, sale.quantity, (float)sale.totalcost/100.0);
		}
	}
	
	//Close relative file
	fclose(tfd);
	
	return 0;
}
/*
int dailyorder(int input){
	//intialize all structures needed
	PRODUCT product;
	SUPPLIER supplier;
	DAILYORDER dailyorder;
	HEADER header;//to store the daily order id 
	
	int check;
	long supplierid;
	//open product and supplier reletive files to read
	FILE * pfd = fopen("productsrelativefile.txt", "r");
	FILE * sfd = fopen("suppliersrelativefile.txt", "r");
	//open daily order reletive file to write to jasper has to do the changing file names according to the date
	FILE * dfd = fopen("dailyordersrelativefile.txt", "r+");
	
	//fseek to the relvant product id 
	fseek(pfd, (input-1)*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
	//fread the product stuff into structure
	fread(&product, sizeof(PRODUCT), 1, pfd);
	
	//find supplier using the manufacturer
	while(check!= 0){
		fread(&supplier, sizeof(SUPPLIER), 1, sfd);
		check = strcmp(product.manufacturer, supplier.manufacturer);
	}
	
	//copy supplier info into dailyorder 
	strcpy(dailyorder.productid, product.PID);
	strcpy(dailyorder.classification, product.classification);
	strcpy(dailyorder.manufacturercode, product.manufacturercode);
	strcpy(dailyorder.suppliercontact, supplier.contact);
	strcpy(dailyorder.suppliertelephone, supplier.telephone);
	strcpy(dailyorder.suppliertelephone, supplier.telephone);
	strcpy(dailyorder.suppliertelephone, supplier.telephone);
	
	//Write sale to relative file
	fseek(dfd, sizeof(HEADER) + (header.first_id-1) * sizeof(SALE), SEEK_SET);
	fwrite(&dailyorder, sizeof(DAILYORDER), 1, dfd);
		
	header.first_id++;//Increment availible ID by 1
		
	//Write new first_id to header
	fseek(dfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, dfd);
	
	fclose(dfd);
	return 0;
	
}

*/

int initdailyorders(void){
	
	FILE * dfd = fopen("dailyordersrelativefile.txt","w");
	
	//Init structs
	HEADER header;
	header.first_id = 1;
	
	//Write header
	fseek(dfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, dfd);
	
	//Close relative file
	fclose(dfd);
	
	return 0;
}

int paymentdue(void){
	//Initialize filename string for 30 days ago ORDERS file
	char pastfilename[32];
	//Initialize command to copy and rename ORDERS file
	char command[100];
	//Fill filename with string
	sprintf(pastfilename, "ORDERS%d", globaldate-30);
	if(access(pastfilename,F_OK) == 0){
		char filename[32];
		sprintf(filename, "PAY_DUE%d", globaldate);
		//Used to format shell command: copy ORDERS(date-30) PAY_DUE(date)
		sprintf(command, "copy %s %s", pastfilename, filename);
		//execute copy command
		system(command);
	}
	else{
		return 0;
	}

}

int fillbackorders(void){
	//Prompt user for products which has a filled order and quantity
	int inputID, inputQuantity;
	printf("Enter PID of product with filled order:\n");
	scanf("%d", &inputID);
	printf("Enter Quantity of items filled:\n");
	scanf("%d", &inputQuantity);
	//Init structs
	SALE sale;
	HEADER header;
	PRODUCT product;
	
	//Open relative files
	FILE * tfd = fopen("backordersrelativefile.txt", "r+");
	FILE * pfd = fopen("productsrelativefile.txt", "r+");
	char filename[MAXLEN];
	sprintf(filename, "ORDERSFILLED%d", globaldate);
	FILE * ordersfilled = fopen(filename, "a");
	
	//Find first_id from header
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	
	//Go through loop moving seeking and reading each element and then if that element mataches the PID and is not deleted printing to daily ORDERSFILLED
	for(int i = 0;i<header.first_id-1; i++){
		fread(&sale, sizeof(SALE), 1, tfd);
		if((sale.status == ACTIVE) && (sale.PID == inputID)){
			fprintf(ordersfilled, "Sale: %ld, %ld: %s, %ld: %s, %d, $ %.2f\n", sale.TID, sale.CID, sale.name,
			sale.PID, sale.productname, sale.quantity, (float)sale.totalcost/100.0);
			deletebackorders(inputID);
			//For every applicable item remove from inputted quantiy the quantity of that order
			inputQuantity-=sale.quantity;
			if(inputQuantity<0){
				printf("Error too little quantity to fill all orders.\n");
				break;
			}
		}
	}
	
	//inputQuantity now reflects what should be added to the system after orders are filled.
	//Read product
	fseek(pfd, sizeof(HEADER) + (inputID-1)*sizeof(PRODUCT), SEEK_SET);
	fread(&product, sizeof(PRODUCT), 1, pfd);
	//Update stock
	product.stock+= inputQuantity;
	//Write product
	fseek(pfd, sizeof(HEADER) + (inputID-1)*sizeof(PRODUCT), SEEK_SET);
	fwrite(&product, sizeof(PRODUCT), 1, pfd);
	
	//Close relative file
	fclose(tfd);
	fclose(pfd);
	
	return 0;
	
}

int deletebackorders(int input){
	//Input variable
	char tempstring[MAXREC];
	
	//Init structs
	SALE sale;
	HEADER header;
	
	//Open relative file
	FILE * tfd = fopen("backordersrelativefile.txt", "r+");
	
	//Read backorders values from file
	fseek(tfd, (input-1)*sizeof(SALE) + sizeof(HEADER), SEEK_SET);
	fread(&sale, sizeof(SALE), 1, tfd);
	
	//Change backorder status to DELETED and then put record into file.
	fseek(tfd, (input-1)*sizeof(SALE) + sizeof(HEADER), SEEK_SET);
	sale.status = DELETED;
	fwrite(&sale, sizeof(SALE), 1, tfd);
	
	//Close relative file
	fclose(tfd);
}





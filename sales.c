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
extern char globaldatestring[DATELEN];

/*The initsales function intializes a reletive file 
by creating it and writes the first available sales id*/
int initsales(void){
	
	FILE * tfd = fopen("salesrelativefile.dat","w");
	
	//Init structs
	HEADER header;
	header.first_id = SALESFIRSTID;
	
	//Write header
	fseek(tfd, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, tfd);
	
	//Close relative file
	fclose(tfd);
	
	return 0;
}

/*The initbackorders function intializes a reletive file 
by creating it and writes the first available backorder number*/
int initbackorders(void){
	
	FILE * bfd = fopen("backorders.dat","w");
	
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
/*The function readsales opens the sales 
relative file and displays the sales store*/
int readsales(void){
	
	//Init structs
	SALE sale;
	HEADER header;
	
	//Open relative file
	FILE * tfd = fopen("salesrelativefile.dat", "r+");
	
	//Find first_id from header
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	
	//Print next TID
	printf("Next TID: %ld\n", header.first_id);
	
	//Go through loop moving seeking and reading each element and then printing to stdout
	for(int i = 0;i<header.first_id-SALESFIRSTID; i++){
		fread(&sale, sizeof(SALE), 1, tfd);
		if(sale.status == ACTIVE){
			printf("Sale %ld:\nDate of the sale: %s\nCustomer ID: %ld\nCustomer Name: %s\nProduct ID: %ld\nProduct name: %s\nQuantity of the product purchsed: %d\nTotal cost: $ %.2f\n\n\n", sale.TID, sale.date, sale.CID, sale.name,
			sale.PID, sale.productname, sale.quantity, (float)sale.totalcost/DOLLARSTOCENTS);
		}
	}
	
	//Close relative file
	fclose(tfd);
	
	return 0;
}

/*This function prompts the user for the customer id
and the product id and uses that to sell a product relating 
to the product id to the customer relating to the customer id*/
int addnewsales(void){
	
	//Init structs
	HEADER header;
	CUSTOMER customer;
	PRODUCT product;
	SALE sale;

	
	//Open relative files
	FILE * cfd = fopen("customersrelativefile.dat", "r");//Open customer relative file
	FILE * pfd = fopen("productsrelativefile.dat", "r+");//Open product relative file
	FILE * tfd;
	
	//Prompt for CID, PID and quantity of order
	printf("Enter Customer ID: ");
	scanf("%d",&sale.CID);
	printf("Enter Product ID: ");
	scanf("%d",&sale.PID);
	printf("Enter quantity of product: ");
	scanf("%d",&sale.quantity);
		
	//Access customer that goes with CID
	fseek(cfd, sizeof(HEADER) + ((sale.CID-CUSTOMERFIRSTID)*sizeof(CUSTOMER)), SEEK_SET);
	fread(&customer, sizeof(CUSTOMER), 1, cfd);

	//Access product that goes with PID
	fseek(pfd, sizeof(HEADER) + ((sale.PID-PRODUCTFIRSTID)*sizeof(PRODUCT)), SEEK_SET);
	fread(&product, sizeof(PRODUCT), 1, pfd);
	
	//Sale quantity and product stock are the same the last of the products has been bought so the product can be deleted.
	if(sale.quantity == product.stock){
		deleteproducts(sale.PID);
		tfd = fopen("salesrelativefile.dat", "r+");//Open sale relative file
		dailyorders(sale.PID);
	}
	//Check if product stock falls below reorder level. If so do not delete the file but add the item to be ordered
	else if((product.stock - sale.quantity < product.reorder) && (product.stock - sale.quantity > 0)) {
		tfd = fopen("salesrelativefile.dat", "r+");//Open sale relative file
		dailyorders(sale.PID);
	}
	
	//Check if quantity is more then is in stock. If so inform customer and add the order to the backorder relative file
	else if(sale.quantity > product.stock){
		
		printf("Sale impossible as quantity of item requested is greater then stock. Order has been added to the backorders to be fullfilled when the stock is updated\n");
	    tfd = fopen("backorders.dat", "r+");//Open product relative file
	}
	//if sale is possible just open the sales reletive file
	else{
		tfd = fopen("salesrelativefile.dat", "r+");//Open sale relative file
		
		//Decrement stock with quantity
		product.stock-=sale.quantity;
		
		//Write new quantity to products relative file
		fseek(pfd, sizeof(HEADER) + (sale.PID-SALESFIRSTID)*sizeof(PRODUCT), SEEK_SET);
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
	strcpy(sale.date, globaldatestring);
	sale.totalcost = product.unitcost * sale.quantity;
		
	//Write sale to relative file
	fseek(tfd, sizeof(HEADER) + (header.first_id-SALESFIRSTID) * sizeof(SALE), SEEK_SET);
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

/*Reads the sale id to be deleted 
from the user and passes it to
the delete function*/

int userdeletesales(void){
	//Prompt user for sales ID
	printf("Enter a TID: ");
	fflush(stdin);
	int input;
	scanf("%d", &input);
	
	//run delete function
	deletesales(input);
}

/*The function delete sales uses the sale id to
acces the sale record to be deleted and changes the 
status to deleted hence logically deleting the record*/

int deletesales(int input){
	//Input variable
	char tempstring[MAXREC];
	
	//Init structs
	SALE sale;
	HEADER header;
	
	//Open relative file
	FILE * tfd = fopen("salesrelativefile.dat", "r+");
	
	//Read sales values from file
	fseek(tfd, (input-SALESFIRSTID)*sizeof(SALE) + sizeof(HEADER), SEEK_SET);
	fread(&sale, sizeof(SALE), 1, tfd);
	
	//Change sale status to DELETED and then put record into file.
	fseek(tfd, (input-SALESFIRSTID)*sizeof(SALE) + sizeof(HEADER), SEEK_SET);
	sale.status = DELETED;
	fwrite(&sale, sizeof(SALE), 1, tfd);
	
	//Close relative file
	fclose(tfd);
}

/*The function readbackorders opens the backorder
relative file and displays the backorder store*/

int readbackorders(void){
	
	//Init structs
	SALE sale;
	HEADER header;
	
	//Open relative file
	FILE * tfd = fopen("backorders.dat", "r+");
	
	//Find first_id from header
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	
	//Print next TID
	printf("Next Backorder number: %ld\n", header.first_id);
	
	//Go through loop moving seeking and reading each element and then printing to stdout
	for(int i = 0;i<header.first_id-SALESFIRSTID; i++){
		fread(&sale, sizeof(SALE), 1, tfd);
		if(sale.status == ACTIVE){
			printf("Backorder: %ld\nDate of order: %s\nCustomer ID:%ld\nCustomer name: %s\nProduct ID: %ld\nProduct name: %s\nQuantity attempted to be purchased: %d\nCost of the failed sale: $ %.2f\n\n\n", sale.TID, sale.date, sale.CID, sale.name,
			sale.PID, sale.productname, sale.quantity, (float)sale.totalcost/DOLLARSTOCENTS);
		}
	}
	
	//Close relative file
	fclose(tfd);
	
	return 0;
}

/*This function product id as the argument 
and seacrhes for the supplier that supplies the product
and creates a daily order text file */
int dailyorders(int input){
	//intialize all structures needed
	PRODUCT product;
	SUPPLIER supplier;
	
	
	int check;
	//open product and supplier reletive files to read
	FILE * pfd = fopen("productsrelativefile.dat", "r");
	FILE * sfd = fopen("suppliersrelativefile.dat", "r");
	char filename[MAXLEN];
	sprintf(filename, "ORDERS%d", globaldate);

	FILE * dfd = fopen(filename, "a");
	
	//fseek to the relvant product id 
	fseek(pfd, (input-SALESFIRSTID)*sizeof(PRODUCT) + sizeof(HEADER), SEEK_SET);
	//fread the product stuff into structure
	fread(&product, sizeof(PRODUCT), 1, pfd);

	//find supplier using the manufacturer and place the file pointer at the start of the records in the supplier file
	fseek(sfd, sizeof(HEADER), SEEK_SET);
	while(check!= 0){
		fread(&supplier, sizeof(SUPPLIER), 1, sfd);
		check = strcmp(product.manufacturer, supplier.manufacturer);
	}
	//print the daily order to the text file ORDERS with encoded date
	fprintf(dfd, "%s, %ld, %s, %s, %s, %s, %sQuantity to be orderd: 10\n",globaldatestring, product.PID, product.classification, product.manufacturercode, supplier.contact, supplier.telephone, supplier.email);
	fclose(dfd);
	
}

int paymentdue(void){
	//Initialize filename string for 30 days ago ORDERS file
	char pastfilename[MAXLEN];
	//Initialize command to copy and rename ORDERS file
	char command[MAXLEN]; //This length is because we are using two filenames that  could be up to maxlen + the copy command
	//Fill filename with string
	sprintf(pastfilename, "ORDERS%d", globaldate-30);
	if(access(pastfilename,F_OK) == 0){
		char filename[MAXLEN];
		sprintf(filename, "PAY_DUE%d", globaldate);
		//Used to format shell command: copy ORDERS(date-30) PAY_DUE(date)
		sprintf(command, "copy %s %s", pastfilename, filename);
		//execute copy command
		system(command); //The system function passes a string to the command line. Here we use the command line function copy to copy and rename the chosen file. Source: https://www.tutorialspoint.com/c_standard_library/c_function_system.htm
	}
	else{
		return 0;
	}

}
/**/
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
	FILE * tfd = fopen("backorders.dat", "r+");
	FILE * pfd = fopen("productsrelativefile.dat", "r+");
	char filename[MAXLEN];
	sprintf(filename, "ORDERSFILLED%d", globaldate);
	FILE * ordersfilled = fopen(filename, _access("customersrelativefile.dat", 0) < 0 ? "w" : "a");
	
	//Find first_id from header
	fseek(tfd, 0, SEEK_SET);
	fread(&header, sizeof(HEADER), 1, tfd);
	
	//Go through loop moving seeking and reading each element and then if that element mataches the PID and is not deleted printing to daily ORDERSFILLED
	for(int i = 0;i<header.first_id-SALESFIRSTID; i++){
		fread(&sale, sizeof(SALE), 1, tfd);
		if((sale.status == ACTIVE) && (sale.PID == inputID)){
			//Set filemode as binary
			_fmode = _O_TEXT;
			//print sale to file for order filled
			fprintf(ordersfilled, "Sale: %ld, %ld: %s, %ld: %s, %d, $ %.2f\n", sale.TID, sale.CID, sale.name,
			sale.PID, sale.productname, sale.quantity, (float)sale.totalcost/DOLLARSTOCENTS);
			//Set filemode as binary
			_fmode = _O_BINARY;

			//For every applicable item remove from inputted quantiy the quantity of that order
			inputQuantity-=sale.quantity;
			if(inputQuantity<0){
				printf("Error too little quantity to fill all orders.\n");
				//Unremove stock of order if negative. This allows the above inputQuantity-=sale.quantity; to be valid in any case.
				inputQuantity+=sale.quantity;
				break;
			}
			else{
				//Only delete backorder if inputquantity did not go under 0.
				deletebackorders(sale.TID);
			
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
	FILE * tfd = fopen("backorders.dat", "r+");
	
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





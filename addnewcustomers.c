#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>


#include "relativefiles.h"

int initcustomers(void){

	HEADER_CUSTOMER header;

	CUSTOMER newcustomer;
	char *element;
	char temprecord[MAXREC];
	
	FILE * input = fopen("Customers.txt", "r");
	FILE * output = fopen("customersrelativefile.txt", _access("customersrelativefile.txt", 0) < 0 ? "w" : "r+");

	long customerid = header.first_cid = 1001; //1001 is the first CID
	
	fseek(output, 0, SEEK_SET);
	fwrite(&header, sizeof(HEADER_CUSTOMER), 1, output);
	
	fgets(temprecord, MAXREC, input); //Disregard first line
	
	while (fgets(temprecord, MAXREC, input)){
		TRUNCATE(temprecord);
		newcustomer.CID = customerid;
		
		element = strtok(temprecord, ",");
		strcpy(newcustomer.name, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.businessname, element);
	
		element = strtok(NULL, ",");
		strcpy(newcustomer.streetaddress, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.town, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.province, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.postalcode, element);
		
		element = strtok(NULL, ",");	
		strcpy(newcustomer.telephone, element);
		
		customerid++;
		fseek(output, ((newcustomer.CID - 1001) * sizeof(CUSTOMER)) + sizeof(HEADER_CUSTOMER), SEEK_SET);
		fwrite(&newcustomer, sizeof(CUSTOMER), 1, output);
	}
	fclose(input);
	fclose(output);
	
	return 0;
	
}

int readcustomers(void){
	CUSTOMER customer;
	FILE * output = fopen("customersrelativefile.txt", "r+");
	
	fseek(output, sizeof(HEADER_CUSTOMER), SEEK_SET);
	fread(&customer, sizeof(CUSTOMER), 1, output);
	printf("Customer:%s, %d, %s, %s, %s, %s, %s, %s\n", customer.name, customer.CID, customer.businessname, customer.streetaddress, customer.town, customer.province, customer.postalcode, customer.telephone);
}





int addnewcustomers(void)
{
/* Add a customer to the customerfile */
CUSTOMER newcustomer;
HEADER_CUSTOMER header;
int customer_id;


FILE * cfd = fopen("customersrelativefile.txt", _access("customersrelativefile.txt", 0) < 0 ? "w" : "r+");


/* Access header record to get first available customer id */
fseek(cfd, 0, SEEK_SET);
fread(&header, sizeof(HEADER_CUSTOMER), 1, cfd);


/* Geting all the information */

printf("Enter name\n");
fgets(newcustomer.name, NAMELEN, stdin);
TRUNCATE(newcustomer.name);

printf("Enter businessname\n");
fgets(newcustomer.businessname, MAXLEN, stdin);
TRUNCATE(newcustomer.businessname);

printf("Enter street address\n");
fgets(newcustomer.streetaddress, MAXLEN, stdin);
TRUNCATE(newcustomer.streetaddress);

printf("Enter town\n");
fgets(newcustomer.town, MAXLEN, stdin);
TRUNCATE(newcustomer.town);

printf("Enter province\n");
fgets(newcustomer.province, MAXLEN, stdin);
TRUNCATE(newcustomer.province);

printf("Enter postalcode\n");
fgets(newcustomer.postalcode, MAXLEN, stdin);
TRUNCATE(newcustomer.postalcode);

printf("Enter Telephone\n");
fgets(newcustomer.postalcode, MAXLEN, stdin);
TRUNCATE(newcustomer.postalcode);



/* Complete remaining fields */
newcustomer.CID= customer_id;
/* Display complete record */
printf("ID: >%d< Name: >%s< businessname: >%s< Province: >%s<  Town: >%s<  postal code: >%s<  telephone: >%s<\n", 
	newcustomer.CID, newcustomer.name, newcustomer.businessname, newcustomer.province, newcustomer.town, newcustomer.postalcode, newcustomer.telephone);
/* 
 - Write customer data to customer file
*/
fwrite(&newcustomer, sizeof(CUSTOMER), 1, cfd);

fseek(cfd, 0, SEEK_SET);
header.first_cid = header.first_cid + 1;
fwrite(&header, sizeof(HEADER_CUSTOMER), 1, cfd);
}










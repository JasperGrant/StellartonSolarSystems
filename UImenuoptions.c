/* 
customers.c
Definition file for functions related to customers.
Date modified: 2021-11-19
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

//Initialize stores

//Add to stores
//Function brings user to menu of adding options
int addelements(void){
	//function selection variable
	int input;
	printf("Select Function\n1. Add new customers\n2. Add new suppliers\n3. Add new products\n");
	scanf("%d", &input);
	switch(input){
		case 1:
			//Add new customers
			printf("Adding new customers...\n");
			addnewcustomers();
			break;
			
		case 2:
			//Add new suppliers
			printf("Adding new suppliers...\n");\
			addnewsuppliers();
			break;
			
		case 3:
			//Add new products
			printf("Adding new products...\n");
			addnewproducts();
			break;
	}
				
}

//Delete from stores
//Function brings user to menu of deleting options
int deleteelements(void){
	//function selection variable
	int input;
	printf("Select Function\n1. Delete customers\n2. Delete suppliers\n3. Delete products\n4. Delete sales\n");
	scanf("%d", &input);
	switch(input){
		case 1:
			//Delete customers
			printf("Deleting customers...\n");
			userdeletecustomers();
			break;
			
		case 2:
			//Delete suppliers
			printf("Deleting suppliers...\n");\
			userdeletesuppliers();
			break;
			
		case 3:
			//Delete products
			printf("Deleting products...\n");
			userdeleteproducts();
			break;
		case 4:
			//Delete sales
			printf("Deleting sales...\n");
			userdeletesales();
			break;
	}
				
}

//Display Stores
//Function brings user to menu of displaying options
int displayelements(void){
	//function selection variable
	int input;
	printf("Select Function\n1. Display customers\n2. Display suppliers\n3. Display products\n4. Display sales\n5. Display backorders\n");
	scanf("%d", &input);
	switch(input){
		case 1:
			//Display customers
			printf("Displaying customers...\n");
			readcustomers();
			break;
			
		case 2:
			//Display suppliers
			printf("Displaying suppliers...\n");\
			readsuppliers();
			break;
			
		case 3:
			//Display products
			printf("Displaying products...\n");
			readproducts();
			break;
		case 4:
			//Display sales
			printf("Displaying sales...\n");
			readsales();
			break;
		case 5:
			//Display backorders
			printf("Displaying backorders...\n");
			readbackorders();
			break;
	}
				
}

//Change Stores
//Function brings user to menu of changing options
int changeelements(void){
	//function selection variable
	int input;
	printf("Select Function\n1. Change element of customer\n2. Change element of supplier\n3. Change element of product\n");
	scanf("%d", &input);
	switch(input){
		case 1:
			//Change customers
			printf("Changing customers...\n");
			changecustomers();
			break;
			
		case 2:
			//Change suppliers
			printf("Changing suppliers...\n");\
			changesuppliers();
			break;
			
		case 3:
			//Change products
			printf("Changin products...\n");
			changeproducts();
			break;
		
	}
				
}


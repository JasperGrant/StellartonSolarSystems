#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int main(void){
	//_set_fmode_(_O_BINARY);
	
	//function selection variable
	int input;
	
	//Loop continues to run as customer uses functions. Ends on a selection of Quit
	while(1){
		
		//Print initial welcome message and user options
		printf("Welcome to the Stellarton Solar Systems Digital Storage System\n");
		printf("Select function:\n1. Add new customers\n2. Add new suppliers\n3. Add a new product\n4. Make sale\n5. Initialize customer, product, supplier, and sales stores\n6. Display customer store\n7. Display product store\n8. Display supplier store\n9. Display sales store\n0. Quit\n");
		
		//Read user input
		scanf("%d", &input);
		
		//Switch based on entered integer
		switch(input){
			//Quit case
			case 0:
				return 0;
			
			//Adding cases
			case 1:
				printf("Adding new customers...\n");
				addnewcustomers();
				break;
			
			case 2:
				printf("Adding new suppliers...\n");\
				addnewsuppliers();
				break;
			
			case 3:
				printf("Adding new products...\n");
				addnewproducts();
				break;
				
			
			case 4:
				printf("Making sale...\n");
				addnewsales();
				break;
			
			//Initialization cases
			case 5:
				//Password protected to protect system
				printf("Enter password: ");
				scanf("%d", &input);
				if(input == 1234){ //If correct password
					printf("Initializing stores...\n");
					initcustomers();
					initproducts();
					initsuppliers();
					initsales();
				}
				else{ //If wrong password
					printf("Incorrect password, files not initialized\n");
				}
				break;
			
			//Displaying cases
			case 6:
				printf("Displaying customers...\n");
				readcustomers();
				break;
			
			case 7:
				printf("Displaying products...\n");
				readproducts();
				break;
			
			case 8:
				printf("Displaying suppliers...\n");
				readsuppliers();
				break;
		
			case 9:
				printf("Displaying sales...\n");
				readsales();
				break;
		}
	}
}

/* 
UI.c
File containing main UI that allows users to select functions of the program.
Date modified: 2021-11-19
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

//Global date variables
int globaldate;
char globaldatestring[11];

int main(void){
	
	//Set filemode as binary
	_fmode = _O_BINARY;
	
	//function selection variable
	int input;
	
	//String representing date
	char datestring[11];
	
	//Print welcome message
	printf("Welcome to the Stellarton Solar Systems Digital Storage System\n");
	
	//Prompt for today's date
	printf("What is today's date? Format: YYYY:MM:DD\n");
	fgets(datestring, 11, stdin);
	
	//Convert to operable int
	globaldate = datestringtoint(datestring);
	
	//Copy input string to global string
	strcpy(globaldatestring, datestring);
	
	
	
	//Loop continues to run as product uses functions. Ends on a selection of Quit
	while(1){
		
		//Print initial user options
		printf("Select function:\n1. Add new customers\n2. Add new suppliers\n3. Add a new product\n4. Make sale\n5. Initialize customer, product, supplier, and sales stores\n6. Display customer store\n7. Display product store\n8. Display supplier store\n9. Display sales store\n10. Update Information\n0. Quit\n");
		
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
				//Correct password is 1234
				if(input == 1234){ //If correct password
					printf("Initializing stores...\n");
					initcustomers();
					initproducts();
					initsuppliers();
					initsales();
					initbackorders();
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
				
			case 15:
				printf("Displaying backorders...\n");
				readbackorders();
				break;
			
			case 10:
				//Sub menu
			    printf("Select function:\n1. Change a Customer Field\n2. Change a Product Field\n3. Change a Supplier Field\n");
			    
			    scanf("%d", &input);
			    
			    switch(input){
			    	case 1:
			    		printf("Updating Customer Fields...\n");
			    		changecustomers();
			    		break;
			    	
			    	case 2:
			    		printf("Updating Product Fields...\n");
			    	    changeproducts();
			    	    break;
			    	
			    	case 3:
			    		printf("Updating Supplier Fields...\n");
			    	    changesuppliers();
			    	    break;
				}
				break;
			case 11:
				userdeletecustomers();
				break;
			case 12:
				userdeleteproducts();
			break;
			case 13:
				userdeletesuppliers();
			break;
			case 14:
				userdeletesales();
			break;
		}
	}
}




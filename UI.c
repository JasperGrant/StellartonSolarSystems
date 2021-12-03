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
extern int globaldate;
extern char globaldatestring[DATELEN];

int main(void){
	
	//Set filemode as binary
	_fmode = _O_BINARY;
	
	//function selection variable
	int input;
	
	//String representing date
	char datestring[DATELEN];
	
	//Print welcome message
	printf("Welcome to the Stellarton Solar Systems Digital Storage System\n");
	
	//Prompt for today's date
	printf("What is today's date? Format: YYYY:MM:DD\n");
	fgets(datestring, DATELEN, stdin);
	
	//Convert to operable int
	globaldate = datestringtoint(datestring);

	//Copy input string to global string
	strcpy(globaldatestring, datestring);
	
	//Generate 30 day payment reminders
	paymentdue();
	
	//Loop continues to run as product uses functions. Ends on a selection of Quit
	while(1){
		
		//Print initial user options
		printf("Select function:\n1.Change stores\n2.Add to stores\n3.Delete Records\n4.Display Stores\n5.Initialize Stores\n6.Make Sale\n7.Fill backorders\n0.Quit\n");
		
		//Read user input
		scanf("%d", &input);
		
		//Switch based on entered integer
		switch(input){
			//Quit case
			case 0:
				return 0;
			
			//updating records case
			case 1:
				printf("Updating elements...\n");
				changeelements();
				break;
			//Adding new records case
			case 2:
				printf("Adding new records...\n");
			    addelements();
				break;
			//deleting records case
			case 3:
				printf("Deleting records...\n");
				deleteelements();
				break;
				
			//disply stores case
			case 4:
				displayelements();
				printf("Displaying stores...\n");
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
			
			//Making sales case
			case 6:
				printf("Making a sale...\n");
				addnewsales();
				break;
		    //filling up a backorder case
			case 7:
				printf("Filling backorders...\n");
				fillbackorders();
			break;
		}
	}
}




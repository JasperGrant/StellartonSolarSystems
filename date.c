/* 
date.c
Definition file for functions to convert date from to string to integer and back
Date modified: 2021-12-01
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

#define STARTYEAR 2000 //This is the first year of of our business. Day 0. This can be adjusted if needed.

//Array to hold number of days in a specific month
int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int datestringtoint(char * string){	
	//Declare date variables
	int year, month, day, dateint = 0;
	//Scan string to retrieve values of year month and day
	sscanf(string, "%d:%d:%d", &year, &month, &day); //sscanf is used to read through a string as if it were data coming in through stdin. Source: https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm 
	//Go through each previous month adding their variable number of days to total
	for(int i = 0; i<(month-1); i++){
		dateint+=months[i];
	}
	//Add number of days and years since STARTYEAR to added up months
	return dateint + day + 365*(year-STARTYEAR);
}

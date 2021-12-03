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
#define YEARDAYS 365 //This is the number of days in a non leap year
#define LEAPYEARDAYS 366 //This is the number of days in a leap year

//Arrays to hold number of days in a specific month for leap years and none leap years
int normalmonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int leapyearmonths[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int usedmonths[12];

int datestringtoint(char * string){	
	//Declare date variables
	int year, month, day, dateint = 0;
	//Scan string to retrieve values of year month and day
	sscanf(string, "%d:%d:%d", &year, &month, &day); //sscanf is used to read through a string as if it were data coming in through stdin. Source: https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm 
	//Go through each previous month adding their variable number of days to total
	if(year%4 == 0){
		memcpy(usedmonths, leapyearmonths, sizeof(usedmonths)); //memcpy function is like strcpy() but it can be used for arrays and takes a size argument. Source: https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm
	}
	else{
		memcpy(usedmonths, normalmonths, sizeof(usedmonths));
	}
	for(int i = 0; i<(month-1); i++){
		dateint+=usedmonths[i];
	}
	//Add number of years since STARTYEAR to added up months factoring in leap years
	for(int i = STARTYEAR;i<year; i++){
		if(i%4 == 0){
			dateint+=366;
		}
		else{
			dateint+=365;
		}
	}
	//Add number of days since STARTYEAR to added up months
	return dateint + day;
}


/* 
stringtoint.c
File containing function which converts a string representing a decimal number to an integer which represents that number multiplied by 100 to make an integer
Date modified: 2021-12-03
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int stringtointegervalue(char * string){
	//declare integers
	int dollars, cents;
	//use sscanf to pull out two integers from the string
	sscanf(string, "%d.%d", &dollars, &cents);
	return dollars*100+cents;
}

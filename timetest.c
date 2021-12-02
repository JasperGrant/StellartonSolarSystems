
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
	char * string;
	char * string2 = "2000:10:21";
	int year, month, day;
	sscanf(string2, "%d:%d:%d",&year,&month,&day);
	printf("%d, %d, %d", year, month, day);
}

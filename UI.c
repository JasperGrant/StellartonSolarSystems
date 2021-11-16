#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int main(void){
	int input;
	initcustomers();
	while(1){
		printf("Select function:\n1. Add new customers\n2. Add new suppliers\n3. Add a new product\n4. Make sale\n0. Quit\n");
		scanf("%d", &input);
		if(input == 0){
			return 0;
		}
		if(input == 1){
			printf("Adding new customers\n");
		}
		if(input == 2){
			printf("Adding new suppliers\n");
		}
		if(input == 3){
			printf("Adding new products\n");
			
		}
		if(input == 4){
			printf("Making sale\n");
		}
	}
}

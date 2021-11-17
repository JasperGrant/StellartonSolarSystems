#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "relativefiles.h"

int main(void){
	//_set_fmode_(_O_BINARY);
	int input;
	while(1){
		readcustomers();
		readproducts();
		printf("Select function:\n1. Add new customers\n2. Add new suppliers\n3. Add a new product\n4. Make sale\n5. Initialize customer, product and supplier stores\n0. Quit\n");
		scanf("%d", &input);
		if(input == 0){
			return 0;
		}
		if(input == 1){
			printf("Adding new customers\n");
			addnewcustomers();
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
		if(input == 5){
			printf("Initializing stores");
			initcustomers();
			initproducts();
		}
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#define NAMELEN 32
#define BUSINESSNAMELEN 16
#define ADRESSLEN 32

struct customer{
	long CID;
	char name[NAMELEN];
	char businessname[BUSINESSNAMELEN];
	char streetaddress[ADRESSLEN];
	char town[NAMELEN];
	char province[NAMELEN];
	char postalcode[NAMELEN];
	char telephone[NAMELEN];
};
typedef struct customer CUSTOMER;



int addnewcustomers(void);
int addnewpeoducts(void);
int addnewsuppliers(void);
int makesale(void);

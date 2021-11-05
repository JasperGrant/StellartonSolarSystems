#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#define NAMELEN 16
#define BUSINESSNAMELEN 16
#define ADRESSLEN 32

struct customer{
	long CID;
	char lastname[NAMELEN];
	char firstname[NAMELEN];
	char businessname[BUSINESSNAMELEN];
	char streetaddress[ADRESSLEN];
	char town[NAMELEN];
	char province[NAMELEN]
	char postalcode[NAMELEN];
	char telephone[]
};



int addnewcustomers(void);
int addnewpeoducts(void);
int addnewsuppliers(void);
int makesale(void);

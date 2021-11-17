#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define NAMELEN 32
#define MAXLEN 32
#define MAXREC 256
#define TRUNCATE(name) name[strlen(name)-1] = '\0'

struct customer{
	long CID;
	char name[MAXLEN];
	char businessname[MAXLEN];
	char streetaddress[MAXLEN];
	char town[MAXLEN];
	char province[MAXLEN];
	char postalcode[MAXLEN];
	char telephone[MAXLEN];
};
typedef struct customer CUSTOMER;

struct product{
	long PID;
	char name[MAXLEN];
	char classification[MAXLEN];
	char manufacturer[MAXLEN];
	int unitcost;
	char manufacturercode[MAXLEN];
	int stock;
};
typedef struct product PRODUCT;

struct supplier{
	long SID;
	char manufacturer[MAXLEN];
	char contact[MAXLEN];
	char company[MAXLEN];
	char address[MAXLEN];
	char telephone[MAXLEN];
	
};
typedef struct supplier SUPPLIER;

struct header
{
	long first_id; //First availible ID
};
typedef struct header HEADER;



int initcustomers(void);
int addnewcustomers(void);
int readcustomers(void);
int initproducts(void);
int addnewproducts(void);
int addnewsuppliers(void);
int makesale(void);

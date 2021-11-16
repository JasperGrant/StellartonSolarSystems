#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

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
	char name[MAXLEN];
	char classificationn[MAXLEN];
	char manufacturer[MAXLEN];
	int unitcost[MAXLEN];
	char manufacturercode[MAXLEN];
	int stock;
};

struct supplier{
	char manufacturer[MAXLEN];
	char contact[MAXLEN];
	char company[MAXLEN];
	char address[MAXLEN];
	char telephone[MAXLEN];
	
};

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

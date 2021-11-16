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

struct header_customer
{
	long first_cid;		/* First available sid */
	long del_rec_list;	/* Deleted record list */
};
typedef struct header_customer HEADER_CUSTOMER;
extern FILE *cfd;



int initcustomers(void);
int addnewcustomers(void);
int readcustomers(void);
int initproducts(void);
int addnewproducts(void);
int addnewsuppliers(void);
int makesale(void);

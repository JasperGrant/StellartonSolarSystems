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

union customer_file
{
CUSTOMER crec;
HEADER_CUSTOMER hrec;
DEL_CUS drec;
};
typedef union customer_file CFREC;

struct header_customer
{
long del_rec_list;	/* Deleted record list */
long first_cid;		/* First available sid */
};
typedef struct header_customer HEADER_CUSTOMER;
extern FILE *cfd;



int initcustomers(void);
int addnewpeoducts(void);
int addnewsuppliers(void);
int makesale(void);

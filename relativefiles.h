/* 
relativefiles.h
Header file that contains structs and declarations of all functions.
Date modified: 2021-11-19
Authors: Jasper Grant B00829263, Rehan Khalid B00826127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define MAXLEN 32 //Used for standard 32 byte elements
#define MAXREC 288 //Max length of entire structure
#define LONGMAXLEN 64 //Used for elements that will be longer then 32 bytes
#define TRUNCATE(name) name[strlen(name)-1] = '\0' //Macro to remove a 

//Customer structure
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

//Product structure
struct product{
	long PID;
	char name[LONGMAXLEN];
	char classification[MAXLEN];
	char manufacturer[MAXLEN];
	int unitcost;
	char manufacturercode[MAXLEN];
	int stock;
};
typedef struct product PRODUCT;

//Supplier structure
struct supplier{
	long SID;
	char manufacturer[MAXLEN];
	char contact[MAXLEN];
	char company[MAXLEN];
	char address[MAXLEN];
	char telephone[MAXLEN];
	char email[LONGMAXLEN];
};
typedef struct supplier SUPPLIER;

//General header structure
struct header{
	long first_id; //First availible ID
};
typedef struct header HEADER;

//Sale structure
struct sale{
	long TID;
	long CID;
	char name[MAXLEN];
	long PID;
	char productname[LONGMAXLEN];
	int quantity;
	int totalcost;
};
typedef struct sale SALE;

//Customer functions
extern int initcustomers(void);
extern int addnewcustomers(void);
extern int readcustomers(void);
extern int lookupcustomer(void);
//Product functions
extern int initproducts(void);
extern int addnewproducts(void);
extern int readproducts(void);
extern int lookupproduct(void);

//Supplier functions
extern int initsuppliers(void);
extern int addnewsuppliers(void);
extern int readsuppliers(void);
extern int lookupsupplier(void);
//Sales functions
extern int initsales(void);
extern int addnewsales(void);
extern int readsales(void);


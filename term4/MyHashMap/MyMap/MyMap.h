#ifndef __MYMAP_H
#define __MYMAP_H

#include "list.h"

typedef struct MyMap MyMap_t;
typedef struct TableRow TableRow_t;
typedef enum {MYMAP_OK, MYMAP_BADPOINTER, MYMAP_BADTABLEPOINTER} MyMapErrorsEnum;
MyMapErrorsEnum MyMapError;


#define PRIME 257
#define TABLESIZE 257
#define SCALECONSTANT 2

struct TableRow {
    /*long long unsigned hashValue;*/
    MyList_t listOfItems;
};

struct MyMap {
    TableRow_t* table;
    /*const int bucketAddition;
    long long unsigned rowsNum;
    long long unsigned freeRows;
    const long long unsigned powers[ITEMSIZE];*/
    long long unsigned prime;
};

void MyMapCreate(MyMap_t* map);
void MyMapDestroy(MyMap_t* map);
void MyMapAdd(MyMap_t* map, char* key, char* value);
//void MyMapAutoScale(MyMap_t* map);
void MyMapPrint(MyMap_t* map);
int MyMapRemove(MyMap_t* map, char* key);
char* MyMapGetByKey(MyMap_t* map, char* key);
long long unsigned MyMapHashFunction(MyMap_t* map, char* key);
void MyMapDump();
int MyMapOk(MyMap_t* map);

#endif
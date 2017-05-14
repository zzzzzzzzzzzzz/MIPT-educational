#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include "MyMap.h"
#include <string.h>

void MyMapCreate(MyMap_t* map) {
    MyMapError = MYMAP_OK;
    map->table = (TableRow_t*)malloc(sizeof(TableRow_t)*TABLESIZE);
    if (map->table == NULL) {
        printf("MyMapCreate: couldn't allocate memory for table\n");
        exit(errno);
    }
    for (int i = 0; i < TABLESIZE; i++) {
        MyListCreate(&map->table[i].listOfItems);
    }
    map->prime = PRIME;
    /*map->rowsNum += map->bucketAddition;
    map->freeRows = map->rowsNum;*/
    /*calculate powers of prime number only once*/
    /*map->powers[0] = 1;
    for (int i = 1; i < ITEMSIZE; i++) {
        map->powers[i] = map->prime * map->powers[i-1];
    }*/
}

void MyMapDestroy(MyMap_t* map) {
    if (!MyMapOk(map)) {
        MyMapDump();
        printf("\nDESTROY\n");
        return;
    }
    for (int i = 0; i < TABLESIZE; i++) {
        MyListDestroy(&map->table[i].listOfItems);
    }
    free(map->table);
    map->table = NULL;
}

/*void MyMapAutoScale(MyMap_t* map) {
    map = realloc(map->table, sizeof(TableRow_t)*(map->rowsNum + map->bucketAddition));
    for (int i = map->freeRows; i < map->rowsNum + map->bucketAddition; i++) {
        map->table[i]->hashValue = 0;
        ListCreate(map->table[i]->listOfItems);        
    }
    map->rowsNum += map->bucketAddition;
    map->freeRows += map->bucketAddition;
}*/

long long unsigned MyMapHashFunction(MyMap_t* map, char* key) {
    if (!MyMapOk(map)) {
        MyMapDump();
        return 0;
    }
    long long unsigned sum = 0;
    for (int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % map->prime;
}

void MyMapAdd(MyMap_t* map, char* key, char* value) {
    if (!MyMapOk(map)) {
        MyMapDump();
        return;
    }
    long long unsigned hash = MyMapHashFunction(map, key);
    /*if (map->table[i]->hashValue == 0) {
        map->freeRows--;
    }*/
    MyListAppend(&map->table[hash].listOfItems, key, value);
    /*if (map->freeRows < SCALECONSTANT) {
        MyMapAutoScale(map);
    }*/
}

int MyMapRemove(MyMap_t* map, char* key) {
    if (!MyMapOk(map)) {
        MyMapDump();
        return 0;
    }
    long long unsigned hash = MyMapHashFunction(map, key);
    if (!MyListRemoveByKey(&map->table[hash].listOfItems, key)) {
        return 0;
    } else {
        return 1;
    }
}

char* MyMapGetByKey(MyMap_t* map, char* key) {
    if (!MyMapOk(map)) {
        MyMapDump();
        return NULL;
    }
    long long unsigned hash = MyMapHashFunction(map, key);
    return MyListGetItem(&map->table[hash].listOfItems, key);
}

void MyMapPrint(MyMap_t* map) {
    if (!MyMapOk(map)) {
        MyMapDump();
        return;
    }
    for (int i = 0; i < TABLESIZE; i++) {
        printf(" [%d] ===>", i);
        MyListPrint(&map->table[i].listOfItems);
        printf("\n");
    }
}

int MyMapOk(MyMap_t* map) {
    if (!map) {
        MyMapError = MYMAP_BADPOINTER;
    } else {    
        if (!map->table) {
            MyMapError = MYMAP_BADTABLEPOINTER;
        } else {
            MyMapError = MYMAP_OK;
        }
    }
    
    if (MyMapError > 0) {
        return 0;
    } else {
        return 1;
    }
}

void MyMapDump() {
    if (MyMapError == MYMAP_BADPOINTER) {
        printf("\nMyMapDump: Bad pointer was given!\n");
    }
    
    if (MyMapError == MYMAP_BADTABLEPOINTER) {
        printf("\nMyMapDump: Something happened with your table!\n");
    }
}
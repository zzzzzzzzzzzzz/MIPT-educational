#include <stdio.h>
#include <string.h>
#include "MyMap.h"

int testAmount;
int testOkAmount;
int testFailAmount;

#define CHECK(CONDITION, TESTNAME, ROLLBACKERROR) \
    testAmount++; \
    if (CONDITION) { \
        testOkAmount++; \
    } else { \
        printf(TESTNAME); \
        printf(" failed\n"); \
        testFailAmount++; \
    } \
    if (ROLLBACKERROR) { \
        MyMapError = MYMAP_OK; \
        MyListError = MYLIST_OK; \
    } \


int main(int argc, char **argv)
{
    testAmount = 0;
    testOkAmount = 0;
    testFailAmount = 0;
    MyMap_t* p = NULL;
    MyMapAdd(p, "xxx", "yyy");
    CHECK(MyMapError == MYMAP_BADPOINTER, "MyMapAdd, bad pointer check", 1);
    MyMapPrint(p);
    CHECK(MyMapError == MYMAP_BADPOINTER, "MyMapPrint, bad pointer check", 1);
    MyMapRemove(p, "xxx");
    CHECK(MyMapError == MYMAP_BADPOINTER, "MyMapRemove, bad pointer check", 1);
    MyMapGetByKey(p, "asas");
    CHECK(MyMapError == MYMAP_BADPOINTER, "MyMapGetByKey, bad pointer check", 1);
    MyMapHashFunction(p, "abc");
    CHECK(MyMapError == MYMAP_BADPOINTER, "MyMapHashFunction, bad pointer check", 1);
    MyMapDestroy(p);
    CHECK(MyMapError == MYMAP_BADPOINTER, "MyMapDestroy, bad pointer check", 1);
    
    MyMap_t map;
    MyMapCreate(&map);
    CHECK(map.table != NULL, "MyMapCreate, create check", 1);
    MyMapAdd(&map, "key one", "one");
    CHECK(strcmp(MyListGetItem(&map.table[MyMapHashFunction(&map, "key one")].listOfItems, "key one"), "one") == 0, "MyMapAdd, adding item check", 1);
    MyMapAdd(&map, "key one", "one changed");
    CHECK(strcmp(MyListGetItem(&map.table[MyMapHashFunction(&map, "key one")].listOfItems, "key one"), "one changed") == 0, "MyMapAdd, reassignment item check", 1);
    MyMapAdd(&map, "key owt", "more to this hashvalue");
    CHECK(strcmp(MyMapGetByKey(&map, "key owt"), "more to this hashvalue") == 0, "MyMapGetByKey, getting by key test", 1);
    MyMapAdd(&map, "key three", "three");
    MyMapPrint(&map);
    MyMapRemove(&map, "key one");
    CHECK(MyMapGetByKey(&map, "key one") == NULL, "MyMapRemove, removing item check", 1);
    TableRow_t* t = map.table;
    map.table = NULL;
    MyMapAdd(&map, "abc", "def");
    CHECK(MyMapError == MYMAP_BADTABLEPOINTER, "MyMapAdd, bad table pointer check", 1);
    MyMapDestroy(&map);
    CHECK(MyMapError == MYMAP_BADTABLEPOINTER, "MyMapDestroy, bad table pointer check", 1);
    map.table = t;
    MyMapDestroy(&map);
    
    MyMapCreate(&map);
    MyMapDestroy(&map);
    CHECK(map.table == NULL, "MyMapDestroy, check destroy", 1);
    
    if (testAmount == testOkAmount) {
        printf("All %d test ok\n", testAmount);
    } else {
        printf("Some of tests failed\n");
        printf("Tests amount %d\n", testAmount);
        printf("Tests ok: %d\n", testOkAmount);
        printf("Tests failed: %d\n", testFailAmount);
    }
    
	return 0;
}
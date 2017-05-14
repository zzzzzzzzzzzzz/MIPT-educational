#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyCPPList.h"

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
        MyListError = MYLIST_OK; \
    } \


int main() {
    testAmount = 0;
    testOkAmount = 0;
    testFailAmount = 0;
    MyList_t test;
    MyListCreate(&test);
    CHECK(test.head != NULL, "MyListCreate, check creation", 1);
    MyListRemoveByKey(&test, "lala");
    CHECK(MyListError == MYLIST_NOSUCHITEM, "MyListRemoveByKey, removing item which not exists in list", 1);
    MyListGetItem(&test, "key lalal");
    CHECK(MyListError == MYLIST_NOSUCHITEM, "MyListGetItem, getting item which not exists in list", 1);
    MyListAppend(&test, "key one", "one");
    CHECK(strcmp(MyListGetItem(&test, "key one"), "one") == 0, "MyListAppend, check item append", 1);
    MyListAppend(&test, "key two", "two");
    CHECK(strcmp(MyListGetItem(&test, "key two"), "two") == 0, "MyListAppend, check item append", 1);    
    MyListAppend(&test, "key three", "three");
    CHECK(strcmp(MyListGetItem(&test, "key three"), "three") == 0, "MyListAppend, check item append", 1);
    MyListAppend(&test, "key four", "four");
    CHECK(strcmp(MyListGetItem(&test, "key four"), "four") == 0, "MyListAppend, check item append", 1);
    MyListAppend(&test, "key five", "five");
    CHECK(strcmp(MyListGetItem(&test, "key five"), "five") == 0, "MyListAppend, check item append", 1);
    MyListPrint(&test);
    printf("\n");
    MyListSortByKey(&test);
    Node_t* prev = test.head->next;
    Node_t* front = prev->next;
    int flag = 1;
    while (front != NULL) {
        if (strcmp(prev->key, front->key) > 0) {
            flag = 0;
            break;
        }
        prev = front;
        front = front->next;
    }
    CHECK(flag == 1, "MyListSortByKey, check sort by key", 1);
    MyListPrint(&test);
    printf("\n");
    char* str = MyListPop(&test);
    CHECK(strcmp(str, "two") == 0, "MyListPop, check pop", 1);
    free(str);
    MyListAppend(&test, "key two", "two CHANGED");
    CHECK(strcmp(MyListGetItem(&test, "key two"), "two CHANGED") == 0, "MyListAppend, check existing item changing", 1);
    CHECK(MyListRemoveByKey(&test, "key one") == 1, "MyListRemoveByKey, removing item", 1);
    CHECK(MyListRemoveByKey(&test, "key three") == 1, "MyListRemoveByKey, removing item", 1);
    CHECK(MyListGetItem(&test, "key xxx") == NULL, "MyListGetItem, getting not existing item in not empty list", 1);
    MyListDestroy(&test);
    CHECK(test.head == NULL, "MyListDestroy, check destroying item", 1);
    //Sending NULL
    MyList_t* p;
    p = NULL;
    MyListAppend(p, "xxx", "yyy");
    CHECK(MyListError == MYLIST_BADPOINTER, "MyListAppend, bad pointer check", 1);
    MyListPrint(p);
    CHECK(MyListError == MYLIST_BADPOINTER, "MyListPrint, bad pointer check", 1);
    MyListRemoveByKey(p, "xxx");
    CHECK(MyListError == MYLIST_BADPOINTER, "MyListRemoveByKey, bad pointer check", 1);
    MyListGetItem(p, "yyy");
    CHECK(MyListError == MYLIST_BADPOINTER, "MyListGetItem, bad pointer check", 1);
    MyListIsEmpty(p);
    CHECK(MyListError == MYLIST_BADPOINTER, "MyListIsEmpty, bad pointer check", 1);
    MyListPop(p);
    CHECK(MyListError == MYLIST_BADPOINTER, "MyListPop, bad pointer check", 1);
    MyListDestroy(p);
    CHECK(MyListError == MYLIST_BADPOINTER, "MyListDestroy, bad pointer check", 1);
    
    if (testAmount == testOkAmount) {
        printf("All %d test ok\n", testAmount);
    } else {
        printf("Some of tests failed\n");
        printf("Tests amount %d\n", testAmount);
        printf("Tests ok: %d\n", testOkAmount);
        printf("Tests failed: %d\n", testFailAmount);
    }
}
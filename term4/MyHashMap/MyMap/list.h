#ifndef __MYLIST_H
#define __MYLIST_H

typedef struct Node Node_t;
typedef struct MyList MyList_t;
typedef enum {MYLIST_OK, MYLIST_BADPOINTER, MYLIST_NOSUCHITEM} MyListErrorsEnum;
MyListErrorsEnum MyListError;

#define KEYSIZE 256
#define ITEMSIZE 256

struct Node {
    char* key;
    char* item;
    Node_t* next;
};

struct MyList {
    Node_t* head;
};

void MyListCreate(MyList_t* listVar);
void MyListDestroy(MyList_t* listVar);
int MyListAppend(MyList_t* listVar, char* keyVal, char* itemVal);
char* MyListPop(MyList_t* listVar);
char* MyListGetItem(MyList_t* listVar, char* keyVal);
int MyListIsEmpty(MyList_t* listVar);
void MyListPrint(MyList_t* listVar);
int MyListRemoveByKey(MyList_t* listVar, char* keyVal);
void MyListSortByKey(MyList_t* listVar);
int MyListCountItems(MyList_t* listVar);
void MyListDump();
int MyListOk(MyList_t* listVar);

#endif


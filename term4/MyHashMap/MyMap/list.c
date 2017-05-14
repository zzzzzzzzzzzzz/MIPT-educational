#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <errno.h>


void MyListCreate(MyList_t* listVar) {
    listVar->head = (Node_t*)malloc(sizeof(Node_t));
    if (listVar->head == NULL) {
        printf("ListCreate: couldn't allocate memory for head\n");
        exit(errno);
    }    
    MyListError = MYLIST_OK;
    listVar->head->next = NULL;
    listVar->head->key = (char*)calloc(KEYSIZE, 1);
    if (listVar->head->key == NULL) {
        printf("ListCreate: couldn't allocate memory for head->key\n");
        exit(errno);
    }
    listVar->head->item = (char*)calloc(ITEMSIZE, 1);
    if (listVar->head->item == NULL) {
        printf("ListCreate: couldn't allocate memory for head->item\n");
        exit(errno);
    }
}

void MyListDestroy(MyList_t* listVar) {
    if (!MyListOk(listVar)) {
        MyListDump();
        return;
    }
    while(!MyListIsEmpty(listVar)) {
        free(MyListPop(listVar));
    }
    free(listVar->head->key);
    free(listVar->head->item);
    free(listVar->head);
    listVar->head = NULL;
}

char* MyListPop(MyList_t* listVar) {
    if (!MyListOk(listVar)) {
        MyListDump();
        return NULL;
    }
    Node_t* current = listVar->head->next;
    Node_t* prev = listVar->head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    char* popped = (char*)calloc(strlen(current->item)+1, 1);
    strncpy(popped, current->item, strlen(current->item));
    popped[strlen(current->item)] = '\0';
    prev->next = NULL;
    free(current->key);
    free(current->item);
    free(current);
    current = NULL;
    return popped;
}

int MyListIsEmpty(MyList_t* listVar) {
    if (!MyListOk(listVar)) {
        MyListDump();
        return -1;
    }
    if (listVar->head->next == NULL) {
        return 1;
    } else {
        return 0;
    }
}

int MyListAppend(MyList_t* listVar, char* keyVal, char* itemVal) {
    if (!MyListOk(listVar)) {
        MyListDump();
        return 0;
    }
    Node_t* current = listVar->head;
    while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->key, keyVal) == 0) {
            strncpy(current->item, itemVal, ITEMSIZE-1);
            current->item[strlen(itemVal)] = '\0';
            return 1;
        }
    }
    Node_t* newNode = (Node_t*)malloc(sizeof(Node_t));
    newNode->next = NULL;
    newNode->key = (char*)malloc(KEYSIZE);
    newNode->item = (char*)malloc(ITEMSIZE);
    strncpy(newNode->key, keyVal, KEYSIZE-1);
    strncpy(newNode->item, itemVal, ITEMSIZE-1);
    newNode->key[strlen(keyVal)] = '\0';
    newNode->item[strlen(itemVal)] = '\0';
    current->next = newNode;
    return 1;
}

char* MyListGetItem(MyList_t* listVar, char* keyVal) {
    if (!MyListOk(listVar)) {
        MyListDump();
        return NULL;
    }
    if (!MyListIsEmpty(listVar)) {
        Node_t* current = listVar->head->next;
        while ((strcmp(current->key, keyVal) != 0)) {
            if (current->next == NULL) {
                MyListError = MYLIST_NOSUCHITEM;
                MyListDump();
                return NULL;
            }
            current = current->next;
        }
        return current->item;
    }
    MyListError = MYLIST_NOSUCHITEM;
    MyListDump();
    return NULL;
}

void MyListPrint(MyList_t* listVar) {
    if (!MyListOk(listVar)) {
        MyListDump();
        return;
    }
    Node_t* current = listVar->head;
    do {
        printf("->[%s,%s]", current->key, current->item);
    } while(current->next != NULL && (current = current->next));
}

int MyListRemoveByKey(MyList_t* listVar, char* keyVal) {
    if (!MyListOk(listVar)) {
        MyListDump();
        return 0;
    }
    if (!MyListIsEmpty(listVar)) {
        Node_t* current = listVar->head->next;
        Node_t* prev = listVar->head;
        if (strcmp(current->key, keyVal) == 0) {
            goto END;
        }
        while (current->next != NULL) {
            prev = current;
            current = current->next;
            if (strcmp(current->key, keyVal) == 0) {
                goto END;
            }
        }
        END:prev->next = current->next;
        free(current->key);
        free(current->item);
        free(current);
        current = NULL;
        return 1;
    } else {
        MyListError = MYLIST_NOSUCHITEM;
        MyListDump();
        return 0;
    }
}

int MyListOk(MyList_t* listVar) {
    if (!listVar) {
        MyListError = MYLIST_BADPOINTER;
    }
    
    if (MyListError > 0) {
        return 0;
    } else {
        return 1;
    }
}

void MyListDump() {
    if (MyListError == MYLIST_BADPOINTER) {
        printf("\nListDump: Bad pointer was given!\n");
    }
    
    if (MyListError == MYLIST_NOSUCHITEM) {
        printf("\nListDump: No such item, can't remove!\n");
    }
}

int MyListCountItems(MyList_t* listVar) {
    int count = 0;
    Node_t* current = listVar->head->next;
    while (current != NULL) {
        current = current->next;
        count++;
    }
    return count;
} 

void MyListSortByKey(MyList_t* listVar) {
    if (!MyListIsEmpty(listVar)) {
        int num = MyListCountItems(listVar);
        char* keys[num];
        char* items[num];
        Node_t* current = listVar->head->next;
        for (int i = 0; i < num; i++) {
            keys[i] = current->key;
            items[i] = current->item;
            current = current->next;
        }
         for (int i = 0; i < num-1; i++) {
            int swapped = 0;
             for (int j = 0; j < num-i-1; j++) {
                 if (strcmp(keys[j], keys[j+1]) > 0) {
                     char* tmp = keys[j]; 
                     keys[j] = keys[j+1];
                     keys[j+1] = tmp;
                     tmp = items[j];
                     items[j] = items[j+1];
                     items[j+1] = tmp;
                     swapped = 1;
                 }
             }
             if(!swapped)
                break;
         }
         current = listVar->head->next;
         int i = 0;
         while (current != NULL) {
             current->item = items[i];
             current->key = keys[i];
             i++;
             current = current->next;
         }
    }
}
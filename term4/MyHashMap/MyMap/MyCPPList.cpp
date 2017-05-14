#include "MyCPPList.h"
#include <errno.h>
#include <string.h>

MyCPPList::MyCPPList() :
    head_(new Node_t),
    MyCPPListError_(MYCPPLIST_OK)
{
    head_->next = NULL;
    head_->key = new char [KEYSIZE];
    if (head_->key == NULL) {
        printf("MyCPPListCreate: couldn't allocate memory for head_->key\n");
        exit(errno);
    }
    head_->item = new char [ITEMSIZE];
    if (head_->item == NULL) {
        printf("MyCPPListCreate: couldn't allocate memory for head_->item\n");
        exit(errno);
    }
}

MyCPPList::~MyCPPList()
{
    if (!Ok()) {
        Dump();
        return;
    }
    while(!IsEmpty()) {
        free(Pop());
    }
    delete [] head_->key;
    delete [] head_->item);
    delete head_;
    head_ = NULL;
}

char* MyCPPList::Pop() 
{
    if (!Ok()) {
        Dump();
        return NULL;
    }
    Node_t* current = head_->next;
    Node_t* prev = head_;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    char* popped = new char [strlen(current->item) + 1];
    strncpy(popped, current->item, strlen(current->item));
    popped[strlen(current->item)] = '\0';
    prev->next = NULL;
    delete [] current->key;
    delete [] current->item;
    delete current;
    current = NULL;
    return popped;
}

int MyCPPList::IsEmpty()
{
    if (!Ok()) {
        Dump();
        return -1;
    }
    if (head_->next == NULL) {
        return 1;
    } else {
        return 0;
    }
}

int MyCPPList::Append(char *keyVal, char *itemVal)) 
{
    if (!Ok()) {
        Dump();
        return 0;
    }
    Node_t* current = head_;
    while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->key, keyVal) == 0) {
            strncpy(current->item, itemVal, ITEMSIZE-1);
            current->item[strlen(itemVal)] = '\0';
            return 1;
        }
    }
    Node_t* newNode = new Node_t;
    newNode->next = NULL;
    newNode->key = new char [KEYSIZE];
    newNode->item = new char [ITEMSIZE];
    strncpy(newNode->key, keyVal, KEYSIZE-1);
    strncpy(newNode->item, itemVal, ITEMSIZE-1);
    newNode->key[strlen(keyVal)] = '\0';
    newNode->item[strlen(itemVal)] = '\0';
    current->next = newNode;
    return 1;
}

char* MyCPPList::GetItem(char* keyVal) 
{
    if (!Ok()) {
        Dump();
        return NULL;
    }
    if (!IsEmpty()) {
        Node_t* current = head_->next;
        while ((strcmp(current->key, keyVal) != 0)) {
            if (current->next == NULL) {
                MyCPPListError_ = MYCPPLIST_NOSUCHITEM;
                Dump();
                return NULL;
            }
            current = current->next;
        }
        return current->item;
    }
    MyCPPListError_ = MYCPPLIST_NOSUCHITEM;
    Dump();
    return NULL;
}

void MyCPPList::Print()
{
    if (!Ok()) {
        Dump();
        return;
    }
    Node_t* current = head_;
    do {
        printf("->[%s,%s]", current->key, current->item);
    } while(current->next != NULL && (current = current->next));
}

int MyCPPList::RemoveByKey(char* keyVal)
{
    if (!Ok()) {
        Dump();
        return 0;
    }
    if (!IsEmpty()) {
        Node_t* current = head_->next;
        Node_t* prev = head_;
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
        delete [] current->key;
        delete [] current->item;
        delete [] current;
        current = NULL;
        return 1;
    } else {
        MyCPPListError_ = MYCPPLIST_NOSUCHITEM;
        Dump();
        return 0;
    }
}

int MyCPPList::Ok()
{
    if (!head_) {
        MyCPPListError = MYCPPLIST_BADPOINTER;
    }
    
    if (MyCPPListError > 0) {
        return 0;
    } else {
        return 1;
    }
}

void MyCPPList::Dump()
{
    if (MyCPPListError == MYCPPLIST_BADPOINTER) {
        printf("\nMyCPPListDump: Bad pointer was given!\n");
    }
    
    if (MyCPPListError == MYCPPLIST_NOSUCHITEM) {
        printf("\nMyCPPListDump: No such item, can't remove!\n");
    }
}

int MyCPPList::CountItems()
{
    int count = 0;
    Node_t* current = head_->next;
    while (current != NULL) {
        current = current->next;
        count++;
    }
    return count;
}

void MyCPPList::SortByKey() 
{
    if (!IsEmpty()) {
        int num = CountItems();
        char* keys[num];
        char* items[num];
        Node_t* current = head_->next;
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
         current = head_->next;
         int i = 0;
         while (current != NULL) {
             current->item = items[i];
             current->key = keys[i];
             i++;
             current = current->next;
         }
    }
}
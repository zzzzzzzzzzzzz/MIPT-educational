#ifndef MYCPPLIST_H
#define MYCPPLIST_H

typedef struct Node Node_t;
typedef enum {MYCPPLIST_OK, MYCPPLIST_BADPOINTER, MYCPPLIST_NOSUCHITEM} MyCPPListErrorsEnum;

#define KEYSIZE 256
#define ITEMSIZE 256

struct Node {
    char* key;
    char* item;
    Node_t* next;
};


class MyCPPList
{
public:
    MyCPPList();
    ~MyCPPList();
private:
    int Append(char *keyVal, char *itemVal);
    char* Pop();
    char* GetItem(char* keyVal);
    int IsEmpty();
    void Print();
    int RemoveByKey(char* keyVal);
    void SortByKey();
    int CountItems();
    void Dump();
    int Ok();
    
    MyCPPListErrorsEnum MyCPPListError_;
    Node_t* head_;
};

#endif // MYCPPLIST_H

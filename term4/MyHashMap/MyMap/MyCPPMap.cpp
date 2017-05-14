#include "MyCPPMap.h"


void HashMap::Add(string key, string value)
{
    long long unsigned hash = HashFunction(key);
    Node_t toAdd;
    toAdd.key = key;
    toAdd.item = value;
    try {
        for (list<Node_t>::iterator it = table_[hash].listOfItems.begin(); it != table_[hash].listOfItems.end(); ++it) {
            if ((*it).key.compare(key) == 0) {
                (*it).item = toAdd.item;
                return;
            }
        }
        table_[hash].listOfItems.push_back(toAdd);
    }
    catch(const exception& ec) {
        cerr << "Bad push: " << ec.what() << endl;
        MyCPPMapAddError ex;
        throw ex;
    }
}

int HashMap::Remove(string key)
{
    long long unsigned hash = HashFunction(key);
    list<Node_t>::iterator it = table_[hash].listOfItems.begin();
    while (it != table_[hash].listOfItems.end()) {
        if ((*it).key.compare(key) == 0) {
            it = table_[hash].listOfItems.erase(it);
            return 1;
        } else {
            ++it;
        }
    }
    return 0;
}

string HashMap::GetByKey(string key) 
{
    long long unsigned hash = HashFunction(key);
    list<Node_t>::iterator it = table_[hash].listOfItems.begin();
    while (it != table_[hash].listOfItems.end()) {
        if ((*it).key.compare(key) == 0) {
            return (*it).item;
        } else {
            ++it;
        }
    }
}

void HashMap::Print()
{
    for (int i = 0; i < tablesize_; i++) {
        printf(" [%d] ===>", i);
        for (list<Node_t>::iterator it = table_[i].listOfItems.begin(); it != table_[i].listOfItems.end(); ++it) {
            printf("[%s,%s]->", (*it).key, (*it).item);
        }
        printf("|\n");
    }
}



MyCPPMap::MyCPPMap()
{
    try {
        prime_ = PRIME;
        tablesize_ = prime_;
        table_ = new TableRow_t [tablesize_];
        if (table_ == nullptr) {
            MyCPPMapBadAlloc er;
            throw er;
        }
    }
    catch (exception& er) {
        cerr << "bad_alloc caught: " << er.what() << endl;
        exit(errno);
    }
}

MyCPPMap::MyCPPMap(long long unsigned myprime)
{
    try {
        if (myprime < 10) {
            MyCPPMapBadPrime er;
            throw er;
        } else {
            tablesize_ = myprime; 
        }
    }
    catch (exception& er) {
        cerr << er.what() << '\n';
        throw er;
    }
    try {
        table_ = new TableRow_t [tablesize_];
    }
    catch (exception& er) {
        cerr << "bad_alloc caught: " << er.what() << endl;
        exit(errno);
    }
}

MyCPPMap::~MyCPPMap()
{
    delete [] table_;
    table_ = nullptr;
}

long long unsigned MyCPPMap::HashFunction(string key)
{
    long long unsigned sum = 0;
    for (int i = 0; i < key.length(); i++) {
        sum += key[i];
    }
    return sum % prime_;
}
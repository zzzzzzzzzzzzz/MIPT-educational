#ifndef MYCPPMAP_H
#define MYCPPMAP_H

#define KEYSIZE 256
#define ITEMSIZE 256
#define PRIME 257
#define SCALECONSTANT 2

using namespace std;

#include <list>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node_t;
typedef struct TableRow TableRow_t;

class MyCPPMapBadAlloc: public exception
{
  virtual const char* what() const throw()
  {
    return "\nCouldn't allocate memory for table\n";
  }
};

class MyCPPMapAddError: public exception
{
  virtual const char* what() const throw()
  {
    return "\nError while adding an element to list\n";
  }
};

class MyCPPMapBadPrime: public exception
{
  virtual const char* what() const throw()
  {
    return "choose prime number bigger than 10";
  }
};

typedef struct Node Node_t;

struct Node {
    string key;
    string item;
};

struct TableRow {
    list<Node_t> listOfItems; 
};

// Base class
class HashMap 
{
    
protected:
    TableRow_t* table_;
    long long unsigned tablesize_;
    long long unsigned prime_;
    
public:
    void Add(string key, string value);
    void Print();
    int Remove(string key);
    string GetByKey(string key);
    
private:
    virtual long long unsigned HashFunction(string key) = 0;
};

//implementation
class MyCPPMap : public HashMap
{
public:
    MyCPPMap();
    MyCPPMap(long long unsigned myprime);
    ~MyCPPMap();
private:
    long long unsigned HashFunction(string key);
};

#endif // MYCPPMAP_H

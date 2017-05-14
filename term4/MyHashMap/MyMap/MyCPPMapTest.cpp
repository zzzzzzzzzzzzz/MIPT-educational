#include "MyCPPMap.h"

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
         \
    } \



int main(int argc, char **argv)
{
    MyCPPMap map;
    testAmount = 0;
    testOkAmount = 0;
    testFailAmount = 0;
    
    map.Add("key one", "one");
    CHECK(map.GetByKey("key one").compare("one") == 0, "MyMapAdd, adding item check", 1);
    map.Add("key one", "one changed");
    CHECK(map.GetByKey("key one").compare("one changed") == 0, "MyMapAdd, reassignment item check", 1);
    map.Add("key owt", "more to this hashvalue");
    CHECK(map.GetByKey("key owt").compare("more to this hashvalue") == 0, "MyMapGetByKey, getting by key test", 1);
    map.Add("key three", "three");
    map.Add("key there", "there");
    CHECK(map.GetByKey("key there").compare("there") == 0, "MyMapGetByKey, getting elemnt which is not in the beginning", 1);
    map.Print();
    map.Remove("key one");
    map.Remove("key there");
    CHECK(map.Remove("no key") == 0, "Trying to remove element that doesn't exist", 1);
    
    /*testing another constructor*/
    try {
        MyCPPMap m(8);
    }
    catch(exception& er) {
        CHECK(1, "Checking bad prime number", 1);
    }
    //now create normal map
    MyCPPMap m(257);
    CHECK(&m, "checking m is ok", 1);
    
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
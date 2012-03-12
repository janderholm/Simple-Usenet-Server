#include <iostream>
#include <vector>
#include "inmemorydb.h"

//using namespace std;
using namespace sus;

int
main()
{
    InMemoryDatabase db;
    db.createNewsgroup("A");
    return 0;
}

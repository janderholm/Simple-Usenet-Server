#include <iostream>
#include <vector>
#include "inmemorydb.h"

using namespace std;
using namespace sus;

int
main()
{
    InMemoryDatabase db;
    db.createNewsgroup("A");
    auto v = db.getNewsgroups();
    for(auto it = v.begin(); it != v.end(); ++it){
        cout << it->name << endl;
    }
    return 0;
}

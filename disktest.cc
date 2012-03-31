#include <iostream>
#include <vector>
#include "diskdb.h"

using namespace std;
using namespace sus;

int
main()
{


    DiskDatabase db;

    db.createNewsgroup("lol");
    db.createNewsgroup("lolol2");
    db.createNewsgroup("lolol3");

    vector<Newsgroup> vec = db.getNewsgroups();
    for(auto it = vec.begin(); it != vec.end(); ++it){
        cout << it->ident << " " << it->name << endl;
    }

    

    return 0;
}

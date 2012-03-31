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
    //db.deleteNewsgroup(4);
    
    db.createArticle(5, "the new article",
                "the german", "body ahoe!");
    db.getArticles(5);

    vector<Newsgroup> vec = db.getNewsgroups();
    for(auto it = vec.begin(); it != vec.end(); ++it){
        cout << it->ident << " " << it->name << endl;
    }

    vector<Article> vec1 = db.getArticles(5);
    for(auto it = vec1.begin(); it != vec1.end(); ++it){
        cout << it->ident << " " << it->title << endl;
    }

    

    return 0;
}

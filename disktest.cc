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

    db.deleteArticle(5, 2);

    db.getArticles(5);

    const Article* a = db.getArticle(5,1);
    cout << "i got " << a->ident << " " << a->title << " " << a->author;
    //free(a);

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

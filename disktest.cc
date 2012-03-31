#include <iostream>
#include <vector>
#include "diskdb.h"
#include<sys/stat.h>
#include<sys/types.h>

using namespace std;
using namespace sus;

int
main()
{
    string dirname("diskdb"); 
    mkdir(dirname.c_str(),0777);

    DiskDatabase db;
    cout << "create newsgroups" << endl;

    db.createNewsgroup("newsgroup 1");
    db.createNewsgroup("newsgroup 2");
    db.createNewsgroup("newsgroup 3");

    cout << endl <<  "print newsgroups" << endl;

    vector<Newsgroup> vec = db.getNewsgroups();
    for(auto it = vec.begin(); it != vec.end(); ++it){
        cout << it->ident << " " << it->name << endl;
    }

    cout << endl << "create articles" << endl;

    db.createArticle(1, "first article",
                "author", "body");
    db.createArticle(1, "second article",
                "author", "body");
    db.createArticle(2, "first article",
                "author", "body");
    db.createArticle(2, "second article",
                "author", "body");
    db.createArticle(3, "first article",
                "author", "body");
    db.createArticle(3, "second article",
                "author", "body");

    cout << "print articles" << endl;
    cout << endl << "group 1" << endl;
    vector<Article> vec1 = db.getArticles(1);
    for(auto it = vec1.begin(); it != vec1.end(); ++it){
        cout << it->ident << " " << it->title << endl;
    }
    
    cout << endl << "group 2" << endl;
    vec1 = db.getArticles(2);
    for(auto it = vec1.begin(); it != vec1.end(); ++it){
        cout << it->ident << " " << it->title << endl;
    }

    cout << endl << "group 3" << endl;
    vec1 = db.getArticles(3);
    for(auto it = vec1.begin(); it != vec1.end(); ++it){
        cout << it->ident << " " << it->title << endl;
    }

    cout << endl << "get an article" << endl;

    const Article* a = db.getArticle(2,1);
    cout << a->ident << " " << a->title << " " << a->author << endl;

    cout << "delete all newsgroups" << endl;
    
    db.deleteNewsgroup(1);
    db.deleteNewsgroup(2);
    db.deleteNewsgroup(3);

    //free(a);

    return 0;
}

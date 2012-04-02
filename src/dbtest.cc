#include <iostream>
#include <vector>
#include "inmemorydb.h"

using namespace std;
using namespace sus;

int
main()
{
    InMemoryDatabase db;
    cout << "Add A" << endl;
    db.createNewsgroup("A");

    auto newsgroup = *db.getNewsgroups().begin();
    cout << "Should be A: " << newsgroup.name << endl;
    if (newsgroup.name != "A") {
        return 1;
    }

    cout << "delete A" << endl;
    db.deleteNewsgroup(newsgroup.ident);
    cout << "db should be empty: ";
    auto ngs = db.getNewsgroups();
    if (ngs.begin() == ngs.end()) {
        cout << "empty" << endl;
    } else {
        cout << "not empty" << endl;
        return 1;
    }


    db.createNewsgroup("A");

    if(db.existsNewsgroup(1000)) {
        cout << "Nonexistand should not exist" << endl;
        return 1;
    }

    if(db.deleteNewsgroup(1000)) {
        cout << "Delete nonexistant should not be possible" << endl;
        return 1;
    }

    newsgroup = *db.getNewsgroups().begin();
    db.createArticle(newsgroup.ident, "TEST", "ME", "This is a test");
    auto arts = db.getArticles(newsgroup.ident);
    auto art = *arts.begin();
    art = db.getArticle(newsgroup.ident, art.ident);
    cout << "Should be TEST: ";
    cout << art.title << endl;
    if (art.title != "TEST") {
        return 1;
    }

    db.deleteArticle(newsgroup.ident, art.ident);
    arts = db.getArticles(newsgroup.ident);
    cout << "Should be empty: ";
    if (arts.begin() == arts.end()) {
        cout << "empty" << endl;
    } else {
        cout << "not empty" << endl;
        return 1;
    }


    return 0;
}

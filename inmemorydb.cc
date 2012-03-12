#include "inmemorydb.h"
#include "vector"


using namespace std;
using namespace sus;

// TODO: A more C++ish way of doing it.
static newsident = 0;

vector<Newsgroup>
InMemoryDatabase::getNewsgroups()
{
    return vector<Newsgroup> v;
}

void
InMemoryDatabase::createNewsgroup(string name)
{
    // TODO: string reference.
    MemoryNewsgroup n;
    n->name = name;
    n->ident = newsident++;
    newsgroups.push_back(n); 
}


void
InMemoryDatabase::deleteNewsgroup(unsigned long news)
{
}


const Article&
InMemoryDatabase::getArticle(unsigned long newsIdent, unsigned long artIdent)
{
    vector<Article> articles = getArticles(newsIdent);
    vector<Article> :: const_iterator itr = articles.begin();
    for(;itr != articles.end(); ++itr){
        if(itr->ident == artIdent)
            return *itr;    
    }
    return 0;
}


#include <algorithm>

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
InMemoryDatabase::deleteNewsgroup(unsigned long newsIdent)
{
    auto it = find_if(newsgroups.begin(), newsgroups.end(),
            [&newsIdent](MemoryNewsgroup& n) { return newsIdent == n.ident; });
    newsgroups.erase(it);
}

void
InMemoryDatabase::createArticle(unsigned long newsIdent, const Article& article)
{
    vector<Article> articles = getArticles(newsIdent);
    articles.push_back(article);
}

void
InMemoryDatabase::deleteArticle(unsigned long newsIdent, unsigned long artIdent)
{
    vector<Article> articles = getArticles(newsIdent);
    auto it = find_if(articles.begin(), articles.end(),
        [&artIdent](Article& a) {return a.ident == artIdent;});
    articles.erase(it);
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


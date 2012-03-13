#include <algorithm>

#include "inmemorydb.h"
#include <vector>


using namespace std;
using namespace sus;

// TODO: A more C++ish way of doing it.
static long newsident = 0;



vector<Newsgroup>
InMemoryDatabase::getNewsgroups()
{
    return vector<Newsgroup>(newsgroups.begin(),newsgroups.end());
}


void
InMemoryDatabase::createNewsgroup(const string& name)
{
    // TODO: string reference.
    MemoryNewsgroup n;
    n.name = name;
    n.ident = newsident++;
    newsgroups.push_back(n); 
}


void
InMemoryDatabase::deleteNewsgroup(unsigned long newsIdent)
{
    auto it = find_if(newsgroups.begin(), newsgroups.end(),
            [&newsIdent](MemoryNewsgroup& n) { return newsIdent == n.ident; });
    newsgroups.erase(it);
}

vector<Article>
InMemoryDatabase::getArticles(unsigned long newsIdent)
{
    auto it = find_if(newsgroups.begin(), newsgroups.end(),
        [&newsIdent](MemoryNewsgroup& g) {return g.ident == newsIdent;});
    return it->articles;
    //TODO: what if it doesn't exist?
}

void
InMemoryDatabase::createArticle(unsigned long newsIdent, const string& title,
                const string& author, const string& body)
{   
    Article a;
    a.title = title;
    a.author = author;
    a.body = body;
    vector<Article> articles = getArticles(newsIdent);
    articles.push_back(a);
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
    //TODO:fix return statement for non existing article
}


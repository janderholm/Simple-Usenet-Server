#include <algorithm>

#include "inmemorydb.h"
#include <vector>


using namespace std;
using namespace sus;

// TODO: A more C++ish way of doing it.
//static int newsident = 0;


InMemoryDatabase::InMemoryDatabase() : newsident(1) {}


vector<Newsgroup>
InMemoryDatabase::getNewsgroups()
{
    return vector<Newsgroup>(newsgroups.begin(),newsgroups.end());
}


bool
InMemoryDatabase::createNewsgroup(const string& name)
{
    // TODO: string reference.
    auto it = find_if(newsgroups.begin(), newsgroups.end(),
        [&name](MemoryNewsgroup& n) { return name == n.name; });

    if (it != newsgroups.end()) {
        // Already exist;
        return false;
    }
  
    MemoryNewsgroup n;
    n.name = name;
    n.ident = newsident++;
    n.artident = 1;
    newsgroups.push_back(n); 
    return true;
}


bool
InMemoryDatabase::deleteNewsgroup(unsigned int newsIdent)
{
    auto it = find_if(newsgroups.begin(), newsgroups.end(),
            [&newsIdent](MemoryNewsgroup& n) { return newsIdent == n.ident; });

    if (it == newsgroups.end()) {
        // Does not exist
        return false;
    }

    newsgroups.erase(it);
    return true;
}

bool
InMemoryDatabase::existsNewsgroup(unsigned int newsIdent)
{
    auto it = find_if(newsgroups.begin(), newsgroups.end(),
            [&newsIdent](MemoryNewsgroup& n) { return newsIdent == n.ident; });

    return it != newsgroups.end();
}

vector<Article>
InMemoryDatabase::getArticles(unsigned int newsIdent)
{
    auto it = find_if(newsgroups.begin(), newsgroups.end(),
        [&newsIdent](MemoryNewsgroup& g) {return g.ident == newsIdent;});
    return it->articles;

    //TODO: We need some kind exit status here...
}

bool
InMemoryDatabase::createArticle(unsigned int newsIdent, const string& title,
                const string& author, const string& body)
{   
     auto it = find_if(newsgroups.begin(), newsgroups.end(),
            [&newsIdent](MemoryNewsgroup& g) {return g.ident == newsIdent;});

    if (it == newsgroups.end()) {
        return false;
    }

    Article a;
    a.title = title;
    a.author = author;
    a.body = body;
    a.ident = it->artident++;

    it->articles.push_back(a);
    return true;
}

bool
InMemoryDatabase::deleteArticle(unsigned int newsIdent, unsigned int artIdent)
{

    auto it = find_if(newsgroups.begin(), newsgroups.end(),
            [&newsIdent](MemoryNewsgroup& g) {return g.ident == newsIdent;});

    if (it == newsgroups.end()) {
        return false;
    }

    auto artit = find_if(it->articles.begin(), it->articles.end(),
           [&artIdent](Article& a) {return a.ident == artIdent;});

    if (artit == it->articles.end()) {
        return false;
    }

    it->articles.erase(artit);
    return true;
}


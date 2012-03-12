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

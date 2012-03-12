#include "inmemorydb.h"

using namespace std;
using namespace sus;

const Article& InMemoryDatabase::getArticle(unsigned long newsIdent, unsigned long artIdent){
    vector<Article> articles = getArticles(newsIdent);
    vector<Article> :: const_iterator itr = articles.begin();
    for(;itr != articles.end(); ++itr){
        if(itr->ident == artIdent)
            return *itr;    
    }
    return 0;
}

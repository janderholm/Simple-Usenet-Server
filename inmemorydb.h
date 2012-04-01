#ifndef INMEMORYDB_H
#define INMEMORYDB_H

#include "susdb.h"
#include <vector>

namespace sus
{
    class MemoryNewsgroup : public Newsgroup
    {
        public: vector<Article> articles;
    };

    class InMemoryDatabase : public DatabaseInterface
    {
        public:
            vector<Newsgroup> getNewsgroups();

            bool createNewsgroup(const string& name);

            bool deleteNewsgroup(unsigned long newsIdent);

            bool existsNewsgroup(unsigned long newsIdent);

            vector<Article> getArticles(unsigned long newsIdent);
            bool createArticle(unsigned long newsIdent, const string& title,
                const string& author, const string& body);
            bool deleteArticle(unsigned long newsIdent, unsigned long artIdent);
            const Article* getArticle(unsigned long newsIdent, unsigned long artIdent);
        private:
            vector<MemoryNewsgroup> newsgroups;
    };
}

#endif

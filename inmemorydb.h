#ifndef INMEMORYDB_H
#define INMEMORYDB_H

#include "susdb.h"
#include <vector>

namespace sus
{
    class MemoryNewsgroup : public Newsgroup
    {
        public:
            vector<Article> articles;
            unsigned int artident;
    };

    class InMemoryDatabase : public DatabaseInterface
    {
        public:
            InMemoryDatabase();

            vector<Newsgroup> getNewsgroups();

            bool createNewsgroup(const string& name);

            bool deleteNewsgroup(unsigned int newsIdent);

            bool existsNewsgroup(unsigned int newsIdent);

            vector<Article> getArticles(unsigned int newsIdent);
            bool createArticle(unsigned int newsIdent, const string& title,
                const string& author, const string& body);
            bool deleteArticle(unsigned int newsIdent, unsigned int artIdent);
            Article getArticle(unsigned int newsIdent, unsigned int artIdent);
        private:
            vector<MemoryNewsgroup> newsgroups;
            unsigned int newsident;
    };
}

#endif

#ifdef INMEMORYDB_H
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
            void createNewsgroup(const string& name);
            void deleteNewsgroup(unsigned long newsIdent);
            vector<Article> getArticles(unsigned long newsIdent);
            void createArticle(unsigned long newsIdent, const Article& article);
            void deleteArticle(unsigned long newsIdent, unsigned long artIdent);
            const Article& getArticle(unsigned long newsIdent, unsigned long artIdent);
        private:
            vector<MemoryNewsgroup> newsgroups;
    };
}

#endif

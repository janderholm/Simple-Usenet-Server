#ifdef INMEMORYDB_H
#define INMEMORYDB_H

#include "susdb.h"
#include <vector>
namespace sus {
    class MemoryNewsgroup : public Newsgroup {
        public: vector<Article> articles;
    }

    class InMemoryDatabase : public DatabaseInterface
    {
        public:
            vector<Newsgroup> getNewsgroups();
            void createNewsgroup(string name);
            void deleteNewsgroup(unsigned long newsIdent);
            vector<Article> getArticles(unsigned long newsIdent);
            void createArticle(unsigned long newsIdent);
            void deleteArticle(unsigned long artIdent);
            void getArticle(unsigned long newsIdent, unsigned long artIdent);
        private:
            vector<MemoryNewsgroup> newsgroups;
    }

}

#endif

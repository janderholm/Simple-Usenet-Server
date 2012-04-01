#ifndef DISKDB_H
#define DISKDB_H

#include "susdb.h"
#include <vector>

namespace sus
{
    class DiskDatabase : public DatabaseInterface
    {
        public:
            vector<Newsgroup> getNewsgroups();
            bool createNewsgroup(const string& name);
            bool deleteNewsgroup(unsigned long newsIdent);
            vector<Article> getArticles(unsigned long newsIdent);
            bool createArticle(unsigned long newsIdent, const string& title,
                const string& author, const string& body);
            bool deleteArticle(unsigned long newsIdent, unsigned long artIdent);
            const Article* getArticle(unsigned long newsIdent, unsigned long artIdent);
            string readFile(string file);
    };
}

#endif

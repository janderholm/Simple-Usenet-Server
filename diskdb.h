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

            bool deleteNewsgroup(unsigned int newsIdent);

            vector<Article> getArticles(unsigned int newsIdent);

            bool createArticle(unsigned int newsIdent, const string& title,
                const string& author, const string& body);

            bool deleteArticle(unsigned int newsIdent, unsigned int artIdent);

            bool existsNewsgroup(unsigned int newsIdent);

            string readFile(string file);
    };
}

#endif

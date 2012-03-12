#ifdef SUSDB_H
#define SUSDB_H

#include <ctime>
#include <string>
#include <vector>

namespace sus {
    struct Article
    {
        string title;
        string author;
        string body;
        unsigned long ident;
    }

    struct Newsgroup
    {
        string name;
        unsigned long ident;
    }

    class DatabaseInterface
    {
        public:
            // Create newsgroup with 
            virtual vector<Newsgroup> getNewsgroups() = 0;
            virtual void createNewsgroup(string name) = 0;
            virtual void deleteNewsgroup(unsigned long newsIdent) = 0;
            virtual vector<Article> getArticles(unsigned long newsIdent) = 0;
            virtual void createArticle(unsigned long newsIdent) = 0;
            virtual void deleteArticle(unsigned long artIdent) = 0;
            virtual const Article& getArticle(unsigned long newsIdent, unsigned long artIdent) = 0;
    }
}

#endif

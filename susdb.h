#ifdef SUSDB_H
#define SUSDB_H

#include <ctime>
#include <string>
#include <map>

namespace sus {
    struct Article {
        string title;
        string author;
        string body;
        unsigned long ident;
    }

    struct Newsgroup {
        string name;
        unsigned long ident;
    }


    class DatabaseInterface {
        public:
            // Create newsgroup with 
            virtual List<Newsgroup> getNewsGroups() = 0;
            virtual void createNewsGroup(string name) = 0;
            virtual void deleteNewsGroup(unsigned long newsIdent) = 0;
            virtual List<Article> getArticles(unsigned long newsIdent) = 0;
            virtual void createArticle(unsigned long newsIdent) = 0;
            virtual void deleteArticle(unsigned long artIdent) = 0;
            virtual void getArticle(unsigned long newsIdent, unsigned long artIdent) = 0;
    }
}

#endif

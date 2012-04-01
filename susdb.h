#ifndef SUSDB_H
#define SUSDB_H

#include <ctime>
#include <string>
#include <vector>
using namespace std;
namespace sus
{
    struct Article
    {
        string title;
        string author;
        string body;
        unsigned long ident;
    };

    struct Newsgroup
    {
        string name;
        unsigned long ident;
    };

    class DatabaseInterface
    {
        public:
            // TODO: Consider returning dummy object with begin, end iterators.
            //virtual ~DatabaseInterface() = 0;

            /* Return list of newsgroups */
            virtual vector<Newsgroup> getNewsgroups() = 0;

            /* Create new newsgroup, return false if already exist */
            virtual bool createNewsgroup(const string& name) = 0;

            /* Delete newsgroup, return false if not exist */
            virtual bool deleteNewsgroup(unsigned long newsIdent) = 0;

            /* Return true if newsgroup exist */
            virtual bool existsNewsgroup(unsigned long newsIdent) = 0;

            /* Return list of articles in newsgroup */
            virtual vector<Article> getArticles(unsigned long newsIdent) = 0;

            /* Create article in newsgroup identified by newsIdent
             * Return false of newsgroup does not exist */
            virtual bool createArticle(unsigned long newsIdent, const string& title,
                const string& author, const string& body) = 0;

            /* Delete article in newsident, return false if not possible */
            virtual bool deleteArticle(unsigned long newsIdent, unsigned long artIdent) = 0;

            /* Return article */
            virtual Article getArticle(unsigned long newsIdent, unsigned long artIdent) = 0;
    };
}

#endif

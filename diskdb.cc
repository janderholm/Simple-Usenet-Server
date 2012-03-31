

#include <algorithm>
#include "diskdb.h"
#include <vector>
#include <dirent.h>
#include <string.h>
#include <iostream> 
#include <fstream>
#include <sstream>
#include<sys/stat.h>
#include<sys/types.h>

using namespace std;
using namespace sus;

class DirIterator {
    public:
        DirIterator(DIR* d) : dirp(d) { 
            if(d == 0)
                entry = 0;
            else
                entry = (operator++()).entry;
        }

        string operator*() const { return entry->d_name; }

        DirIterator& operator++() {
            entry = readdir(dirp);
            if(entry != 0 && (strcmp(entry->d_name,".") == 0 ||
                             strcmp(entry->d_name,"..") == 0 ||
                             strcmp(entry->d_name,"name") == 0)){
                operator++();
            }
            return *this;
        }

        bool operator!=(const DirIterator& rhs) { return entry != rhs.entry; }

    private:
        DIR* dirp;
        dirent* entry;
};

class Directory {
    public:
        typedef DirIterator iterator;
        Directory(const char* name) : dirp(opendir(name)) {}
        ~Directory() { closedir(dirp); }
        iterator begin() const { return iterator(dirp); }
        iterator end() const { return iterator(0); }
    private:
        DIR* dirp;
};


// TODO: A more C++ish way of doing it.
static long newsident = 0;
static long artident = 0;
string dbname = "diskdb";

vector<Newsgroup>
DiskDatabase::getNewsgroups()
{
    // TODO: paths should atleast be global constants
    
    Directory dir(dbname.c_str());
    vector<Newsgroup> groups;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        Newsgroup n;
        n.name = readFile(dbname + "/" + *it + "/" + "name");
        istringstream(*it) >> n.ident;
        groups.push_back(n);
    }
    sort(groups.begin(),groups.end(),
        [](const Newsgroup& a,const Newsgroup& b) {return a.ident < b.ident;});

    return groups;
}


bool
DiskDatabase::createNewsgroup(const string& name)
{
    //TODO error handling
    Directory dir(dbname.c_str());
    //find the max ident
    //TODO perhaps it is always the first/last
    unsigned long maxIdent = 0;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        unsigned long ident;
        istringstream(*it) >> ident;
        if(readFile(dbname + "/" + *it + "/" + "name") == name){
            return false; //already exist        
        }
        if(ident > maxIdent){
            maxIdent = ident;
        }
    }

    ++maxIdent;

    stringstream out;
    out << maxIdent;
    string ident = out.str();

    if(mkdir((dbname + "/" + ident).c_str(),0777)==-1){
        return false;
    }

    ofstream write (dbname + "/" + ident + "/name");
    write << name << endl;
    write.close();

    return true;
}


bool
DiskDatabase::deleteNewsgroup(unsigned long newsIdent)
{
    stringstream out;
    out << newsIdent;
    string ident = out.str();
    Directory newsGroups((dbname + "/" + ident).c_str());
    for(auto it = newsGroups.begin(); it != newsGroups.end(); ++it){
        remove((dbname + "/" + ident + "/" + *it + "/body").c_str());
        remove((dbname + "/" + ident + "/" + *it + "/author").c_str());
        remove((dbname + "/" + ident + "/" + *it + "/title").c_str());
        rmdir((dbname + "/" + ident + "/" + *it).c_str());
        //returns -1 even if it worked ?!
        /*if(rmdir((dbname + "/" + ident + "/" + *it).c_str()) == -1){
            cerr << "Error: directory could not be removed" << " " << dbname + "/" + ident + "/" + *it  << endl;
            return false;
        }*/
    }
    remove((dbname + "/" + ident + "/name").c_str());
    rmdir((dbname + "/" + ident).c_str());
    return true;
}

vector<Article>
DiskDatabase::getArticles(unsigned long newsIdent)
{
    stringstream out;
    out << newsIdent;
    string ident = out.str();
    vector<Article> articles; 
    Directory dir((dbname + "/" + ident).c_str());
    for(auto it = dir.begin(); it != dir.end(); ++it){
        Article a;
        a.title = readFile(dbname + "/" + ident + "/" + *it + "/" + "title");
        a.body = readFile(dbname + "/" + ident + "/" + *it + "/" + "body");
        a.author = readFile(dbname + "/" + ident + "/" + *it + "/" + "author");
        istringstream(*it) >> a.ident;
        articles.push_back(a);
    }
    sort(articles.begin(),articles.end(),
        [](const Article& a,const Article& b) {return a.ident < b.ident;});

    return articles;
}

bool
DiskDatabase::createArticle(unsigned long newsIdent, const string& title,
                const string& author, const string& body)
{   

    stringstream sid;
    sid << newsIdent;
    string s_newsIdent = sid.str();

    Directory dir((dbname + "/" + s_newsIdent).c_str());
    //find the max ident
    //TODO perhaps it is always the first/last
    unsigned long maxIdent = 0;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        unsigned long ident;
        istringstream(*it) >> ident;
        if(readFile(dbname + "/" + s_newsIdent + "/" +  *it + "/" + "title") == title){
            return false; //already exist        
        }
        if(ident > maxIdent){
            maxIdent = ident;
        }
    }

    ++maxIdent;

    stringstream out;
    out << maxIdent;
    string ident = out.str();

    if(mkdir((dbname + "/" + s_newsIdent + "/" + ident).c_str(),0777)==-1){
        return false;
    }

    ofstream write1(dbname + "/" + s_newsIdent + "/" + ident + "/title");
    write1 << title << endl;
    ofstream write2(dbname + "/" + s_newsIdent + "/" + ident + "/body");
    write2 << body << endl;
    ofstream write3(dbname + "/" + s_newsIdent + "/" + ident + "/author");
    write3 << author << endl;
    write1.close();
    write2.close();
    write3.close();

    return true;
}

bool
DiskDatabase::deleteArticle(unsigned long newsIdent, unsigned long artIdent)
{
    stringstream out;
    out << newsIdent;
    string s_newsIdent = out.str();
    out.str("");
    out << artIdent;
    string s_artIdent = out.str();

    remove((dbname + "/" + s_newsIdent + "/" + s_artIdent + "/body").c_str());
    remove((dbname + "/" + s_newsIdent + "/" + s_artIdent + "/author").c_str());
    remove((dbname + "/" + s_newsIdent + "/" + s_artIdent + "/title").c_str());
    rmdir((dbname + "/" + s_newsIdent + "/" + s_artIdent).c_str());

    return true;
}

const Article*
DiskDatabase::getArticle(unsigned long newsIdent, unsigned long artIdent)
{
    stringstream out;
    out << newsIdent;
    string s_newsIdent = out.str();
    out.str("");
    out << artIdent;
    string s_artIdent = out.str();

    Directory dir((dbname + "/" + s_newsIdent + "/" + s_artIdent).c_str());

    Article* a = new Article();  // XXX: Need to think about freeing it

    a->title = readFile(dbname + "/" + s_newsIdent + "/" + s_artIdent + "/" + "title");
    a->body = readFile(dbname + "/" + s_newsIdent + "/" + s_artIdent + "/" + "body");
    a->author = readFile(dbname + "/" + s_newsIdent + "/" + s_artIdent + "/" + "author");
    istringstream(s_artIdent) >> a->ident;

    return a;
}

string
DiskDatabase::readFile(string file){
    ifstream indata;
    string result;
    string c;
    indata.open(file);
    if(!indata) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      exit(1);
    }
    string line;
    while ( indata.good() )
    {
        getline(indata,line);
        result += line;
    }
    indata.close();

    return result;
}
 

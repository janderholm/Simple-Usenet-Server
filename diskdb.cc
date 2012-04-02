

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
#include <streambuf>

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
            //ignore some files
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
    unsigned int maxIdent = 0;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        unsigned int ident;
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

    mkdir((dbname + "/" + ident).c_str(),0777);

    ofstream write (dbname + "/" + ident + "/name");
    write << name;
    write.close();

    return true;
}


bool
DiskDatabase::deleteNewsgroup(unsigned int newsIdent)
{
    stringstream out;
    out << newsIdent;
    string ident = out.str();
    
    if(opendir((dbname + "/" + ident).c_str()) == NULL)
        return false; //does not exist

    Directory newsGroups((dbname + "/" + ident).c_str());
    for(auto it = newsGroups.begin(); it != newsGroups.end(); ++it){
        string artPath(dbname + "/" + ident + "/" + *it);
        remove((artPath + "/body").c_str());
        remove((artPath + "/author").c_str());
        remove((artPath + "/title").c_str());
        rmdir(artPath.c_str());
    }
    remove((dbname + "/" + ident + "/name").c_str());
    rmdir((dbname + "/" + ident).c_str());
    return true;
}

vector<Article>
DiskDatabase::getArticles(unsigned int newsIdent)
{
    stringstream out;
    out << newsIdent;
    string ident = out.str();
    vector<Article> articles;

    Directory dir((dbname + "/" + ident).c_str());
    for(auto it = dir.begin(); it != dir.end(); ++it){
        Article a;
        string artPath = dbname + "/" + ident + "/" + *it;
        a.title = readFile(artPath + "/title");
        a.body = readFile(artPath + "/body");
        a.author = readFile(artPath + "/author");
        istringstream(*it) >> a.ident;
        articles.push_back(a);
    }
    sort(articles.begin(),articles.end(),
        [](const Article& a,const Article& b) {return a.ident < b.ident;});

    return articles;
}

bool
DiskDatabase::createArticle(unsigned int newsIdent, const string& title,
                const string& author, const string& body)
{   

    stringstream sid;
    sid << newsIdent;
    string s_newsIdent = sid.str();

    if(opendir((dbname + "/" + s_newsIdent).c_str()) == NULL)
        return false; //does not exist

    Directory dir((dbname + "/" + s_newsIdent).c_str());
    //find the max ident
    //TODO perhaps it is always the first/last
    unsigned int maxIdent = 0;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        unsigned int ident;
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
    string artPath = dbname + "/" + s_newsIdent + "/" + ident;

    mkdir(artPath.c_str(),0777);

    ofstream write1(artPath + "/title");
    write1 << title;
    ofstream write2(artPath + "/body");
    write2 << body;
    ofstream write3(artPath + "/author");
    write3 << author;
    write1.close();
    write2.close();
    write3.close();

    return true;
}

bool
DiskDatabase::deleteArticle(unsigned int newsIdent, unsigned int artIdent)
{
    stringstream out;
    out << newsIdent;
    string s_newsIdent = out.str();
    out.str("");
    out << artIdent;
    string s_artIdent = out.str();

    string artPath = dbname + "/" + s_newsIdent + "/" + s_artIdent;

    if(opendir(artPath.c_str()) == NULL)
        return false; //does not exist

    remove((artPath + "/body").c_str());
    remove((artPath + "/author").c_str());
    remove((artPath + "/title").c_str());
    rmdir(artPath.c_str());

    return true;
}

Article
DiskDatabase::getArticle(unsigned int newsIdent, unsigned int artIdent)
{
    stringstream out;
    out << newsIdent;
    string s_newsIdent = out.str();
    out.str("");
    out << artIdent;
    string s_artIdent = out.str();

    string artPath = dbname + "/" + s_newsIdent + "/" + s_artIdent;    

    if(opendir(artPath.c_str()) == NULL)
        return Article(); //does not exist //XXX return?

    Directory dir(artPath.c_str());

    Article a;
    a.title = readFile(artPath + "/title");
    a.body = readFile(artPath + "/body");
    a.author = readFile(artPath + "/author");
    istringstream(s_artIdent) >> a.ident;

    return a;
}

bool
DiskDatabase::existsNewsgroup(unsigned int newsIdent)
{
    stringstream out;
    out << newsIdent;
    string s_newsIdent = out.str();
    if(opendir((dbname + "/" + s_newsIdent).c_str()) == NULL)
        return false;
    return true;
}

string
DiskDatabase::readFile(string file){
    ifstream indata;
    indata.open(file.c_str());
    string result((istreambuf_iterator<char>(indata)),
                     istreambuf_iterator<char>());
    indata.close();
    return result;
}
 

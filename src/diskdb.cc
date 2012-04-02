

#include <algorithm>
#include "diskdb.h"
#include <vector>
#include <dirent.h>
#include <string.h>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <streambuf>

//#define TRACE_DISKDB
#ifdef TRACE_DISKDB
#define trace cerr
#else
#define trace if(false) cerr
#endif


using namespace std;
using namespace sus;

class DirIterator {
    public:
        DirIterator(DIR* dirp) : dirp(dirp), entry(0) {
            trace << "opened dir: " << (long) dirp << endl;
            if (dirp) {
                operator++();
            } 
        }

        string operator*() const { return entry->d_name; }

        DirIterator& operator++() {
            // Ignore uninteresting files.
            do {
                entry = readdir(dirp);
            } while (entry &&
                     (!strcmp(entry->d_name, ".")  ||
                      !strcmp(entry->d_name, "..") ||
                      !strcmp(entry->d_name, "name")
                     )
                    );

            return *this;
        }

        bool operator!=(const DirIterator& rhs) {
            trace << "comparing: " << entry << " - " << rhs.entry << endl;
            return entry != rhs.entry;
        }

    private:
        DIR* dirp;
        dirent* entry;
};

class Directory {
    public:
        typedef DirIterator iterator;

        Directory(string path) : dirp(opendir(path.c_str())) {}

        ~Directory() {
            if (dirp) {
                closedir(dirp);
            }
        }

        bool exist() {
            return dirp != nullptr;
        }

        iterator begin() const { return iterator(dirp); }

        iterator end() const { return iterator(0); }
    private:
        DIR* dirp;
};


DiskDatabase::DiskDatabase(const string& dbpath) : dbpath(dbpath)
{
    Directory d(dbpath);
    if (!d.exist()) {
        mkdir(dbpath.c_str(), 0755);
    }
}

vector<Newsgroup>
DiskDatabase::getNewsgroups()
{
    trace << "getNewsgroups()" << endl;
    Directory dir(dbpath);
    vector<Newsgroup> groups;
    trace << "Found: " << endl;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        Newsgroup n;
        n.name = readFile(dbpath + "/" + *it + "/" + "name");
        istringstream(*it) >> n.ident;
        groups.push_back(n);
        trace << n.ident << " --  " << n.name << endl;
    }
    sort(groups.begin(),groups.end(),
        [](const Newsgroup& a,const Newsgroup& b) {return a.ident < b.ident;});

    trace << "done" << endl;
    return groups;
}


bool
DiskDatabase::createNewsgroup(const string& name)
{

    Directory dir(dbpath);

    //find the max ident
    unsigned int maxIdent = 0;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        unsigned int ident;
        istringstream(*it) >> ident;
        if(readFile(dbpath + "/" + *it + "/" + "name") == name){
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

    mkdir((dbpath + "/" + ident).c_str(),0755);

    ofstream write (dbpath + "/" + ident + "/name");
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
    
    Directory newsGroups((dbpath + "/" + ident));

    if (!newsGroups.exist()) {
        return false;
    }

    for(auto it = newsGroups.begin(); it != newsGroups.end(); ++it){
        string artPath(dbpath + "/" + ident + "/" + *it);
        remove((artPath + "/body").c_str());
        remove((artPath + "/author").c_str());
        remove((artPath + "/title").c_str());
        rmdir(artPath.c_str());
    }

    remove((dbpath + "/" + ident + "/name").c_str());
    rmdir((dbpath + "/" + ident).c_str());
    return true;
}

vector<Article>
DiskDatabase::getArticles(unsigned int newsIdent)
{
    stringstream out;
    out << newsIdent;
    string ident = out.str();
    vector<Article> articles;

    Directory dir((dbpath + "/" + ident));
    for(auto it = dir.begin(); it != dir.end(); ++it){
        Article a;
        string artPath = dbpath + "/" + ident + "/" + *it;
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

    Directory dir((dbpath + "/" + s_newsIdent));

    // Newsgroup does not exist.
    if (!dir.exist()) {
        return false;
    }

    //find the max ident
    unsigned int maxIdent = 0;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        unsigned int ident;
        istringstream(*it) >> ident;
        if(ident > maxIdent){
            maxIdent = ident;
        }
    }

    ++maxIdent;

    stringstream out;
    out << maxIdent;
    string ident = out.str();
    string artPath = dbpath + "/" + s_newsIdent + "/" + ident;

    mkdir(artPath.c_str(),0755);

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

    string artPath = dbpath + "/" + s_newsIdent + "/" + s_artIdent;

    Directory* d = new Directory(artPath);
    // Newsgroup does not exist.
    if (!d->exist()) {
        delete d;
        return false;
    }

    delete d;

    remove((artPath + "/body").c_str());
    remove((artPath + "/author").c_str());
    remove((artPath + "/title").c_str());
    rmdir(artPath.c_str());

    return true;
}

bool
DiskDatabase::existsNewsgroup(unsigned int newsIdent)
{
    stringstream out;
    out << newsIdent;
    string s_newsIdent = out.str();
    
    Directory d(dbpath + "/" + s_newsIdent);
    if (!d.exist()) {
        return false;
    } else {
        return true;
    }
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
 

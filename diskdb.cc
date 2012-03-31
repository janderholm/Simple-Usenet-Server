

#include <algorithm>
#include "diskdb.h"
#include <vector>
#include <dirent.h>
#include <string.h>
#include <iostream> 
#include <fstream>
#include <sstream>

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

class Directory { //newsgroups i guess
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

vector<Newsgroup>
DiskDatabase::getNewsgroups()
{
    // TODO: paths should atleast be global constants
    string dirname = "diskdb";
    Directory dir(dirname.c_str());
    vector<Newsgroup> groups;
    for(auto it = dir.begin(); it != dir.end(); ++it){
        Newsgroup n;
        n.name = readFile(dirname + "/" + *it + "/" + "name");
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
    return false;
}


bool
DiskDatabase::deleteNewsgroup(unsigned long newsIdent)
{
    return false;
}

vector<Article>
DiskDatabase::getArticles(unsigned long newsIdent)
{
    return vector<Article>();
}

bool
DiskDatabase::createArticle(unsigned long newsIdent, const string& title,
                const string& author, const string& body)
{   
    return false;
}

bool
DiskDatabase::deleteArticle(unsigned long newsIdent, unsigned long artIdent)
{
    return false;
}

const Article*
DiskDatabase::getArticle(unsigned long newsIdent, unsigned long artIdent)
{

    return 0;
}

string
DiskDatabase::readFile(string file){
    ifstream indata;
    string result;
    string c;
    indata.open(file); // opens the file
    if(!indata) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      exit(1);
    }

    indata >> result;
    while (true) { // keep reading until end-of-file
        indata >> c; // sets EOF flag if no value found
        if(indata.eof()){
            break;
        }
        result += c;
    }
    return result;
}
 

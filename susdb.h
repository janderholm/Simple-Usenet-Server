#ifdef SUSDB_H
#define SUSDB_H

#include <ctime>
#include <string>

namespace sus {
    struct Article {
        string title;
        string author;
        string body;
        unsigned long ident;
        time_t timestamp;
    }

    struct Newsgroup {
        unsigned long ident;
    }


    class DatabaseInterface {
        public:
            
    }
}

#endif

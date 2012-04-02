#include <string>
#include <iostream>
#include "utils.h"
#include "protocol.h"

//#define TRACE_UTILS

#ifdef TRACE_UTILS
#define trace cerr
#else
#define trace if(false) cerr
#endif


using namespace std;
using namespace sus;
using namespace client_server;
using namespace protocol;

MessageHandler::MessageHandler(Connection* connection) :
	connection(connection) {}

unsigned int
MessageHandler::readNum()
{
    trace << "Reading number" << endl;
    unsigned int n;
    char* nc = (char*) &n;

    for (int i = 3; i >= 0; --i) {
        nc[i] = connection->read();
    }
    
    trace << "Read: " << n << endl;
    return n;
}

string
MessageHandler::readString()
{
    trace << "Reading string" << endl;
    unsigned int n = readNum();
    string s(n, '\0');
    for (unsigned int i = 0; i < n; ++i) {
        s[i] = connection->read();
    }
    trace << "Read: " << s << endl;
    return s;
}

void
MessageHandler::writeNum(unsigned int n)
{
    trace << "writeNum: " << n << endl;
    // TODO: static_cast
    char* nb = (char*) &n;

    // TODO: Portability.
    for (int i = 3; i >= 0; --i) {
        trace << "write byte nb " << i << endl;
        connection->write(nb[i]);
    }
    trace << "done" << endl;
}


void
MessageHandler::writeString(string& s)
{
    unsigned int n = s.size();
    writeNum(n);
    trace << "writing: " << n << " characters" << endl;
    trace << "of " << s << endl;
    for (unsigned int i = 0; i < n; ++i) {
        connection->write(s[i]);
    }
    trace << "done" << endl;
}


#include <string>
#include "utils.h"
#include "clientserver/protocol.h"

using namespace std;
using namespace sus;
using namespace client_server;
using namespace protocol;

MessageHandler::MessageHandler(Connection* connection) :
	connection(connection) {}

int
MessageHandler::readNum()
{
    int n;
    char* nc = (char*) &n;

    for (int i = 0; i < 4; ++i) {
        nc[i] = connection->read();
    }
    
    return n;
}

string
MessageHandler::readString()
{
    int n = readNum();
    string s(n, '\0');
    for (int i = 0; i < n; ++i) {
        s[i] = connection->read();
    }
    return s;
}

void
MessageHandler::writeNum(int n)
{
    // TODO: static_cast
    char* nb = (char*) &n;

    // TODO: Portability.
    for (int i = 0; i < 4; ++i) {
        connection->write(nb[0]);
    }
}


void
MessageHandler::writeString(string& s)
{
    int n = s.size();
    writeNum(n);
    for (int i = 0; i < n; ++i) {
        connection->write(s[i]);
    }

}


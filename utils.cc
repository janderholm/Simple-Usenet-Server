#include "utils.h"
#include "clientserver/protocol.h"



using namespace std;
using namespace sus;
using namespace client_server;
using namespace protocol;

ConnectionHandler::ConnectionHandler(Connection* connection) :
	connection(connection) {}

int
ConnectionHandler::readNum()
{
    int n;
    char* nc = (char*) &n;

    for (int i = 0; i < 4; ++i) {
        nc[i] = connection->read();
    }
    
    return n;
}

string
ConnectionHandler::readString()
{
    int n = readNum();
    string s(n, '\0');
    for (int i = 0; i < n; ++i) {
        s[i] = connection->read();
    }
    return s;
}

void
ConnectionHandler::writeNum(int n)
{
    // TODO: static_cast
    char* nb = (char*) &n;

    // TODO: Portability.
    for (int i = 0; i < 4; ++i) {
        connection->write(nb[0]);
    }
}


void
ConnectionHandler::writeString(string& s)
{
    int n = s.size();
    writeNum(n);
    for (int i = 0; i < n; ++i) {
        connection->write(s[i]);
    }

}


#ifndef UTILS_H
#define UTILS_H


#include <string>
#include "server.h"
#include "connection.h"
#include "protocol.h"

using namespace std;
using namespace client_server;

namespace sus
{
class MessageHandler
{
public:
    MessageHandler(Connection* connection);
    string readString();
    unsigned int readNum();
    void writeNum(unsigned int n);
    void writeString(string& s);
protected:
    Connection* connection;
};
}

#endif

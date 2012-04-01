#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H


#include <string>
#include "clientserver/server.h"
#include "clientserver/connection.h"
#include "clientserver/protocol.h"

using namespace std;
using namespace client_server;

namespace sus {
    class ConnectionHandler {
        public:
            ConnectionHandler(Connection* connection);
            string readString();
            int readNum();
            void writeNum(int n);
            void writeString(string& s);
        private:
            Connection* connection;
    };
}

#endif

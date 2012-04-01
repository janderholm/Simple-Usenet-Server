
#ifndef MHANDLER_H
#define MHANDLER_H

#include <iostream>
#include <sstream>
#include <string>
#include "susdb.h"
#include "mhandler.h"
#include "clientserver/server.h"
#include "clientserver/connection.h"
#include "clientserver/connectionclosedexception.h"
#include "clientserver/protocol.h"

using namespace std;
using namespace sus;
using namespace client_server;
using namespace protocol;

class MessageHandler {
    public:
        MessageHandler(Connection* connection, DatabaseInterface* db):
            connection(connection), db(db) {}

        void writeNum(int n){
            // TODO: static_cast
            char* nb = (char*) &n;
            connection->write(Protocol::PAR_NUM);

            // TODO: Portability.
            for (int i = 0; i < 4; ++i) {
                connection->write(nb[0]);
            }
        }

        void writeString(string& s){
            int n = s.size();
            connection->write(Protocol::PAR_STRING);
            writeNum(n);
            for (int i = 0; i < n; ++i) {
                connection->write(s[i]);
            }
        }

        virtual void listNG() = 0;
    protected:
        Connection* connection;
        DatabaseInterface* db;
};

#endif

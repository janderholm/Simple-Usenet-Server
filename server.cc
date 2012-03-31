#include <iostream>
#include <sstream>
#include <string>

#include "susdb.h"
#include "inmemorydb.h"
//#include "diskdb.h"
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
        MessageHandler(Connection* connection, DatabaseInterface* db);
        void listNG();
        void writeNum(int n);
        void writeString(string& s);
    private:
        Connection* connection;
        DatabaseInterface* db;
};

MessageHandler::MessageHandler(Connection* connection, DatabaseInterface* db):
    connection(connection), db(db) {}


void
MessageHandler::writeNum(int n)
{
    // TODO: static_cast
    char* nb = (char*) &n;
    connection->write(Protocol::PAR_NUM);

    // TODO: Portability.
    for (int i = 0; i < 4; ++i) {
        connection->write(nb[0]);
    }
}

void
MessageHandler::writeString(string& s)
{
    int n = s.size();

    connection->write(Protocol::PAR_STRING);
    writeNum(n);
    for (int i = 0; i < n; ++i) {
        connection->write(s[i]);
    }

}

void
MessageHandler::listNG()
{
    // Leave any exceptions to a higher power.
    char b = connection->read();
    if (b == Protocol::COM_END) {
        auto ng = db->getNewsgroups();
        connection->write(Protocol::ANS_LIST_NG);
        writeNum(ng.size());
        for (auto it = ng.begin(); it != ng.end(); ++it) {
            writeNum(it->ident);
            writeString(it->name);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << b << "in listNG" << endl;
    }
}



int
main(int argc, const char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " PORT" << endl;
        return 1;
    }

    int port;
    istringstream(argv[1]) >> port;

    Server server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error" << endl;
        return 1;
    }


    DatabaseInterface* db = new InMemoryDatabase();
    //DatabaseInterface* db = new DiskDatabase();
    char b;

    while (true) {
        Connection* connection = server.waitForActivity();
        MessageHandler handle(connection, db);
        if (connection != nullptr) {
            try {
                b = connection->read();

                switch (b) {
                    case Protocol::COM_LIST_NG:
                        handle.listNG();
                        break;
                    case Protocol::COM_CREATE_NG:
                        break;
                    case Protocol::COM_DELETE_NG:
                        break;
                    case Protocol::COM_LIST_ART:
                        break;
                    case Protocol::COM_DELETE_ART:
                        break;
                    case Protocol::COM_GET_ART:
                        break;
                    case Protocol::COM_END:
                        break;
                }


                
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(connection);
                delete connection;
                cout << "Client closed connection" << endl;
            }
        } else {
            server.registerConnection(new Connection);
            cout << "New client connects" << endl;
        }
    }

    return 0;
}

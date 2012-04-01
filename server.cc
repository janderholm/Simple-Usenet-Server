#include <iostream>
#include <sstream>
#include <string>

#include "susdb.h"
#include "inmemorydb.h"
//#include "diskdb.h"
#include "utils.h"
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
        MessageHandler(Connection* c, DatabaseInterface* db);
        void listNG();
        void createNG();
        void deleteNG();
    private:
        DatabaseInterface* db;
        Connection* connection;
        ConnectionHandler ch;
};


MessageHandler::MessageHandler(Connection* c, DatabaseInterface* db) :
    connection(c), ch(c) {}

void
MessageHandler::listNG()
{
    // Leave any exceptions to a higher power.
    char b = connection->read();
    if (b == Protocol::COM_END) {
        auto ng = db->getNewsgroups();
        connection->write(Protocol::ANS_LIST_NG);
        connection->write(Protocol::PAR_NUM);
        ch.writeNum(ng.size());
        for (auto it = ng.begin(); it != ng.end(); ++it) {
            connection->write(Protocol::PAR_NUM);
            ch.writeNum(it->ident);
            connection->write(Protocol::PAR_STRING);
            ch.writeString(it->name);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << b << "in listNG" << endl;
    }
}


void
MessageHandler::createNG()
{
    char b = connection->read();
    string s;
    if (b == Protocol::PAR_STRING) {
        s = ch.readString();
    } else {
        cerr << "Malformed message byte: " << b << "in createNG" << endl;
    }

    b = connection->read();
    if (b == Protocol::ANS_END) {
        if (db->createNewsgroup(s)) {
            connection->write(Protocol::ANS_ACK);
        } else {
            connection->write(Protocol::ANS_NAK);
            connection->write(Protocol::ERR_NG_ALREADY_EXISTS);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << b << "in createNG" << endl;
    }
}

void
MessageHandler::deleteNG()
{
    char b;
    int n;
    if ((b = connection->read()) == Protocol::PAR_NUM) {
        n = ch.readNum();
    } else {
        cerr << "Malformed message byte: " << b << "in deleteNG" << endl;
    }

    if ((b = connection->read()) == Protocol::COM_END) {
        if (db->deleteNewsgroup(n)) {
            connection->write(Protocol::ANS_ACK);
        } else {
            connection->write(Protocol::ANS_NAK);
            connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "BAD: " << b << "deleteNG" << endl;
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
                        handle.createNG();
                        break;
                    case Protocol::COM_DELETE_NG:
                        handle.deleteNG();
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

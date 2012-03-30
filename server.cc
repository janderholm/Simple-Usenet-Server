#include <iostream>
#include <sstream>

#include "susdb.h"
#include "inmemorydb.h"
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
    private:
        Connection* connection;
        DatabaseInterface* db;
};

MessageHandler::MessageHandler(Connection* connection, DatabaseInterface* db):
    connection(connection), db(db) {}

void
MessageHandler::listNG()
{
    // Leave any exceptions to a higher power.
    char b = connection->read();
    if (b == Protocol::COM_END) {
        //Send answer here;
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

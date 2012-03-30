#include <iostream>
#include <sstream>

#include "clientserver/server.h"
#include "clientserver/connection.h"
#include "clientserver/connectionclosedexception.h"

using namespace std;
using namespace client_server;


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

    while (true) {
        Connection* connection = server.waitForActivity();
        if (connection != nullptr) {
            try {
                cout << connection->read() << endl;
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

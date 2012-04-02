#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "susdb.h"
#include "inmemorydb.h"
#include "diskdb.h"
#include "utils.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

//#define TRACE_SERVER

#ifdef TRACE_SERVER
#define trace cerr
#else
#define trace if(false) cerr
#endif

using namespace std;
using namespace sus;
using namespace client_server;
using namespace protocol;


class ServerMessageHandler : public MessageHandler {
    public:
        ServerMessageHandler(Connection* connection, DatabaseInterface* db);
        bool listNG();
        bool createNG();
        bool deleteNG();
        bool listArt();
        bool createArt();
        bool deleteArt();
        bool getArt();
    private:
        DatabaseInterface* db;
};


ServerMessageHandler::ServerMessageHandler(Connection* connection, DatabaseInterface* db) :
    MessageHandler(connection), db(db) {}

bool
ServerMessageHandler::listArt()
{
    char c;
    unsigned int n;

    if ((c = connection->read()) == Protocol::PAR_NUM) {
        n = readNum();
    } else {
        cerr << "Malformed message byte: " << hex << c << "in listArt" << endl;
        return true;
    }

    if ((c = connection->read()) == Protocol::COM_END) {
        connection->write(Protocol::ANS_LIST_ART);
        if (db->existsNewsgroup(n)) {
            connection->write(Protocol::ANS_ACK);
            auto arts = db->getArticles(n);
            connection->write(Protocol::PAR_NUM);
            writeNum(arts.size());

            for (auto it = arts.begin(); it != arts.end(); ++it) {
                connection->write(Protocol::PAR_NUM);
                writeNum(it->ident);
                connection->write(Protocol::PAR_STRING);
                writeString(it->title);
           }
        } else {
            trace << "ng ident: " << n << "did not exist" << endl;
            connection->write(Protocol::ANS_NAK); 
            connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << hex << c << "in listArt" << endl;
    }
    return false;
}

bool
ServerMessageHandler::createArt()
{
    char c;
    unsigned int n;
    string s[3];

    if ((c = connection->read()) == Protocol::PAR_NUM) {
        trace << "createArt() got num" << endl;
        n = readNum();
    } else {
        cerr << "Malformed message byte: " << hex << c << "in createArt" << endl;
        return true;
    }

    for (int i = 0; i < 3; ++i) {
        if ((c = connection->read()) == Protocol::PAR_STRING) {
            s[i] = readString();
        } else {
            cerr << "Malformed message byte: " << hex << c << "in createArt" << endl;
            return true;
        }
    }

    if ((c = connection->read()) == Protocol::COM_END) {
        connection->write(Protocol::ANS_CREATE_ART);
        if (db->createArticle(n, s[0], s[1], s[2])) {
            connection->write(Protocol::ANS_ACK);
        } else {
            connection->write(Protocol::ANS_NAK);
            connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << hex << c << "in createArt" << endl;
        return true;
    }
    return false;
}


bool
ServerMessageHandler::deleteArt()
{
    char c;
    unsigned int n[2];
    for (int i = 0; i < 2; ++i) {
        if ((c = connection->read()) == Protocol::PAR_NUM) {
            n[i] = readNum();
        } else {
            cerr << "Malformed message byte: " << hex << c << "in deleteArt" << endl;
            return true;
        }
    }

    if ((c = connection->read()) == Protocol::COM_END) {
        connection->write(Protocol::ANS_DELETE_ART);
        if(db->deleteArticle(n[0], n[1])) {
            connection->write(Protocol::ANS_ACK);
        } else {
            connection->write(Protocol::ANS_NAK);
            if (db->existsNewsgroup(n[0])) {
                connection->write(Protocol::ERR_ART_DOES_NOT_EXIST);
            } else {
                connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
            }
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << hex << c << "in deleteArt" << endl;
        return true;
    }
    return false;
}

bool
ServerMessageHandler::getArt()
{
    char c;
    unsigned int n[2];
    for (int i = 0; i < 2; ++i) {
        if ((c = connection->read()) == Protocol::PAR_NUM) {
            n[i] = readNum();
        } else {
            cerr << "Malformed message byte: " << hex << c << "in getArt" << endl;
            return true;
        }
    }

    if ((c = connection->read()) == Protocol::COM_END) {
        connection->write(Protocol::ANS_GET_ART);
        if (db->existsNewsgroup(n[0])) {
            auto arts = db->getArticles(n[0]);
            auto it = find_if(arts.begin(), arts.end(),
                    [&n](Article& a) {return a.ident == n[1];});
            if (it != arts.end()) {
                connection->write(Protocol::ANS_ACK);
                connection->write(Protocol::PAR_STRING);
                writeString(it->title);
                connection->write(Protocol::PAR_STRING);
                writeString(it->author);
                connection->write(Protocol::PAR_STRING);
                writeString(it->body);
            } else {
                connection->write(Protocol::ANS_NAK);
                connection->write(Protocol::ERR_ART_DOES_NOT_EXIST);
            }
        } else {
            connection->write(Protocol::ANS_NAK);
            connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << hex << c << "in getArt" << endl;
        return true;
    }
    return false;
}


bool
ServerMessageHandler::listNG()
{
    trace << "listNG()" << endl;
    // Leave any exceptions to a higher power.
    char b;

    if ((b = connection->read()) == Protocol::COM_END) {
        trace << "Got message" << endl;
        auto ng = db->getNewsgroups();
        connection->write(Protocol::ANS_LIST_NG);
        connection->write(Protocol::PAR_NUM);
        writeNum(ng.size());
        for (auto it = ng.begin(); it != ng.end(); ++it) {
            trace << "Sending ng: " << it->ident << endl;
            connection->write(Protocol::PAR_NUM);
            writeNum(it->ident);
            connection->write(Protocol::PAR_STRING);
            writeString(it->name);
        }
        connection->write(Protocol::ANS_END);
        trace << "Done listing" << endl;
    } else {
        cerr << "Malformed message byte: " << hex << b << "in listNG" << endl;
        return true;
    }
    return false;
}


bool
ServerMessageHandler::createNG()
{
    char b = connection->read();
    string s;
    if (b == Protocol::PAR_STRING) {
        trace << "createNG() got String" << endl;
        s = readString();
    } else {
        cerr << "Malformed message byte: " << hex << b << "in createNG" << endl;
        return true;
    }

    b = connection->read();
    if (b == Protocol::COM_END) {
        connection->write(Protocol::ANS_CREATE_NG);
        if (db->createNewsgroup(s)) {
            connection->write(Protocol::ANS_ACK);
        } else {
            connection->write(Protocol::ANS_NAK);
            connection->write(Protocol::ERR_NG_ALREADY_EXISTS);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << hex << b << "in createNG" << endl;
        return true;
    }
    return false;
}

bool
ServerMessageHandler::deleteNG()
{
    char b;
    int n;
    if ((b = connection->read()) == Protocol::PAR_NUM) {
        n = readNum();
    } else {
        cerr << "Malformed message byte: " << hex << b << "in deleteNG" << endl;
        
        return true;
    }

    if ((b = connection->read()) == Protocol::COM_END) {
        connection->write(Protocol::ANS_DELETE_NG);
        if (db->deleteNewsgroup(n)) {
            connection->write(Protocol::ANS_ACK);
        } else {
            connection->write(Protocol::ANS_NAK);
            connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << hex << b << "in deleteNG" << endl;
        return true;
    }
    return false;
}



int
main(int argc, const char *argv[])
{
#ifdef DISKDB
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " PORT PATH" << endl;
        cerr << "Start server on port PORT with database on PATH" << endl;
        return 1;
    }
#else
    if (!(argc == 2)) {
        cerr << "Usage: " << argv[0] << " PORT" << endl;
        cerr << "Start server on port PORT" << endl;
        return 1;
    }
#endif

    DatabaseInterface* db;
#ifdef DISKDB
    cout << "Starting persistant database in " << argv[2] << endl;
    db = new DiskDatabase(argv[2]);
#else
    cout << "Starting nonpersistand database in memory" << endl;
    db = new InMemoryDatabase();
#endif

    int port;
    istringstream(argv[1]) >> port;

    Server server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error" << endl;
        return 1;
    }

    char b;
    bool fail;

    while (true) {
        Connection* connection = server.waitForActivity();
        ServerMessageHandler handle(connection, db);
        if (connection != nullptr) {
            try {
                b = connection->read();

                switch (b) {
                    case Protocol::COM_LIST_NG:
                        trace << "LIST_NG" << endl;
                        fail = handle.listNG();
                        break;
                    case Protocol::COM_CREATE_NG:
                        trace << "CREATE_NG" << endl;
                        fail = handle.createNG();
                        break;
                    case Protocol::COM_DELETE_NG:
                        trace << "DELETE_NG" << endl;
                        fail = handle.deleteNG();
                        break;
                    case Protocol::COM_CREATE_ART:
                        trace << "CREATE_ART" << endl;
                        fail = handle.createArt();
                        break;
                    case Protocol::COM_LIST_ART:
                        trace << "LIST_ART" << endl;
                        fail = handle.listArt();
                        break;
                    case Protocol::COM_DELETE_ART:
                        trace << "DELETE_ART" << endl;
                        fail = handle.deleteArt();
                        break;
                    case Protocol::COM_GET_ART:
                        trace << "GET_ART" << endl;
                        fail = handle.getArt();
                        break;
                    default:
                        fail = false;
                        cerr << "not sure what to do with: ";
                        printf("%02x\n", b);
                }

                if(fail) {
                    server.deregisterConnection(connection);
                    delete connection;
                    cout << "Dropping missbehaving client" << endl;
                }

            } catch (ConnectionClosedException&) {
                server.deregisterConnection(connection);
                delete connection;
                cout << "Client closed connection" << endl;
                //break;
            }
        } else {
            server.registerConnection(new Connection);
            cout << "New client connects" << endl;
        }
    }

    delete db;
    return 0;
}

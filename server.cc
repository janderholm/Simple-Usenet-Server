#include <algorithm>
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


class ServerMessageHandler : public MessageHandler {
    public:
        ServerMessageHandler(Connection* connection, DatabaseInterface* db);
        void listNG();
        void createNG();
        void deleteNG();
        void listArt();
        void createArt();
        void deleteArt();
        void getArt();
    private:
        DatabaseInterface* db;
};


ServerMessageHandler::ServerMessageHandler(Connection* connection, DatabaseInterface* db) :
    MessageHandler(connection), db(db) {}

void
ServerMessageHandler::listArt()
{
    char c;
    unsigned int n;

    if ((c = connection->read()) == Protocol::PAR_NUM) {
        n = readNum();
    } else {
        cerr << "wrong: " << c << endl;
        return;
    }

    if ((c = connection->read()) == Protocol::COM_END) {
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
                connection->write(Protocol::PAR_STRING);
                writeString(it->author);
                connection->write(Protocol::PAR_STRING);
                writeString(it->body);
            }
        } else {
            connection->write(Protocol::ANS_NAK); 
            connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "wrong byte: " << c << endl;
    }


}

void
ServerMessageHandler::createArt()
{
    char c;
    unsigned int n;
    string s[3];

    if ((c = connection->read()) == Protocol::PAR_NUM) {
        n = readNum();
    } else {
        cerr << "ERR" << endl;
        return;
    }

    for (int i = 0; i < 3; ++i) {
        if ((c = connection->read()) == Protocol::PAR_STRING) {
            s[i] = readString();
        } else {
            cerr << "ERR" << endl;
            return;
        }
    }

    if ((c = connection->read()) == Protocol::COM_END) {
        if (db->createArticle(n, s[0], s[1], s[2])) {
            connection->write(Protocol::ANS_ACK);
        } else {
            connection->write(Protocol::ANS_NAK);
            connection->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "ERR" << endl;
    }
}


void ServerMessageHandler::deleteArt()
{
    char c;
    unsigned int n[2];
    for (int i = 0; i < 2; ++i) {
        if ((c = connection->read()) == Protocol::PAR_NUM) {
            n[i] = readNum();
        } else {
            cerr << "ERR" << endl;
            return;
        }
    }

    if ((c = connection->read()) == Protocol::COM_END) {
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
        cerr << "ERR" << endl;
        return;
    }
}

void ServerMessageHandler::getArt()
{
    char c;
    unsigned int n[2];
    for (int i = 0; i < 2; ++i) {
        if ((c = connection->read()) == Protocol::PAR_NUM) {
            n[i] = readNum();
        } else {
            cerr << "ERR" << endl;
            return;
        }
    }

    if ((c = connection->read()) == Protocol::COM_END) {
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
    }
}


void
ServerMessageHandler::listNG()
{
    // Leave any exceptions to a higher power.
    char b = connection->read();
    if (b == Protocol::COM_END) {
        auto ng = db->getNewsgroups();
        connection->write(Protocol::ANS_LIST_NG);
        connection->write(Protocol::PAR_NUM);
        writeNum(ng.size());
        for (auto it = ng.begin(); it != ng.end(); ++it) {
            connection->write(Protocol::PAR_NUM);
            writeNum(it->ident);
            connection->write(Protocol::PAR_STRING);
            writeString(it->name);
        }
        connection->write(Protocol::ANS_END);
    } else {
        cerr << "Malformed message byte: " << b << "in listNG" << endl;
    }
}


void
ServerMessageHandler::createNG()
{
    char b = connection->read();
    string s;
    if (b == Protocol::PAR_STRING) {
        s = readString();
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
ServerMessageHandler::deleteNG()
{
    char b;
    int n;
    if ((b = connection->read()) == Protocol::PAR_NUM) {
        n = readNum();
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
        ServerMessageHandler handle(connection, db);
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

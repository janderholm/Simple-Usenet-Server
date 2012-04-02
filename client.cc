/*
list                                lists newsgorup or articles
open   [ident]                      open a newsgroup
close                               close current newsgroup
read   [ident]                      prints the article
create                              create newsgroup
delete [ident]                      delete newgroup or article depending on current state.
exit                                exits the client
*/

#include "clientserver/connection.h"
#include "clientserver/connectionclosedexception.h"
#include "clientserver/protocol.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <cstdlib>    /* for exit() and atoi() */
#include <sstream>
#include <limits>

using namespace std;
using namespace sus;
using namespace client_server;
using namespace protocol;


class ClientMessageHandler : public MessageHandler {
    public:
        ClientMessageHandler(Connection* connection);
        void listNG();
        void createNG();
        void deleteNG(stringstream&);
        bool openNG(stringstream&,int&);
        void listArt(int& newsIdent);
        void createArt(int& newsIdent);
};

ClientMessageHandler::ClientMessageHandler(Connection* connection) :
    MessageHandler(connection) {}

void ClientMessageHandler::listNG(){
    connection->write(Protocol::COM_LIST_NG);
    connection->write(Protocol::COM_END);
    char b;
    if((b = connection->read()) != Protocol::ANS_LIST_NG){
        cerr << "Malformed message byte: " << b << " in listNG" << endl;
        exit(1);
    }
    if((b = connection->read()) != Protocol::PAR_NUM){        
        cerr << "Malformed message byte: " << b << " in listNG" << endl;
        exit(1);
    }
    int size = readNum();

    for(int i = 0; i != size; ++i){
        if((b = connection->read()) != Protocol::PAR_NUM){
            cerr << "Malformed message byte: " << b << " in listNG" << endl;
            exit(1);
        }
        int ident = readNum();
        if((b = connection->read()) != Protocol::PAR_STRING){
            cerr << "Malformed message byte: " << b << " in listNG" << endl;
            exit(1);
        }
        string name = readString();
        cout << ident << " " << name << endl; 
    }
    b = connection->read();
    if(b != Protocol::ANS_END){
        cerr << "Malformed message byte: " << b << " in listNG" << endl;
        exit(1);
    }
}

void ClientMessageHandler::listArt(int& newsIdent){
    connection->write(Protocol::COM_LIST_ART);
    connection->write(Protocol::PAR_NUM);
    writeNum(newsIdent);
    connection->write(Protocol::COM_END);

    char b;
    if((b = connection->read()) != Protocol::ANS_LIST_ART){
        cerr << "Malformed message byte: " << b << " in listArt" << endl;
        exit(1);
    }
    
    b = connection->read();
    if((b != Protocol::ANS_NAK) && (b != Protocol::ANS_ACK)){
        cerr << "Malformed message byte: " << b << " in listArt" << endl;
        exit(1);
    }
    
    if(b == Protocol::ANS_NAK){
        if((b = connection->read()) != Protocol::ERR_NG_DOES_NOT_EXIST){
            cerr << "Malformed message byte: " << b << " in listArt" << endl;
            exit(1);
        }
        cerr << "Error: the newsgroup does not exist" << endl;
    }

    if(b == Protocol::ANS_ACK){
        if((b = connection->read()) != Protocol::PAR_NUM){
                cerr << "Malformed message byte: " << b << " in listArt" << endl;
                exit(1);
        }
        int size = readNum();
        for(int i = 0; i != size; ++i){
            if((b = connection->read()) != Protocol::PAR_NUM){
                cerr << "Malformed message byte: " << b << " in listArt" << endl;
                exit(1);
            }
            int ident = readNum();
            if((b = connection->read()) != Protocol::PAR_STRING){
                cerr << "Malformed message byte: " << b << " in listArt" << endl;
                exit(1);
            }
            string title = readString();
            cout << ident << " " << title << endl;
        }
    }

    b = connection->read();
    if(b != Protocol::ANS_END){
        cerr << "Malformed message byte: " << b << " in listNG" << endl;
        exit(1);
    }
}

void ClientMessageHandler :: createNG() {
    string name;
    cout << "enter a newsgroup name: ";
    getline (cin, name);
    connection->write(Protocol::COM_CREATE_NG);
    connection->write(Protocol::PAR_STRING);
    writeString(name);
    connection->write(Protocol::COM_END);
    
    char b;
    if((b = connection->read()) != Protocol::ANS_CREATE_NG){
        cerr << "Malformed message byte: " << b << " in createNG" << endl;
        exit(1);
    }

    b = connection->read();
    if((b != Protocol::ANS_NAK) && (b != Protocol::ANS_ACK)){
        cerr << "Malformed message byte: " << b << " in createNG" << endl;
        exit(1);
    }
    
    if(b == Protocol::ANS_NAK){
        if((b = connection->read()) != Protocol::ERR_NG_ALREADY_EXISTS){
            cerr << "Malformed message byte: " << b << " in createNG" << endl;
            exit(1);
        }
        cerr << "Error: the newsgroup already exists" << endl;
    }
    
    if(b == Protocol::ANS_ACK){
    }

    if((b = connection->read()) != Protocol::ANS_END){
        cerr << "Malformed message byte: " << b << " in createNG" << endl;
        exit(1);
    }
}

void ClientMessageHandler :: createArt(int& newsIdent) {
    string title;
    string author;
    string body;
    cout << "enter an article title: ";
    getline (cin, title);
    cout << "enter an article author: ";
    getline (cin, author);
    cout << "enter article text: ";
    getline (cin, body);
    connection->write(Protocol::COM_CREATE_ART);
    connection->write(Protocol::PAR_NUM);
    writeNum(newsIdent);
    connection->write(Protocol::PAR_STRING);
    writeString(title);
    connection->write(Protocol::PAR_STRING);
    writeString(body);
    connection->write(Protocol::PAR_STRING);
    writeString(author);
    connection->write(Protocol::COM_END);
    
    char b;
    if((b = connection->read()) != Protocol::ANS_CREATE_ART){
        cerr << "Malformed message byte: " << b << " in createArt" << endl;
        exit(1);
    }

    b = connection->read();
    if((b != Protocol::ANS_NAK) && (b != Protocol::ANS_ACK)){
        cerr << "Malformed message byte: " << b << " in createArt" << endl;
        exit(1);
    }
    
    if(b == Protocol::ANS_NAK){
        if((b = connection->read()) != Protocol::ERR_NG_DOES_NOT_EXIST){
            cerr << "Malformed message byte: " << b << " in createArt" << endl;
            exit(1);
        }
        cerr << "Error: the newsgroup does not exists" << endl;
    }
    
    if((b = connection->read()) != Protocol::ANS_END){
        cerr << "Malformed message byte: " << b << " in createArt" << endl;
        exit(1);
    }
}

void ClientMessageHandler :: deleteNG(stringstream& ss) {
    int newsIdent;
    if(!(ss >> newsIdent)){
        cerr << "Error: Could not parse the newsIdent" << endl;
        return;
    }
    connection->write(Protocol::COM_DELETE_NG);
    connection->write(Protocol::PAR_NUM);
    writeNum(newsIdent);
    connection->write(Protocol::COM_END);

    char b;
    if((b = connection->read()) != Protocol::ANS_DELETE_NG){
        cerr << "Malformed message byte: " << b << " in deleteNG" << endl;
        exit(1);
    }

    b = connection->read();
    if(b != Protocol::ANS_NAK && b != Protocol::ANS_ACK){
        cerr << "Malformed message byte: " << b << " in deleteNG" << endl;
        exit(1);
    }
    
    if(b == Protocol::ANS_NAK){
        if((b = connection->read()) != Protocol::ERR_NG_DOES_NOT_EXIST){
            cerr << "Malformed message byte: " << b << " in deleteNG" << endl;
            exit(1);
        }
        cerr << "Error: the newsgroup does not exist" << endl;
    }

    if((b = connection->read()) != Protocol::ANS_END){
        cerr << "Malformed message byte: " << b << " in deleteNG" << endl;
        exit(1);
    }
}

bool ClientMessageHandler :: openNG(stringstream& ss, int& newsIdent) {
    if(!(ss >> newsIdent)){
        cerr << "Error: Could not parse the newsIdent" << endl;
        return false;
    }
    return true;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }
    
    Connection* conn = new Connection(argv[1], atoi(argv[2]));
    if (! conn->isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
    
    ClientMessageHandler handle(conn);

    //translate text into number
    string input;
    string command;
    int newsIdent;
    bool inNewsgroup = false;
    while (true) {
        try {
            cout << "news> ";
            stringstream ss;
            getline(cin, input);
            ss.str(input);
            ss >> command;

            if(command == "list"){
                if(inNewsgroup)
                   handle.listArt(newsIdent);
                else
                   handle.listNG();
            }else if(command == "create"){
                if(inNewsgroup)
                    handle.createArt(newsIdent);                   
                else
                    handle.createNG();
            }else if(command == "delete"){
                handle.deleteNG(ss);
            }else if(command == "open"){
                if(inNewsgroup)
                    cerr << "Error: Already in newsgroup " << newsIdent << endl;
                else
                    inNewsgroup = handle.openNG(ss,newsIdent);
            }else if(command == "close"){
                inNewsgroup=false;
            }else if(command == "exit"){
                delete conn;
                exit(1);
            }else{
                cerr << "Error: No such command exists" << endl;
            }
        }catch (ConnectionClosedException&) {
            cerr << "Server closed down!" << endl;
            delete conn;
            exit(1);
        }
    }
}

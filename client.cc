/*
list                                lists newsgorup or articles
open   [ident]                      open a newsgroup
close                               close current newsgroup
read   [ident]                      prints the article
create                              create newsgroup
delete [ident]                      delete newgroup or article depending on current state.
*/

#include "clientserver/connection.h"
#include "clientserver/connectionclosedexception.h"
#include "clientserver/protocol.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <cstdlib>    /* for exit() and atoi() */
#include <sstream>

using namespace std;
using namespace sus;
using namespace client_server;
using namespace protocol;


class ClientMessageHandler : public MessageHandler {
    public:
        ClientMessageHandler(Connection* connection);
        void listNG();
};

ClientMessageHandler::ClientMessageHandler(Connection* connection) :
    MessageHandler(connection) {}

void ClientMessageHandler::listNG(){
    cout << "listNG" << endl;
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
    cout << "listNG wait for ANS_END" << endl;
    b = connection->read();
    if(b != Protocol::ANS_END){
        cerr << "Malformed message byte: " << b << " in listNG" << endl;
        exit(1);
    }
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
    while (true) {
        try {
            cout << "news> ";
            stringstream ss;
            getline(cin, input);
            ss.str(input);
            ss >> command;

            if(command == "list"){
                handle.listNG();
            }else{
                cerr << "No such command exists" << endl;
                exit(1);
            }
        }catch (ConnectionClosedException&) {
            cerr << "Server closed down!" << endl;
            delete conn;
            exit(1);
        }
    }
}

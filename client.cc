
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
    char b = connection->read();
    if(b == Protocol::ANS_LIST_NG){
        b = connection->read();
        int size;
        if(b == Protocol::PAR_NUM){
            size = readNum();
        }else{
            cerr << "Malformed message byte: " << b << "in listNG" << endl;
            exit(1);
        }

        for(int i = 0; i != size; ++i){
            b = connection->read();
            int ident;
            string name;
            if(b == Protocol::PAR_NUM){
                ident = readNum();
            }else{
                cerr << "Malformed message byte: " << b << "in listNG" << endl;
                exit(1);
            }
            b = connection->read();
            if(b == Protocol::PAR_STRING){
                name = readString();
            }else{
                cerr << "Malformed message byte: " << b << "in listNG" << endl;
                exit(1);
            }
            cout << ident << " " << name << endl; 
        }
    }else{
        cerr << "Malformed message byte: " << b << "in listNG" << endl;
        exit(1);
    }
    cout << "listNG wait for ANS_END" << endl;
    b = connection->read();
    if(b != Protocol::ANS_END){
        cerr << "Malformed message byte: " << b << "in listNG" << endl;
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

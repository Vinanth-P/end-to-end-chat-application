#include "client.h"
#include "../include/protocol.h"
#include<mutex>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>

using namespace std;

mutex coutMutex;


Client::Client(string ip,int port){
    serverIp=ip;
    serverPort=port;
}

bool Client::connectToServer(){
    clientSocket=socket(AF_INET,SOCK_STREAM,0);

    if(clientSocket<0){
        cout<<"Client socket creation failed"<<endl;
        return false;
    }
    cout<<"Client socket created successfully"<<endl;

    sockaddr_in serverAddress;
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_port=htons(serverPort);

    if(inet_pton(AF_INET,serverIp.c_str(),&serverAddress.sin_addr) <= 0){
        cout << "Invalid IP Address\n";
        return false;
    }

    if(connect(clientSocket,(sockaddr*)&serverAddress,sizeof(serverAddress))<0){
        cout<<"Connection failed"<<endl;
        close(clientSocket);
        return false;
    }
    cout << "Connected to server\n";
    return true;
}

void Client::run(){
    string username;
    cout<<"Enter your username: ";
    getline(cin, username);
    
    sendMessage(clientSocket, username);

    thread receiveThread([this]() {
        while (true) {
            string receivedMsg;
            if (!receiveMessage(clientSocket, receivedMsg)) {
                // Lock console to print disconnection message safely
                lock_guard<mutex> lock(coutMutex);
                cout << "\033[2K\rDisconnected from server." << endl;
                break;
            }

            // Lock the console while updating the UI
            {
                lock_guard<mutex> lock(coutMutex);
                // \033[2K clears the current line, \r moves cursor to beginning
                cout << "\033[2K\r" << receivedMsg << "\n";
                cout << "Enter message: " << flush;
            }
        }
    });
    receiveThread.detach();

    while(true){
        // Print the initial prompt safely
        {
            lock_guard<mutex> lock(coutMutex);
            cout << "Enter message: " << flush;
        }

        string message;
        getline(cin, message);

        if(message == "exit"){
            break;
        }

        sendMessage(clientSocket, message);
    }
}
void Client::stop(){
    if(clientSocket!=-1){
        close(clientSocket);
        cout<<"Disconnected from server"<<endl;
    }
}
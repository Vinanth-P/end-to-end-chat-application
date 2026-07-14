#include "server.h"
#include "../include/protocol.h"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <algorithm>
using namespace std;

//constructor
Server::Server(int port){
    this->port=port;
    serverSocket=-1;
}

bool Server::start(){
    serverSocket=socket(AF_INET,SOCK_STREAM,0);//ipv4,tcp,default

    if(serverSocket<0){
        cout<<"Failed to create socket"<<endl;
        return false;
    }

    cout<<"Socket created successfully"<<endl;

    int opt = 1;

    if (setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0){
    cout << "Failed to set socket options\n";

    close(serverSocket);

    return false;
    }

    sockaddr_in serverAddress;

    memset(&serverAddress,0,sizeof(serverAddress));

    serverAddress.sin_family=AF_INET;//ipv4
    serverAddress.sin_port=htons(port);
    serverAddress.sin_addr.s_addr=INADDR_ANY;

    //bind
    int result=bind(serverSocket,(sockaddr*)&serverAddress,sizeof(serverAddress));

    if(result<0){
        cout<<"Bind failed"<<endl;
        close(serverSocket);
        return false;
    }

    cout<<"Bind successful"<<endl;

    if(listen(serverSocket,10)<0){
        cout<<"Listen failed"<<endl;
        close(serverSocket);
        return false;
    }

    cout<<"Server is listening on port "<<port<<endl;

    return true;
}

void Server::run(){
    cout<<"Waiting for clients..."<<endl;

    while(true){
        sockaddr_in clientAddress;
        socklen_t clientSize=sizeof(clientAddress);

        int clientSocket=accept(serverSocket,(sockaddr*)&clientAddress,&clientSize);

        if(clientSocket<0){
            cout<<"Failed to accept client"<<endl;
            continue;
        }
        cout<<"Client connected!"<<endl;
        
        // add client to list
        lock_guard<mutex>lock(clientMux);
        
        clientSockets.push_back(clientSocket);
        
        thread clientThread(&Server::handleClient,this,clientSocket);
        clientThread.detach();
    }
}

void Server::handleClient(int clientSocket)
{
    // 1. Receive the username first
    string username = "Unknown";
    if (!receiveMessage(clientSocket, username) || username.empty()) {
        cout << "Client disconnected before sending username\n";
        close(clientSocket);
        return;
    }

    // 2. Safely store the username
    {
        lock_guard<mutex> lock(clientMux);
        clientNames[clientSocket] = username;
    }

    cout << "User '" << username << "' joined the chat.\n";
    string welcomeMessage = "----------------Welcome to the server, " + username + "!---------------";
    sendMessage(clientSocket, welcomeMessage);

    // Broadcast that a new user joined
    broadcast(clientSocket, "[" + username + "] has joined the chat.");

    while(true)
    {
        string chatMessage;
        if (!receiveMessage(clientSocket, chatMessage)) {
            cout << username << " disconnected\n";
            break;
        }
        string formattedMessage = "[" + username + "]: " + chatMessage;

        cout << formattedMessage << endl;
        
        broadcast(clientSocket, formattedMessage);
    }

    close(clientSocket);

    {
        lock_guard<mutex>lock(clientMux);

        // remove client from lists
        clientNames.erase(clientSocket);
        auto it=remove(clientSockets.begin(),clientSockets.end(),clientSocket);
        clientSockets.erase(it,clientSockets.end());
    }

    // Tell everyone else they left
    broadcast(clientSocket, "[" + username + "] disconnected");


} 

void Server::stop(){
    if(serverSocket!=-1){
        close(serverSocket);
        cout<<"Server stopped"<<endl;
    }
}

void Server::broadcast(int senderFd,const string &msg){
    lock_guard<mutex>lock(clientMux);

    for(int fd:clientSockets){
        if(fd==senderFd) continue;

        sendMessage(fd, msg);
    }
}
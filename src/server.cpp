#include "server.h"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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
        
        handleClient(clientSocket);
    }
}

void Server::handleClient(int clientSocket)
{
    string message = "Welcome to the server";

    send(clientSocket,message.c_str(),message.length(),0);

    while(true)
    {
        char buffer[1024];

        memset(buffer,0,sizeof(buffer));

        int bytesReceived =
            recv(clientSocket,buffer,sizeof(buffer),0);

        if(bytesReceived == 0)
        {
            cout << "Client disconnected\n";
            break;
        }

        if(bytesReceived < 0)
        {
            cout << "Receive failed\n";
            break;
        }

        cout << "[Client] "
             << buffer
             << endl;
    }

    close(clientSocket);
}

void Server::stop(){
    if(serverSocket!=-1){
        close(serverSocket);
        cout<<"Server stopped"<<endl;
    }
}
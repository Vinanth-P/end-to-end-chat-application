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
    
    return true;
}

void Server::run(){
    // minimal run loop placeholder
}

void Server::stop(){
    if(serverSocket!=-1){
        close(serverSocket);
        serverSocket=-1;
    }
}
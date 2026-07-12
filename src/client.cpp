#include "client.h"

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

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

    inet_pton(
    AF_INET,
    serverIp.c_str(),
    &serverAddress.sin_addr
    );

    if(connect(clientSocket,(sockaddr*)&serverAddress,sizeof(serverAddress))<0){
        cout<<"Connection failed"<<endl;
        close(clientSocket);
        return false;
    }
    cout << "Connected to server\n";
    return true;
}


void Client::run(){
    char buffer[1024];
    memset(buffer,0,sizeof(buffer));

    int byteReceived=recv(clientSocket,buffer,sizeof(buffer),0);

    if(byteReceived>0){
        cout<<buffer<<endl;
    }
    string message="Hello server!";

    send(clientSocket,message.c_str(),message.length(),0);
}

void Client::stop(){

}
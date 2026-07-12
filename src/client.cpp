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
    char buffer[1024];
    memset(buffer,0,sizeof(buffer));

    int byteReceived=recv(clientSocket,buffer,sizeof(buffer),0);

    if(byteReceived>0){
        cout<<buffer<<endl;
    }
    while(true){
        string message;

        cout<<"Enter message: ";

        getline(cin,message);

        if(message=="exit"){
            break;
        }

        send(clientSocket,message.c_str(),message.length(),0);
    }
}

void Client::stop(){
    if(clientSocket!=-1){
        close(clientSocket);
        cout<<"Disconnected from server"<<endl;
    }
}
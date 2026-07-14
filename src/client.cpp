#include "client.h"

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>

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
    string username;
    cout << "Enter your username: ";
    getline(cin, username);
    
    // Send username as the very first message
    send(clientSocket, username.c_str(), username.length(), 0);

    thread receiveThread([this]() {
        while (true) {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));

            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) {
                cout << "\nDisconnected from server." << endl;
                break;
            }

            cout << "\n" << buffer << "\nEnter message: " << flush;
        }
    });
    receiveThread.detach();

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